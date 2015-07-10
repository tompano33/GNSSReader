#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QFileDialog>
#include <QMdiArea>
#include "qtgui/dockfft.h"
#include "multiplot.h"
#include <time.h>
#include <QSignalMapper>
#include "fftw/fftw3.h"
#include "fft.h"
#include "app/TestApp/GNSSreader.h"
#include "app/TestApp/DecStream.h"

class QSignalMapper;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    size_t getNumStream();

private slots:
    /* Menu and toolbar actions */
    void wfButtonToggled();
    void on_actionAbout_triggered();

    /* Open/load File */
    bool open();
    void load(const char*);

private:

   // bool openFile(const QString &fileName);
    bool wfRunning, threading; // State condition of waterfall plot(s) running

    Ui::MainWindow *ui;

    /* Multiple Windows */
    std::vector<MultiPlot* > multiPlots;
    QMdiArea* workspace;
    QSignalMapper *windowMapper;
    QString fileFilters;

    // Input /File reading
    GNSSReader *test;

    // FFT
    //Fft fft;
    size_t oldFFTsize;
    double **bufferThread, *tempBuffer;
    size_t buffer1size, streamBufferSize;
    uint64_t numStreams;

};

#endif // MAINWINDOW_H
