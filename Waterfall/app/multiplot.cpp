#include "multiplot.h"
#include "ui_multiplot.h"
#include <QToolBar>
#include <iostream>
#include <fstream>
#include <algorithm>

#define PI 3.14159265358979323846

MultiPlot::MultiPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MultiPlot)
{
    defaultFFTsize = 16384;
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    // Setup dockable widgets
    uiDockFft = new DockFft();
    addDockWidget(Qt::RightDockWidgetArea, uiDockFft);

    /* FFT timer & data */
    fft_timer = new QTimer(this);
    connect(fft_timer, SIGNAL(timeout()), this, SLOT(FftTimeout()));

    QToolBar *toolbar = addToolBar(tr("Waterfall"));
    toolbar->addWidget(ui->freqCtrl);
    toolbar->addWidget(ui->sMeter);
    toolbar->addWidget(ui->windowTypeSelect);
    toolbar->addWidget(ui->bufferProgress);
    toolbar->addWidget(ui->progressLabel);
    toolbar->addWidget(ui->numBlocksSelect);
    toolbar->addWidget(ui->numBlocksLabel);

    ui->bufferProgress->setFormat(QString(""));

    /* frequency control widget */
    ui->freqCtrl->setup(10, (quint64) 0, (quint64) 9999e6, 1, UNITS_MHZ);
    ui->freqCtrl->setFrequency(144500000);

    /* Connect the frequency control widget to main window */
    connect(ui->freqCtrl, SIGNAL(newFrequency(qint64)), this, SLOT(setNewFrequency(qint64)));

    /* meter timer */
    meter_timer = new QTimer(this);
    connect(meter_timer, SIGNAL(timeout()), this, SLOT(meterTimeout()));

    /* Connect GUI with FFT rate and size and the FFT dockable widget */
    connect(this, SIGNAL(fftRateChanged(int)), this, SLOT(setFftRate(int)));
    connect(this, SIGNAL(fftSizeChanged(int)), this, SLOT(setFftSize(int)));
    connect(uiDockFft, SIGNAL(fftSizeChanged(int)), this, SLOT(setFftSize(int)));
    connect(uiDockFft, SIGNAL(fftRateChanged(int)), this, SLOT(setFftRate(int)));
    connect(uiDockFft, SIGNAL(fftSplitChanged(int)), this, SLOT(setIqFftSplit(int)));
    connect(uiDockFft, SIGNAL(fftAvgChanged(float)), this, SLOT(setIqFftAvg(float)));
    connect(uiDockFft, SIGNAL(fftZoomChanged(float)), ui->plotter, SLOT(zoomOnXAxis(float)));
    connect(uiDockFft, SIGNAL(resetFftZoom()), ui->plotter, SLOT(resetHorizontalZoom()));
    connect(uiDockFft, SIGNAL(gotoFftCenter()), ui->plotter, SLOT(moveToCenterFreq()));
    connect(uiDockFft, SIGNAL(gotoDemodFreq()), ui->plotter, SLOT(moveToDemodFreq()));
    connect(uiDockFft, SIGNAL(fftColorChanged(QColor)), this, SLOT(setFftColor(QColor)));
    connect(uiDockFft, SIGNAL(fftFillToggled(bool)), this, SLOT(setFftFill(bool)));
    connect(uiDockFft, SIGNAL(fftPeakHoldToggled(bool)), this, SLOT(setFftPeakHold(bool)));
    connect(uiDockFft, SIGNAL(peakDetectionToggled(bool)), this, SLOT(setPeakDetection(bool)));

    // Set min and max dB
    ui->plotter->setMaxDB(100);
    ui->plotter->setMinDB(-100);

    ui->plotter->setSampleRate(44100.0);
    ui->plotter->setSpanFreq(440.0);
    ui->plotter->setPercent2DScreen(30);
    ui->plotter->resetHorizontalZoom();
    ui->plotter->setCenterFreq(14500000);

    ui->numBlocksLabel->setText(" Number of Blocks");

    // FFT Initializations and memory allocation
    inputData = (double*) malloc(sizeof(double)*defaultFFTsize);
    fft = new Fft(inputData);
    currentFFTsize = fft->size();
    outputData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*currentFFTsize);
    outputFFTdata = (double*) malloc(sizeof(double)*currentFFTsize);

    // FFT Select Range
    fftSelRange = currentFFTsize/2;

    // Average FFT
    avgFFT = (double*) malloc(sizeof(double)*fftSelRange);

    // Variable initializations
    d_fftAvg = 0.5;
    windowType = -2; // Default is Hamming
    currentPoint = 0;
    stopPoint = 0;
    minValue = maxValue = 0.0;
    resetMinMax = true; // Reset the min and max
    streamFileLoaded = false; // Is the stream file loaded

    // Number of blocks
    numBlocks = 1;
    k= 1;

    // Fill avgFFT with zeros
    std::fill(avgFFT, avgFFT+fftSelRange,0);
}

