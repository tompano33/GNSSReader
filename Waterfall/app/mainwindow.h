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

private slots:
    /* Menu and toolbar actions */
    void wfButtonToggled();
    void on_actionAbout_triggered();

    /* Open/load File */
    bool open();
    void load(const char*);

private:

    bool wfRunning; // State condition of waterfall plot(s) running

    Ui::MainWindow *ui;

    /* Multiple Windows */
    std::vector<MultiPlot* > multiPlots;
    QMdiArea* workspace;
    QString fileFilters;

    // Input /File reading
    GNSSReader *fileReader;
    uint64_t numStreams;
};

#endif // MAINWINDOW_H
