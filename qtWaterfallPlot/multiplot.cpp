#include "multiplot.h"
#include "ui_multiplot.h"
#include <QToolBar>
#include <iostream>

#define PI 3.14159265358979323846

MultiPlot::MultiPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MultiPlot)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);


    // Setup docks
    uiDockFft = new DockFft();
    uiDockRxOpt = new DockRxOpt();
    uiDockAudio = new DockAudio();
    addDockWidget(Qt::RightDockWidgetArea, uiDockFft);
    addDockWidget(Qt::RightDockWidgetArea, uiDockRxOpt);
    tabifyDockWidget(uiDockRxOpt,uiDockFft);

    // For now, hide Receiver dockable wiget
    uiDockRxOpt->hide();

    /* FFT timer & data */
    fft_timer = new QTimer(this);
    connect(fft_timer, SIGNAL(timeout()), this, SLOT(FftTimeout()));
    fft_timer->setInterval(10); // What does this even do?

    /* Audio timer */
    audio_fft_timer = new QTimer(this);
    //connect(audio_fft_timer, SIGNAL(timeout()), this, SLOT(audioFftTimeout()));

    QToolBar *toolbar = addToolBar(tr("Waterfall"));
    toolbar->addWidget(ui->freqCtrl);
    toolbar->addWidget(ui->sMeter);
    toolbar->addWidget(ui->windowTypeSelect);

    ui->menuView->addAction(uiDockRxOpt->toggleViewAction());
    ui->menuView->addAction(uiDockAudio->toggleViewAction());
    ui->menuView->addAction(uiDockFft->toggleViewAction());

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

    /* Connect GUI with the dockable widget for Receiver options */
    connect(uiDockRxOpt, SIGNAL(filterOffsetChanged(qint64)), this, SLOT(setFilterOffset(qint64)));
    connect(uiDockRxOpt, SIGNAL(demodSelected(int)), this, SLOT(selectDemod(int)));
    connect(uiDockRxOpt, SIGNAL(fmMaxdevSelected(float)), this, SLOT(setFmMaxdev(float)));
    connect(uiDockRxOpt, SIGNAL(fmEmphSelected(double)), this, SLOT(setFmEmph(double)));
    connect(uiDockRxOpt, SIGNAL(amDcrToggled(bool)), this, SLOT(setAmDcr(bool)));
    connect(uiDockRxOpt, SIGNAL(agcToggled(bool)), this, SLOT(setAgcOn(bool)));
    connect(uiDockRxOpt, SIGNAL(agcHangToggled(bool)), this, SLOT(setAgcHang(bool)));
    connect(uiDockRxOpt, SIGNAL(agcThresholdChanged(int)), this, SLOT(setAgcThreshold(int)));
    connect(uiDockRxOpt, SIGNAL(agcSlopeChanged(int)), this, SLOT(setAgcSlope(int)));
    connect(uiDockRxOpt, SIGNAL(agcGainChanged(int)), this, SLOT(setAgcGain(int)));
    connect(uiDockRxOpt, SIGNAL(agcDecayChanged(int)), this, SLOT(setAgcDecay(int)));
    connect(uiDockRxOpt, SIGNAL(noiseBlankerChanged(int,bool,float)), this, SLOT(setNoiseBlanker(int,bool,float)));
    connect(uiDockRxOpt, SIGNAL(sqlLevelChanged(double)), this, SLOT(setSqlLevel(double)));

    /* Connect GUI with the dockable widget for Audio options*/
    connect(uiDockAudio, SIGNAL(audioGainChanged(float)), this, SLOT(setAudioGain(float)));
    connect(uiDockAudio, SIGNAL(audioRecStarted(QString)), this, SLOT(startAudioRec(QString)));
    connect(uiDockAudio, SIGNAL(audioRecStopped()), this, SLOT(stopAudioRec()));
    connect(uiDockAudio, SIGNAL(audioPlayStarted(QString)), this, SLOT(startAudioPlayback(QString)));
    connect(uiDockAudio, SIGNAL(audioPlayStopped()), this, SLOT(stopAudioPlayback()));
    connect(uiDockAudio, SIGNAL(fftRateChanged(int)), this, SLOT(setAudioFftRate(int)));

    // Set min and max dB
    ui->plotter->setMaxDB(100);
    ui->plotter->setMinDB(-100);

    ui->plotter->setSampleRate(44100.0);
    ui->plotter->setSpanFreq(440.0);
    ui->plotter->setPercent2DScreen(30);
    ui->plotter->resetHorizontalZoom();
    ui->plotter->setCenterFreq(14500000);

    // FFT Initialization

    inputData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*16384);
    fft = new Fft(inputData);
    currentFFTsize = fft->size();
    outputData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*currentFFTsize);
    outputFFTdata = (double*) malloc(sizeof(double)*currentFFTsize);
    d_fftAvg = 0.5;
    switchSigns = false;
    windowType = -2; // Default is Hamming
    currentPoint = 0;
    stopPoint = 0;
    minValue = maxValue = 0.0;
    resetMinMax = true; // Reset the min and max
}