MultiPlot::~MultiPlot()
{
    //Memory deallocation:
    fft_timer->stop();
    delete fft_timer;
    meter_timer->stop();
    delete meter_timer;
    //audio_fft_timer->stop();
    free(outputFFTdata);
    inputBuffer.clear();
    delete uiDockFft;
    delete ui;
    delete fft;
}

/*
    Sets the running of the waterfall plot when the timer is active. Executes
    the FFT on the input data and sends the resulting output data and the original
    input data to the plotter.
*/
void  MultiPlot::FftTimeout()
{

    if(!streamFileLoaded)
    {
        streamFileLoaded= loadStream();
    }
    performFFT();
    displayFFT();
}

/*
    Sets the FFT rate with the fps parameter passed as an argument.
*/
void MultiPlot::setFftRate(int fps)
{
    int interval;

    if (fps == 0)
    {
        interval = 36e7; // 100 hours
        ui->plotter->setRunningState(false);
    }
    else
    {
        interval = 1000 / fps;

        if (fft_timer->isActive())
            ui->plotter->setRunningState(true);
    }

    if (interval > 9 && fft_timer->isActive())
        fft_timer->setInterval(interval);
}

/*
    Sets the size of the FFT to the value passed as a parameter. The inputData
    and outputData containers are reallocated to hold the new size FFT. The
    inputData is then filled with data from the inputBuffer.
*/
void MultiPlot::setFftSize(int value)
{
    if(value > 0)
    {
        if(value != currentFFTsize)
        {
            inputData = (double*)malloc(sizeof(double)*value);
            outputData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*value);
            outputFFTdata = (double*) malloc(sizeof(double)*value);
        }
        fft->update(value, inputData);

        currentFFTsize = fft->size();
        fftSelRange = currentFFTsize/2;
        avgFFT = (double*) malloc(sizeof(double)*fftSelRange);
        for(size_t x=0; x < fftSelRange; x++)
        {
            avgFFT[x] = 0.0;
        }
    }
    else
    {
        printf("Invalid size value, fft unchanged.");
    }

}

/*! Set FFT plot color. */
void MultiPlot::setFftColor(const QColor color)
{
    ui->plotter->setFftPlotColor(color);
    //uiDockAudio->setFftColor(color); // Look into whether this is necessary
}

/*! Enable/disable filling the area below the FFT plot. */
void MultiPlot::setFftFill(bool enable)
{
    ui->plotter->setFftFill(enable);
    //uiDockAudio->setFftFill(enable);
}

/*! Set FFT peak hold. */
void MultiPlot::setFftPeakHold(bool enable)
{
    ui->plotter->setPeakHold(enable);
}

/*! Set FFT peak detection. */
void MultiPlot::setPeakDetection(bool enabled)
{
    ui->plotter->setPeakDetection(enabled ,2);
}

/*! Set IQ FFT split */
void MultiPlot::setIqFftSplit(int pct_wf)
{
    if ((pct_wf >= 10) && (pct_wf <= 100))
    {
        ui->plotter->setPercent2DScreen(pct_wf);
    }
}

/*! Set IQ FFT average. */
void MultiPlot::setIqFftAvg(float avg)
{
    d_fftAvg = avg;
}

