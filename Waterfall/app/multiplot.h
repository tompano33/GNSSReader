#ifndef MULTIPLOT_H
#define MULTIPLOT_H

#include <QMainWindow>
#include <QTimer>
#include "fft.h"
#include "qtgui/dockfft.h"
#include "qtgui/dockrxopt.h"
#include "qtgui/dockaudio.h"
#include <complex>
#include "GnssLib/include/GnssReader.h"

namespace Ui {
class MultiPlot;
}

class MultiPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit MultiPlot(QWidget *parent = 0);
    ~MultiPlot();
    void setBuffer(GNSSReader* input, size_t numStream);
    void switchTimer();
    void hideDockAudio();
    void showDockAudio();

signals:
    void fftRateChanged(int fps);   /*! \brief FFT rate changed. */
    void fftSizeChanged(int value);

public slots:

    /* FFT Settings*/
    void setFftRate(int fps);
    void setFftSize(int value);
    void setFftColor(const QColor color);
    void setFftFill(bool enable);
    void setPeakDetection(bool enabled);
    void setFftPeakHold(bool enable);
    void setIqFftSplit(int pct_wf);
    void setIqFftAvg(float avg);
    void setNewFrequency(qint64);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void FftTimeout();
    void on_plotter_newCenterFreq(qint64 f);
    void meterTimeout();
    void on_windowTypeSelect_currentIndexChanged(const QString & text);
    void on_numBlocksSelect_currentIndexChanged(const QString & text);

private:
    bool loadStream();
    void performFFT();
    void displayFFT();
    void iqSquared();

    Ui::MultiPlot *ui;
    Fft* fft;
    QTimer *fft_timer, *audio_fft_timer, *meter_timer;
    size_t defaultFFTsize, currentFFTsize;

    /* Dock Widgets */
    fftw_complex  *outputData;
    double *avgFFT, *wfData;

    std::vector<double> inputBuffer;
    double *inputData, *outputFFTdata, *inputBufferData;
    unsigned int inputBufferSize;
    int minValue, maxValue;
    DockFft *uiDockFft;

    /* Pointer to GNSSReader*/
    GNSSReader *inputStream;
    size_t streamNumber;  // The number of the stream to pull

    double d_fftAvg;      /*!< FFT averaging parameter set by user (not the true gain). */
    int windowType;
    int currentPoint, stopPoint;
    bool resetMinMax, streamFileLoaded;

    size_t numBlocks, fftSelRange;
    size_t k;  // k is the current FFT
};

#endif // MULTIPLOT_H