MultiPlot::~MultiPlot()
{
    //Memory deallocation:
    /* Note: 7-9-15, issues with ~MultiPlot...*/
//    std::cout<< "~MultiPlot()" << std::endl;
//    fft_timer->stop();
//    delete fft_timer;
//    meter_timer->stop();
//    delete meter_timer;
//    audio_fft_timer->stop();
//    delete audio_fft_timer;
//    free(outputFFTdata);
//    inputBuffer.clear();
//    fftw_free(inputData);
//    fftw_free(outputData);
//    delete uiDockFft;
//    delete uiDockRxOpt;
//    delete uiDockAudio;
//    delete ui;
//    delete fft;
}

/*
    Sets the running of the waterfall plot when the timer is active. Executes
    the FFT on the input data and sends the resulting output data and the original
    input data to the plotter.
*/
void MultiPlot::FftTimeout()
{
    // Pull current stream
    stopPoint += int(currentFFTsize);
    int i = 0;
    for(; currentPoint < stopPoint; currentPoint++)
    {
        if(currentPoint == inputBuffer.size()-1)
        {
            stopPoint = int(currentFFTsize - i);
            currentPoint = 0;
        }
        inputData[i][0] = inputBuffer[currentPoint];
        inputData[i][1] = 0.0;
        i++;
    }

    // Calculate Window Functions - Hanning, Hamming, or Blackman-Harris
    // Or no window function at all
    double windowFactor;
    double a0 = 0.35875, a1 = 0.48829, a2=0.14128, a3 = 0.01168;
    double alpha = 0.54, beta = 1- alpha;

    for(unsigned int i=0; i < currentFFTsize; i++)
    {

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


        inputData[i][0] *= windowFactor;
    }

    // FFT execution and input
    outputData = fft->exec();
    double minValue = 0.0, maxValue = 0.0;
    for(int i= 0; i < currentFFTsize; i++)
    {
        outputFFTdata[i] = outputFFTdata[i]/outputFFTdata[0];
        outputFFTdata[i] = sqrt((outputData[i][0]*outputData[i][0])
                +(outputData[i][1]*outputData[i][1]));

        outputFFTdata[i]= 10*log10(outputFFTdata[i]);

        if(outputFFTdata[i] > maxValue)
        {
            maxValue = outputFFTdata[i];
        }
        else if( outputFFTdata[i] < minValue)
        {
            minValue = outputFFTdata[i];
        }
    }

    //Output Data
    printf("\nOutput Buffer: \n");
    printf("Minimum value: %i\n", int(minValue));
    printf("Maximum value: %i\n", int(maxValue));
    printf("Last value: %3.3f\n", outputFFTdata[currentFFTsize/2 - 1]);
    // Set FFT running state to true.
    ui->plotter->setRunningState(true);
    ui->plotter->setNewFttData(outputFFTdata,int(fft->size()));
    if(resetMinMax)
    {
        ui->plotter->setMinDB(0.5*qint32(minValue));
        ui->plotter->setMaxDB(0.5*qint32(maxValue));
        resetMinMax = false;
    }
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
    printf("setFftSize() called! \n");
    if(value > 0)
    {
        if(value != currentFFTsize)
        {
            inputData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*value);
            outputData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*value);
            outputFFTdata = (double*) malloc(sizeof(double)*value);
        }
        fft->update(value);

        currentFFTsize = fft->size();
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
    avg=5; // anti-warning
    avg++;
    // Do nothing for now...
