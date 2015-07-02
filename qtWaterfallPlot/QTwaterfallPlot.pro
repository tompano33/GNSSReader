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

SOURCES += main.cpp\
        mainwindow.cpp \
    qtgui/plotter.cpp \
    qtgui/dockfft.cpp \
    qtgui/qtcolorpicker.cpp \
    app/TestApp/ChunkBuffer.cpp \
    app/TestApp/DecStream.cpp \
    app/TestApp/StreamAnalytics.cpp \
    app/TestApp/XMLtoMeta.cpp \
    app/TestApp/FileReader.cpp \
    app/TestApp/IBuffer.cpp \
    multiplot.cpp \
    qtgui/freqctrl.cpp \
    qtgui/meter.cpp \
    qtgui/dockrxopt.cpp \
    qtgui/agc_options.cpp \
    qtgui/demod_options.cpp \
    qtgui/nb_options.cpp \
    qtgui/dockaudio.cpp \
    app/TestApp/GnssReader.cpp

HEADERS  += mainwindow.h \
    qtgui/plotter.h \
    qtgui/dockfft.h \
    qtgui/qtcolorpicker.h \
    fft.h \
    app/TestApp/ChunkBuffer.h \
    app/TestApp/DecStream.h \
    app/TestApp/StreamAnalytics.h \
    app/TestApp/XMLtoMeta.h \
    app/TestApp/FileReader.h \
    app/TestApp/IBuffer.h \
    multiplot.h \
    qtgui/freqctrl.h \
    qtgui/meter.h \
    qtgui/dockrxopt.h \
    qtgui/agc_options.h \
    qtgui/demod_options.h \
    qtgui/nb_options.h \
    qtgui/dockaudio.h \
    app/TestApp/GnssReader.h

FORMS    += mainwindow.ui \
    qtgui/dockfft.ui \
    multiplot.ui \
    qtgui/dockrxopt.ui \
    qtgui/agc_options.ui \
    qtgui/demod_options.ui \
    qtgui/nb_options.ui \
    qtgui/dockaudio.ui



win32: LIBS += -L$$PWD/fftw/ -llibfftw3-3

#-------------------
# Switch to check for debug or release:
#-------------------
win32: LIBS += $$PWD/lib/x64/Debug/tinyxml2.lib
win32: LIBS += $$PWD/lib/x64/Debug/GnssMetadata.lib

INCLUDEPATH += $$PWD/fftw
DEPENDPATH += $$PWD/fftw

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

INCLUDEPATH += $$PWD/lib/tinyxml2/
DEPENDPATH += $$PWD/lib/tinyxml2/

INCLUDEPATH += $$PWD/lib/GnssMetadata/
DEPENDPATH += $$PWD/lib/GnssMetadata/

INCLUDEPATH += $$PWD/app/
DEPENDPATH += $$PWD/app/

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
    icons.qrc