/*
  Sets the input buffer, which holds the stream data from
  the loaded XML file. Memory is allocated to hold all the
  data sent and then the inputData is filled with buffer
  data up to the current FFT size.
*/
void MultiPlot::setBuffer(GNSSReader* input, size_t numStream)
{
    /* Sent from main window (pointers) */
    inputStream = input;
    streamNumber = numStream;
    inputBufferData = (double*) malloc(sizeof(double)*
                                                  inputStream->getDecStreamArray()[streamNumber]->getBufSize());
}
/*
    Switches the timer on or off. This is called from the main window's
    startWFbutton. It either sets the waterfall plots to run or quit running.
 */
void MultiPlot::switchTimer()
{
    if(!(fft_timer->isActive()))
    {
        fft_timer->start(50);
    }
    else
    {
        fft_timer->stop();
    }

}

/*
    Handles close event for this window.
*/
void MultiPlot::closeEvent(QCloseEvent *event)
{
    event->accept();
}

/* Sets the center frequency */
void MultiPlot::setNewFrequency(qint64 freq)
{
    if(ui->plotter->isVisible())
        ui->plotter->setCenterFreq(freq);
}

/*
   Sends the new center frequency to the frequency
    control object.
*/
void MultiPlot::on_plotter_newCenterFreq(qint64 f)
{
    ui->freqCtrl->setFrequency(f);
}

/*! \brief Signal strength meter timeout */
void MultiPlot::meterTimeout()
{
    float level;

    //level = rx->get_signal_pwr(true);
    level = 0.0;
    ui->sMeter->setLevel(level);
}

/*
    Set the window function type based on the selection from the combo box.
*/
void MultiPlot::on_windowTypeSelect_currentIndexChanged(const QString & text)
{
    std::string windowTypeStr = text.toLower().toStdString();
    if (windowTypeStr == "hamming")
    {
        windowType = -1;
    }
    else if(windowTypeStr == "hanning")
    {
        windowType = 0;
    }
    else if(windowTypeStr == "blackman-harris")
    {
        windowType = 1;
    }
    else if(windowTypeStr == "none")
    {
        windowType = -2;
    }
}

/*
    Set the number of blocks of FFTs to perform.
*/
void MultiPlot::on_numBlocksSelect_currentIndexChanged(const QString & text)
{
    numBlocks = text.toInt();
}