//    if ((avg >= 0) && (avg <= 1.0))
//        d_fftAvg = avg;
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
    double highestValue = 0.0,lowestValue = 0.0;
    inputBuffer.set_capacity(inputStream->getDecStreamArray()[numStream]->getBufSize());
    if(inputStream->isDone())
    {
        printf("InputStream is done.\n");
    }
    else
    {
        printf("InputStream is NOT done.\n");
    }
    uint64_t count = 1;
    while(count != 0)
    {
        count = 0;
        double* tempBuf = (double*) malloc(sizeof(double)*
                                           inputStream->getDecStreamArray()[numStream]->getBufSize());

        inputStream->getDecStreamArray()[numStream]->flushOutputStream(tempBuf, &count);
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
        delete[] tempBuf;

    }
    printf("Input Buffer: \n");
    printf("Highest value: %3.3f \n", highestValue);
    printf("Lowest value: %3.3f \n", lowestValue);
//    printf("Value at start point: %3.3f\n", inputBuffer[0]);
//    printf("Value at 1/2 point: %3.3f\n", inputBuffer[inputBufferSize/2]);
//    printf("Value at 3/4 point: %3.3f\n", inputBuffer[(3*inputBufferSize)/4]);
//    printf("Value at end point: %3.3f\n", inputBuffer[inputBufferSize-1]);
//    double x;
//    printf("Value of uninitialized Double: %3.3f\n",x);

    // How many actual elements are in circular buffer
    inputBufferSize = unsigned int(inputBuffer.size());
    printf("Input Buffer Size %i\n",inputBuffer.size());
//    ui->plotter->setMinDB(5*(lowestValue-1));
//    ui->plotter->setMaxDB(5*(highestValue+1));
}
/*
    Switches the timer on or off. This is called from the main window's
    startWFbutton. It either sets the waterfall plots to run or quit running.
 */
