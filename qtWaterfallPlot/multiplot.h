#ifndef MULTIPLOT_H
#define MULTIPLOT_H

#include <QMainWindow>
#include <QTimer>
#include "fft.h"
#include "qtgui/dockfft.h"
#include "qtgui/dockrxopt.h"
#include "qtgui/dockaudio.h"

namespace Ui {
class MultiPlot;
}

class MultiPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit MultiPlot(QWidget *parent = 0);
    ~MultiPlot();
    void setBuffer(double*, size_t);
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
    void setFilterOffset(qint64);
    void setFmEmph(double tau);
    void setFmMaxdev(float max_dev);
    void selectDemod(int index);
    void setAmDcr(bool enabled);
    void setAgcOn(bool agc_on);
    void setAgcHang(bool use_hang);
    void setAgcThreshold(int threshold);
    void setAgcSlope(int factor);
    void setAgcGain(int gain);
    void setAgcDecay(int msec);
    void setNoiseBlanker(int nbid, bool on, float threshold);
    void setSqlLevel(double level_db);
    void setAudioGain(float value);
    void startAudioRec(const QString filename);
    void stopAudioRec();
    void startAudioPlayback(const QString filename);
    void stopAudioPlayback();
    void setAudioFftRate(int fps);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void FftTimeout();
    void on_plotter_newCenterFreq(qint64 f);

private:

    Ui::MultiPlot *ui;
    Fft* fft;
    QTimer *fft_timer, *audio_fft_timer;
    size_t oldFFTsize, currentFFTsize;

    /* Dock Widgets */
    fftw_complex *inputData, *outputData;
    double *inputBuffer;
    int minValue, maxValue;
    DockFft *uiDockFft;
    DockRxOpt *uiDockRxOpt;
    DockAudio *uiDockAudio;
};

#endif // MULTIPLOT_H