/*
    Load the stream for the FFT input data and place them into an input buffer
    for extracting blocks to perform FFTs on.
*/
bool MultiPlot::loadStream()
{

    /* Read file and place into input buffer */
    std::string inputFileName = "";
    try
    {
        inputFileName = inputStream->fileBeingDecoded();
    }
    catch(std::exception& e)
    {
        inputFileName = "Error getting file name";
        printf(e.what());
        printf("\n");
    }
    double highestValue = 0.0,lowestValue = 0.0;
    uint64_t count = 1;
    uint64_t value = 0;
    ui->bufferProgress->setMaximum(inputStream->getDecStreamArray()[streamNumber]->getBufSize());

    // Show the file being decoded and the number of bytes decoded.
    QString message;
    if(inputStream->isDone())
    {
        message.sprintf("File decoded: %s",inputFileName.c_str());
        ui->bufferProgress->setValue(int(inputBuffer.size()));
        QString labelText;
        labelText.sprintf("%i/%i bytes\t",inputBuffer.size(),ui->bufferProgress->maximum());
        ui->progressLabel->setText(labelText);
    }
    else
    {
        message.sprintf("Currently decoding: %s", inputFileName.c_str());
    }
    ui->statusbar->showMessage(message);

    // Load file data into the input buffer
    while(count != 0)
    {
        count = 0;
        double* tempBuf = (double*) malloc(sizeof(double)*
                                           inputStream->getDecStreamArray()[streamNumber]->getBufSize());

        tempBuf = inputStream->getDecStreamArray()[streamNumber]->flushOutputStream(&count);
        for(unsigned int i = 0; i < count; i++)
        {
            inputBuffer.push_back(tempBuf[i]);
            if(tempBuf[i] > highestValue)
            {
                highestValue = tempBuf[i];
            }
            else if(tempBuf[i] < lowestValue)
            {
                lowestValue = tempBuf[i];
            }
        }
        value = inputBuffer.size();

        // Convert buffer data to fftw_complex format. Need to optimize this section.
        memcpy(inputBufferData, inputBuffer.data(),inputBuffer.size());

        ui->bufferProgress->setValue(value);
        QString labelText;
        labelText.sprintf("%i/%i bytes\t",value,ui->bufferProgress->maximum());
        ui->progressLabel->setText(labelText);

        delete[] tempBuf;
    }
    return true;
}
/*
    Perform the FFT on each of the blocks as determined by the combo box.
    Retrieve blocks from the input buffer, apply window functions, perform the
    FFT and add it to the average FFT buffer.
*/
void MultiPlot::performFFT()
{
    // FFT is performed here:
  /////////////////////////////////////////////////////////////////////////


    // For each block, get data from the input buffer, perform window function
    // calculations and perform an FFT. After the FFT is done, add the square of
    // the real and imaginary components. Add this to the average FFT.
    for(;k <= numBlocks; k++)
    {
        // Pull block from stream
        stopPoint += int(currentFFTsize);
        size_t i = 0;
        for(;currentPoint < stopPoint; currentPoint++)
        {
            if(currentPoint == inputBuffer.size()-1)
            {
                stopPoint = int(currentFFTsize - i);
                currentPoint = 0;
            }
            inputData[i]= inputBuffer[currentPoint];

            // Calculate Window Functions - Hanning, Hamming, or Blackman-Harris
            // Or no window function at all
            double windowFactor;
            double a0 = 0.35875, a1 = 0.48829, a2=0.14128, a3 = 0.01168;
            double alpha = 0.54, beta = 1- alpha;

            switch(windowType)
            {
                case -2:
                    windowFactor = 1.0;
                    break;
                case 0:
                //Hanning
                    windowFactor= 0.5*(1 - cos((2*PI*i)/(currentFFTsize-1)));
                    break;
                case 1:
                    //Blackman-Harris
                    windowFactor = a0 - a1*cos((2*PI*i)/(currentFFTsize-1))
                        + a2*cos((4*PI*i)/(currentFFTsize-1))
                        - a3*cos((6*PI*i)/(currentFFTsize-1));
                    break;
                case -1:
                default:
                    //Hamming
                    windowFactor= alpha-beta*cos((2*PI*i)/(currentFFTsize-1));
                    break;
            }

            inputData[i] *= windowFactor;
            i++;
        }


        // Perform the FFT on this block
        outputData = fft->exec();

        // Calculuate I^2 + Q^2 and add to the average FFT buffer
        iqSquared();

    }

        k=1;  // Reset k to 1 for next round of blocks.

}

/*
    Calculate I^2 + Q^2 for each of the values in the fft selection range
    and add these to the average FFT buffer. This buffer will then be used
    for display data.
*/
void MultiPlot::iqSquared()
{
    // Avg FFT = Avg FFT + BlockFFT(FFTselRange)
    for(size_t i=0; i < fftSelRange; i++)
    {
        avgFFT[i] += outputData[i][0]*outputData[i][0] +
                outputData[i][1]*outputData[i][1];
    }
}

/*
    Prepare the average FFT data for a waterfall plot. For the values in the
    fft selection range, divide by numBlocks and take 10*log10 of the result.
    Keep track of the minimum and maximum value for scaling when graphing.
    After the calculations, set the average FFT buffer to all zeros. Plot the
    waterfall data.
*/
void MultiPlot::displayFFT()
{
    // Swap buffers;
    wfData = avgFFT;

    for(size_t x=0; x < fftSelRange; x++)
    {
        outputFFTdata[x] = 10*log10(wfData[x]/numBlocks);
        if(outputFFTdata[x] > maxValue)
        {
            maxValue = outputFFTdata[x];
        }
        else if(outputFFTdata[x] < minValue)
        {
            minValue = outputFFTdata[x];
        }
    }
    // Fill avgFFT with zeros
    std::fill(avgFFT, avgFFT+fftSelRange,0);

    // Send to plotter
    // Set FFT running state to true.
    ui->plotter->setRunningState(true);
    ui->plotter->setNewFttData(outputFFTdata,int(fftSelRange));

    if(resetMinMax)
    {
        ui->plotter->setMinDB(qint32(minValue));
        ui->plotter->setMaxDB(qint32(maxValue));
        resetMinMax = false;
    }    
}
