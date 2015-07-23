#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T15:24:11
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Tip from: http://www.qtcentre.org/wiki/index.php?title=Version_numbering_using_QMake
VERSTR = '\\"$${VER}\\"'          # place quotes around the version string
DEFINES += VERSION=\"$${VERSTR}\" # create a VERSION macro containing the version string


TARGET = QTwaterfallPlot
TEMPLATE = app

# make clean target
QMAKE_CLEAN += QTwaterfallPlot

SOURCES += qtgui/plotter.cpp \
    qtgui/dockfft.cpp \
    qtgui/qtcolorpicker.cpp \
    qtgui/freqctrl.cpp \
    qtgui/meter.cpp \
    app/main.cpp \
    app/mainwindow.cpp \
    app/multiplot.cpp

HEADERS  += qtgui/plotter.h \
    qtgui/dockfft.h \
    qtgui/qtcolorpicker.h \
    qtgui/freqctrl.h \
    qtgui/meter.h \
    app/fft.h \
    app/mainwindow.h \
    app/multiplot.h

FORMS    += qtgui/dockfft.ui \
    app/mainwindow.ui \
    app/multiplot.ui


# Libs and include path for FFTW
win32: LIBS += -L$$PWD/fftw/ -llibfftw3-3
INCLUDEPATH += $$PWD/fftw/

DISTFILES += \
    icons/audio-card.svg \
    icons/bookmark-new.svg \
    icons/clear.svg \
    icons/clock.svg \
    icons/close.svg \
    icons/document.svg \
    icons/flash.svg \
    icons/floppy.svg \
    icons/folder.svg \
    icons/fullscreen.svg \
    icons/gqrx.svg \
    icons/help.svg \
    icons/info.svg \
    icons/play.svg \
    icons/power-off.svg \
    icons/radio.svg \
    icons/record.svg \
    icons/refresh.svg \
    icons/settings.svg \
    icons/signal.svg \
    icons/tangeo-network-idle.svg \
    icons/terminal.svg \
    icons/gqrx.icns \
    jetColorMap.txt

RESOURCES += \
    icons.qrc \
    jet.qrc

unix|win32: LIBS += -L$$PWD/GnssLib/ -lGnssReader

INCLUDEPATH += $$PWD/GnssLib/include
DEPENDPATH += $$PWD/GnssLib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GnssLib/GnssReader.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/GnssLib/include/libGnssReader.a
