#include "Windows.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <complex.h>

#include <QThread>
#include "math.h"
#include <QMutex>
#include <QFile>
#include <QMessageBox>

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <memory>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // File filters for loading dialog.
    fileFilters = tr("XML files (*.xml)\n"
                     "SDRX files (*.sdrx)\n"
                     "All files (*)");

    /* Set up the  user interface, multiple document interface (MDI) area
       Set the central widget as the MDI area, initiate QSignalMapper
       and connect to the main window. */

    ui->setupUi(this);
    workspace = new QMdiArea;
    setCentralWidget(workspace);


    /* Waterfall plot window variables. */
    numStreams = 0;
    wfRunning = false;

    /* Set up the toolbar. */
    QToolBar *toolbar = addToolBar(tr("Waterfall"));
    ui->blankLabel->setText("\t");
    toolbar->addWidget(ui->startWFbutton);

    /* Connect push button*/
    connect(ui->startWFbutton,SIGNAL(clicked()),this,SLOT(wfButtonToggled()));

    /* Connect the open menu action*/
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    // Default file:
    load("C:\\Users\\ANTuser\\Documents\\Tests\\singleStream\\test.xml");

    // Maximize window
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    // Memory deallocation:
    multiPlots.clear();
    delete ui;
    delete fileReader;
}

/*
    The startWFbutton initiates the waterfall plot(s) in each
    of the respective windows. It first checks to ensure there
    are windows to draw plots in and they equal the number of
    streams sent.

    If the waterfall plot is not running, the button will say "Start
    Waterfall" and clicking it will start the plotting. Once the plots
    are running, the button will say "Stop Waterfall" and clicking it
    will cease all plotting.

*/
void MainWindow::wfButtonToggled()
{
    int subWindowCount = workspace->subWindowList().count();
    if(subWindowCount != 0 && subWindowCount==numStreams)
    {
        if(!wfRunning)
        {
            wfRunning = true;
            for(size_t i=0; i < numStreams; i++)
            {
                multiPlots.at(i)->switchTimer();
            }

            ui->startWFbutton->setText("Stop Waterfall");
            ui->startWFbutton->setShortcut(QString("Shift+W"));
        }
        else
        {
            wfRunning = false;
            for(size_t i=0; i < numStreams; i++)
            {
                multiPlots.at(i)->switchTimer();
            }

            ui->startWFbutton->setText("Start Waterfall");
            ui->startWFbutton->setShortcut(QString("Shift+W"));
        }
    }
    else
    { // No windows open.
        wfRunning = false;
        ui->startWFbutton->setText("Start Waterfall");
        ui->startWFbutton->setShortcut(QString("Shift+W"));
    }
}

/*
    The open() function will first stop all waterfall plots to avoid
    potential issues with the program trying to draw plots to closed
    windows. After this, the function opens a file system browser dialog
    to find XML files to load. Once an XML file is selected and the open
    button is pressed, the file name will be sent to the load() function.

    After the file is loaded, the open() function returns true. If the file
    does not load, "file load failed" prints to the console.

*/
bool MainWindow::open()
{
    int subWindowCount = workspace->subWindowList().count();
    // Reset Waterfall button
    if(wfRunning)
    {
        wfRunning = false;
        for(size_t i=0; i < numStreams; i++)
        {
            if(!multiPlots.empty() && (subWindowCount != 0))
                multiPlots.at(i)->switchTimer();
        }

        ui->startWFbutton->setText("Start Waterfall");
        ui->startWFbutton->setShortcut(QString("Shift+W"));
    }

    // Gives the full path to the file.
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open"),
                                         QDir::currentPath(),
                                         fileFilters,
                                         0,
                                         QFileDialog::DontResolveSymlinks);
    if (fileName.isEmpty())
    {
        printf("File load failed.");
        return false;
    }
    // Switch "/" to "\\" for passing to GNSSReader constructor
    fileName.replace("/","\\");

    load(fileName.toStdString().c_str());
    return true;
}

/*
    Loads the file passed as an argument. First, the
    vector is cleared so new plots can be opened. TODO: This
    is so new scale or zoom levels can be loaded for
    different waveforms. The workspace is cleared of all windows.

    A GNSSReader object is instantiated to run the file loading operations
    that use an XML file to determine how to read the accompanying .DAT file.
    The object's makeDecStreams() and start() functions are called. If successful,
    numStreams is set to the number of streams loaded. For each of these streams,
    a subwindow is created in the MDI area, or workspace. These each have their own
    plotter for graphing and FFT dockable widget for controls. A subwindow contains
    one stream of data from the loaded file.
*/
void MainWindow::load(const char* file)
{
    /*      Start of input file reading     */
    try
    {
        multiPlots.clear();
        workspace->closeAllSubWindows();
        //GNSSReader test(file, 50000L,100000L,10000000L);
        fileReader = new GNSSReader (file,50L,1000L,100000L);
        fileReader->makeDecStreams();
        fileReader->startAsThread();
        numStreams = fileReader->getDecStreamCount();
        for(int i=0; i < numStreams; i++)
        {
            MultiPlot* mplot = new MultiPlot;
            mplot->setBuffer(fileReader, i);
            QString winTitle;
            winTitle.sprintf("Stream #%i", i+1);
            mplot->setWindowTitle(winTitle);
            multiPlots.push_back(mplot);
            workspace->addSubWindow(mplot);
            mplot->show();
            if(numStreams >1)
            {
                mplot->setIqFftSplit(15);
            }
        }
        numStreams = fileReader->getDecStreamCount();
    } catch (std::exception& e) {
        printf(e.what());
    }

    workspace->tileSubWindows();
    /*      End of input file reading     */
}

/*! \brief Action: About Gqrx
 *
 * This slot is called when the user activates the
 * Help|About menu item (or Gqrx|About on Mac)
 */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Gqrx"),
                       tr("<p>This program uses source code from Gqrx %1</p>"
                          "<p>Copyright (C) 2011-2014 Alexandru Csete & contributors.</p>"
                          "<p>Gqrx is a software defined radio receiver powered by "
                          "<a href='http://www.gnuradio.org/'>GNU Radio</a> and the Qt toolkit. "
                          "<p>Gqrx uses the <a href='http://sdr.osmocom.org/trac/wiki/GrOsmoSDR'>GrOsmoSDR</a> "
                          "input source block and and works with any input device supported by it including:"
                          "<ul>"
                          "<li><a href='http://funcubedongle.com/'>Funcube Dongle Pro and Pro+</a></li>"
                          "<li><a href='http://sdr.osmocom.org/trac/wiki/rtl-sdr'>RTL2832U-based DVB-T tuners (rtlsdr and rtlsdr-tcp)</a></li>"
                          "<li><a href='http://www.ettus.com/'>Ettus Research USRP devices</a></li>"
                          "<li><a href='http://sdr.osmocom.org/trac/'>OsmoSDR</a></li>"
                          "<li><a href='https://greatscottgadgets.com/hackrf/'>HackRF One & Jawbreaker</a></li>"
                          "<li><a href='http://nuand.com/bladeRF'>Nuand bladeRF</a></li>"
                          "<li><a href='http://airspy.com'>Airspy</a></li>"
                          "<li><a href='http://rfspace.com'>RFspace receivers</a></li>"
                          "</ul></p>"
                          "<p>You can download the latest version from the "
                          "<a href='http://gqrx.dk/'>Gqrx website</a>."
                          "</p>"
                          "<p>"
                          "Gqrx is licensed under the <a href='http://www.gnu.org/licenses/gpl.html'>GNU General Public License</a>."
                          "</p>").arg(VERSION));
}