void MultiPlot::switchTimer()
{
    printf("Multiplot's switchTimer() is called.\n");
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

/*! \brief Set new channel filter offset.
 *  \param freq_hs The new filter offset in Hz.
 */
void MultiPlot::setFilterOffset(qint64 freq_hz)
{
    // TODO: Implement this after "receiver" class set up
    //rx->set_filter_offset((double) freq_hz);
    ui->plotter->setFilterOffset(freq_hz);
}

/*! \brief Select new demodulator.
 *  \param demod New demodulator index.
 *
 * This slot basically maps the index of the mode selector to receiver::demod
 * and configures the default channel filter.
 *
 */
void MultiPlot::selectDemod(int index)
{
    double quad_rate;
    float maxdev;
    int filter_preset = uiDockRxOpt->currentFilter();
    int flo=0, fhi=0, click_res=100;


    switch (index) {

    case DockRxOpt::MODE_OFF:
        /* Spectrum analyzer only */
        //rx->set_demod(receiver::RX_DEMOD_OFF);
        flo = 0;
        fhi = 0;
        click_res = 100;

        break;

    case DockRxOpt::MODE_RAW:
        /* Raw I/Q */
        qDebug() << "RAW I/Q mode not implemented!";
        break;

        /* AM */
    case DockRxOpt::MODE_AM:
        //rx->set_demod(receiver::RX_DEMOD_AM);
        ui->plotter->setDemodRanges(-20000, -250, 250, 20000, true);
        //uiDockAudio->setFftRange(0,15000);
        click_res = 100;
        switch (filter_preset)
        {
        case 0: //wide
            flo = -10000;
            fhi = 10000;
            break;
        case 2: // narrow
            flo = -2500;
            fhi = 2500;
            break;
        default: // normal
            flo = -5000;
            fhi = 5000;
            break;
        }
        break;

        /* Narrow FM */
    case DockRxOpt::MODE_NFM:
        //rx->set_demod(receiver::RX_DEMOD_NFM);
        click_res = 100;
        maxdev = uiDockRxOpt->currentMaxdev();
        if (maxdev < 20000.0)
        {   /** FIXME **/
            ui->plotter->setDemodRanges(-25000, -250, 250, 25000, true);
            //uiDockAudio->setFftRange(0,12000);
            switch (filter_preset) {
            case 0: //wide
                flo = -10000;
                fhi = 10000;
                break;
            case 2: // narrow
                flo = -2500;
                fhi = 2500;
                break;
            default: // normal
                flo = -5000;
                fhi = 5000;
                break;
            }
        }
        else
        {
            ui->plotter->setDemodRanges(-45000, -10000, 10000, 45000, true);
            //uiDockAudio->setFftRange(0,24000);
            switch (filter_preset) {
            /** FIXME: not sure about these **/
            case 0: //wide
                flo = -45000;
                fhi = 45000;
                break;
            case 2: // narrow
                flo = -10000;
                fhi = 10000;
                break;
            default: // normal
                flo = -35000;
                fhi = 35000;
                break;
            }
        }
        break;

        /* Broadcast FM */
    case DockRxOpt::MODE_WFM_MONO:
    case DockRxOpt::MODE_WFM_STEREO:
        //quad_rate = rx->get_input_rate();
        quad_rate = 100; // temp value
        if (quad_rate < 200.0e3)
            ui->plotter->setDemodRanges(-0.9*quad_rate/2.0, -10000,
                                        10000, 0.9*quad_rate/2.0,
                                        true);
        else
            ui->plotter->setDemodRanges(-250000, -10000, 10000, 250000, true);
        //uiDockAudio->setFftRange(0,24000);  /** FIXME: get audio rate from rx **/
        click_res = 1000;
        switch (filter_preset)
        {
        case 0: //wide
            flo = -100000;
            fhi = 100000;
            break;
        case 2: // narrow
            flo = -60000;
            fhi = 60000;
            break;
        default: // normal
            flo = -80000;
            fhi = 80000;
            break;
        }
        if (index == DockRxOpt::MODE_WFM_MONO)
            printf(""); //rx->set_demod(receiver::RX_DEMOD_WFM_M);
        else
            printf(""); //rx->set_demod(receiver::RX_DEMOD_WFM_S);

        break;

        /* LSB */
    case DockRxOpt::MODE_LSB:
        //rx->set_demod(receiver::RX_DEMOD_SSB);
        ui->plotter->setDemodRanges(-10000, -100, -5000, 0, false);
        //uiDockAudio->setFftRange(0,3500);
        click_res = 10;
        switch (filter_preset)
        {
        case 0: //wide
            flo = -4100;
            fhi = -100;
            break;
        case 2: // narrow
            flo = -1600;
            fhi = -200;
            break;
        default: // normal
            flo = -3000;
            fhi = -200;
            break;
        }
        break;

        /* USB */
    case DockRxOpt::MODE_USB:
        //rx->set_demod(receiver::RX_DEMOD_SSB);
        ui->plotter->setDemodRanges(0, 5000, 100, 10000, false);
        //uiDockAudio->setFftRange(0,3500);
        click_res = 10;
        switch (filter_preset)
        {
        case 0: //wide
            flo = 100;
            fhi = 4100;
            break;
        case 2: // narrow
            flo = 200;
            fhi = 1600;
            break;
        default: // normal
            flo = 200;
            fhi = 3000;
            break;
        }
        break;

        /* CW-L */
    case DockRxOpt::MODE_CWL:
        //rx->set_demod(receiver::RX_DEMOD_SSB);
        ui->plotter->setDemodRanges(-10000, -100, -5000, 0, false);
        //uiDockAudio->setFftRange(0,1500);
        click_res = 10;
        switch (filter_preset)
        {
        case 0: //wide
            flo = -2300;
            fhi = -200;
            break;
        case 2: // narrow
            flo = -900;
            fhi = -400;
            break;
        default: // normal
            flo = -1200;
            fhi = -200;
            break;
        }
        break;

        /* CW-U */
    case DockRxOpt::MODE_CWU:
        //rx->set_demod(receiver::RX_DEMOD_SSB);
        ui->plotter->setDemodRanges(0, 5000, 100, 10000, false);
        //uiDockAudio->setFftRange(0,1500);
        click_res = 10;
        switch (filter_preset)
        {
        case 0: //wide
            flo = 200;
            fhi = 2300;
            break;
        case 2: // narrow
            flo = 400;
            fhi = 900;
            break;
        default: // normal
            flo = 200;
            fhi = 1200;
            break;
        }
        break;

    default:
        qDebug() << "Unsupported mode selection: " << index;
        flo = -5000;
        fhi = 5000;
        click_res = 100;
        break;
    }

    qDebug() << "Filter preset for mode" << index << "LO:" << flo << "HI:" << fhi;
    ui->plotter->setHiLowCutFrequencies(flo, fhi);
    ui->plotter->setClickResolution(click_res);
    ui->plotter->setFilterClickResolution(click_res);
    // TODO: Implement after setting up "receiver" class
    //rx->set_filter((double)flo, (double)fhi, receiver::FILTER_SHAPE_NORMAL);

    // Necessary for this program?
    //d_have_audio = ((index != DockRxOpt::MODE_OFF) && (index != DockRxOpt::MODE_RAW));
}

/*! \brief New FM deviation selected.
 *  \param max_dev The enw FM deviation.
 */
void MultiPlot::setFmMaxdev(float max_dev)
{
    qDebug() << "FM MAX_DEV: " << max_dev;

    /* receiver will check range */
    //rx->set_fm_maxdev(max_dev);

    /* update filter */
    if (max_dev < 20000.0)
    {
        ui->plotter->setDemodRanges(-25000, -1000, 1000, 25000, true);
        ui->plotter->setHiLowCutFrequencies(-5000, 5000);
        //rx->set_filter(-5000.0, 5000.0, receiver::FILTER_SHAPE_NORMAL);
    }
    else
    {
        ui->plotter->setDemodRanges(-45000, -10000, 10000, 45000, true);
        ui->plotter->setHiLowCutFrequencies(-35000, 35000);
        //rx->set_filter(-35000.0, 35000.0, receiver::FILTER_SHAPE_NORMAL);
    }
}


/*! \brief New FM de-emphasis time consant selected.
 *  \param tau The new time constant
 */
void MultiPlot::setFmEmph(double tau)
{
    qDebug() << "FM TAU: " << tau;

        // DOES NOTHING YET --- NO "RECEIVER"
    /* receiver will check range */
    //rx->set_fm_deemph(tau);
}

/*! \brief AM DCR status changed (slot).
 *  \param enabled Whether DCR is enabled or not.
 */
void MultiPlot::setAmDcr(bool enabled)
{
    enabled = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_am_dcr(enabled);
}

/*! \brief Set AGC ON/OFF.
 *  \param agc_on Whether AGC is ON (true) or OFF (false).
 */
void MultiPlot::setAgcOn(bool agc_on)
{
    agc_on = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_agc_on(agc_on);
}

/*! \brief AGC hang ON/OFF.
 *  \param use_hang Whether to use hang.
 */
void MultiPlot::setAgcHang(bool use_hang)
{
    use_hang = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_agc_hang(use_hang);
}

/*! \brief AGC threshold changed.
 *  \param threshold The new threshold.
 */
void MultiPlot::setAgcThreshold(int threshold)
{
    threshold = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_agc_threshold(threshold);
}

/*! \brief AGC slope factor changed.
 *  \param factor The new slope factor.
 */
void MultiPlot::setAgcSlope(int factor)
{
    factor = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_agc_slope(factor);
}

/*! \brief AGC manual gain changed.
 *  \param gain The new manual gain in dB.
 */
void MultiPlot::setAgcGain(int gain)
{
    gain = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_agc_manual_gain(gain);
}

/*! \brief AGC decay changed.
 *  \param factor The new AGC decay.
 */
void MultiPlot::setAgcDecay(int msec)
{
    msec = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_agc_decay(msec);
}

/*! \brief Noide blanker configuration changed.
 *  \param nb1 Noise blanker 1 ON/OFF.
 *  \param nb2 Noise blanker 2 ON/OFF.
 *  \param threshold Noise blanker threshold.
 */
void MultiPlot::setNoiseBlanker(int nbid, bool on, float threshold)
{
    qDebug() << "Noise blanker NB:" << nbid << " ON:" << on << "THLD:" << threshold;

    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_nb_on(nbid, on);
    //rx->set_nb_threshold(nbid, threshold);
}


/*! \brief Squelch level changed.
 *  \param level_db The new squelch level in dBFS.
 */
void MultiPlot::setSqlLevel(double level_db)
{
    level_db = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_sql_level(level_db);
}

void MultiPlot::hideDockAudio()
{
    uiDockAudio->hide();
}

void MultiPlot::showDockAudio()
{
    addDockWidget(Qt::RightDockWidgetArea, uiDockAudio);
}

/*! \brief Audio gain changed.
 *  \param value The new audio gain in dB.
 */
void MultiPlot::setAudioGain(float value)
{
    value = NULL;
    // DOES NOTHING YET --- NO "RECEIVER"
    //rx->set_af_gain(value);
}

/*! \brief Start audio recorder.
 *  \param filename The file name into which audio should be recorded.
 */
void MultiPlot::startAudioRec(const QString filename)
{
    // DOES NOTHING YET --- NO "RECEIVER"
//    if (rx->start_audio_recording(filename.toStdString()))
//    {
//        ui->statusBar->showMessage(tr("Error starting audio recorder"));

//        /* reset state of record button */
//        uiDockAudio->setAudioRecButtonState(false);
//    }
//    else
//    {
//        ui->statusBar->showMessage(tr("Recording audio to %1").arg(filename));
//    }
}

/*! \brief Stop audio recorder. */
void MultiPlot::stopAudioRec()
{
       // DOES NOTHING YET --- NO "RECEIVER"
//    if (rx->stop_audio_recording())
//    {
//        /* okay, this one would be weird if it really happened */
//        ui->statusBar->showMessage(tr("Error stopping audio recorder"));

//        uiDockAudio->setAudioRecButtonState(true);
//    }
//    else
//    {
//        ui->statusBar->showMessage(tr("Audio recorder stopped"), 5000);
//    }
}

/*! \brief Start playback of audio file. */
void MultiPlot::startAudioPlayback(const QString filename)
{
           // DOES NOTHING YET --- NO "RECEIVER"
//    if (rx->start_audio_playback(filename.toStdString()))
//    {
//        ui->statusBar->showMessage(tr("Error trying to play %1").arg(filename));

//        /* reset state of record button */
//        uiDockAudio->setAudioPlayButtonState(false);
//    }
//    else
//    {
//        ui->statusBar->showMessage(tr("Playing %1").arg(filename));
//    }
}

/*! \brief Stop playback of audio file. */
void MultiPlot::stopAudioPlayback()
{
    // DOES NOTHING YET --- NO "RECEIVER"
//    if (rx->stop_audio_playback())
//    {
//        /* okay, this one would be weird if it really happened */
//        ui->statusBar->showMessage(tr("Error stopping audio playback"));

//        uiDockAudio->setAudioPlayButtonState(true);
//    }
//    else
//    {
//        ui->statusBar->showMessage(tr("Audio playback stopped"), 5000);
//    }
}

/*! \brief Audio FFT rate has changed. */
void MultiPlot::setAudioFftRate(int fps)
{
    int interval = 1000 / fps;

    if (interval < 10)
        return;

    if (audio_fft_timer->isActive())
        audio_fft_timer->setInterval(interval);
}

/*! \brief Signal strength meter timeout */
void MultiPlot::meterTimeout()
{
    float level;

    //level = rx->get_signal_pwr(true);
    level = 0.0;
    ui->sMeter->setLevel(level);
}

void MultiPlot::on_windowTypeSelect_currentIndexChanged(const QString & text)
{
    //Reset input buffer
    //setBuffer(inputBuffer, inputBufferSize);
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
