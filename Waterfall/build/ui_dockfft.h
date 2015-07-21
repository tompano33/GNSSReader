/********************************************************************************
** Form generated from reading UI file 'dockfft.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCKFFT_H
#define UI_DOCKFFT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qtgui/qtcolorpicker.h"

QT_BEGIN_NAMESPACE

class Ui_DockFft
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *resetButton;
    QLabel *zoomLevelLabel;
    QLabel *zoomLAbel;
    QComboBox *fftRateComboBox;
    QPushButton *centerButton;
    QPushButton *peakHoldButton;
    QtColorPicker *colorPicker;
    QSlider *fftZoomSlider;
    QPushButton *fillButton;
    QLabel *colorLabel;
    QPushButton *demodButton;
    QSlider *fftSplitSlider;
    QLabel *pandLabel;
    QLabel *peakLabel;
    QLabel *fftAvgLabel;
    QSlider *fftAvgSlider;
    QComboBox *fftSizeComboBox;
    QLabel *fftRateLabel;
    QPushButton *peakDetectionButton;
    QLabel *fftSizeLabel;
    QLabel *wfLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QDockWidget *DockFft)
    {
        if (DockFft->objectName().isEmpty())
            DockFft->setObjectName(QStringLiteral("DockFft"));
        DockFft->resize(348, 300);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DockFft->sizePolicy().hasHeightForWidth());
        DockFft->setSizePolicy(sizePolicy);
        DockFft->setMinimumSize(QSize(250, 300));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/signal.svg"), QSize(), QIcon::Normal, QIcon::Off);
        DockFft->setWindowIcon(icon);
        DockFft->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(5);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        resetButton = new QPushButton(dockWidgetContents);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(resetButton->sizePolicy().hasHeightForWidth());
        resetButton->setSizePolicy(sizePolicy1);
        resetButton->setMinimumSize(QSize(28, 28));
        resetButton->setMaximumSize(QSize(32, 32));

        gridLayout->addWidget(resetButton, 6, 1, 1, 1);

        zoomLevelLabel = new QLabel(dockWidgetContents);
        zoomLevelLabel->setObjectName(QStringLiteral("zoomLevelLabel"));
        sizePolicy1.setHeightForWidth(zoomLevelLabel->sizePolicy().hasHeightForWidth());
        zoomLevelLabel->setSizePolicy(sizePolicy1);
        zoomLevelLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(zoomLevelLabel, 5, 4, 1, 1);

        zoomLAbel = new QLabel(dockWidgetContents);
        zoomLAbel->setObjectName(QStringLiteral("zoomLAbel"));
        zoomLAbel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(zoomLAbel, 5, 0, 1, 1);

        fftRateComboBox = new QComboBox(dockWidgetContents);
        fftRateComboBox->setObjectName(QStringLiteral("fftRateComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(fftRateComboBox->sizePolicy().hasHeightForWidth());
        fftRateComboBox->setSizePolicy(sizePolicy2);
        fftRateComboBox->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(fftRateComboBox, 1, 1, 1, 3);

        centerButton = new QPushButton(dockWidgetContents);
        centerButton->setObjectName(QStringLiteral("centerButton"));
        sizePolicy1.setHeightForWidth(centerButton->sizePolicy().hasHeightForWidth());
        centerButton->setSizePolicy(sizePolicy1);
        centerButton->setMinimumSize(QSize(28, 28));
        centerButton->setMaximumSize(QSize(32, 32));

        gridLayout->addWidget(centerButton, 6, 2, 1, 1);

        peakHoldButton = new QPushButton(dockWidgetContents);
        peakHoldButton->setObjectName(QStringLiteral("peakHoldButton"));
        sizePolicy1.setHeightForWidth(peakHoldButton->sizePolicy().hasHeightForWidth());
        peakHoldButton->setSizePolicy(sizePolicy1);
        peakHoldButton->setMinimumSize(QSize(28, 28));
        peakHoldButton->setMaximumSize(QSize(32, 32));
        peakHoldButton->setCheckable(true);

        gridLayout->addWidget(peakHoldButton, 4, 2, 1, 1);

        colorPicker = new QtColorPicker(dockWidgetContents);
        colorPicker->setObjectName(QStringLiteral("colorPicker"));
        sizePolicy1.setHeightForWidth(colorPicker->sizePolicy().hasHeightForWidth());
        colorPicker->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(colorPicker, 8, 1, 1, 3);

        fftZoomSlider = new QSlider(dockWidgetContents);
        fftZoomSlider->setObjectName(QStringLiteral("fftZoomSlider"));
        fftZoomSlider->setMinimum(1);
        fftZoomSlider->setMaximum(100);
        fftZoomSlider->setPageStep(5);
        fftZoomSlider->setValue(1);
        fftZoomSlider->setOrientation(Qt::Horizontal);
        fftZoomSlider->setInvertedAppearance(false);
        fftZoomSlider->setInvertedControls(false);
        fftZoomSlider->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(fftZoomSlider, 5, 1, 1, 3);

        fillButton = new QPushButton(dockWidgetContents);
        fillButton->setObjectName(QStringLiteral("fillButton"));
        sizePolicy1.setHeightForWidth(fillButton->sizePolicy().hasHeightForWidth());
        fillButton->setSizePolicy(sizePolicy1);
        fillButton->setMinimumSize(QSize(28, 28));
        fillButton->setMaximumSize(QSize(32, 32));
        fillButton->setCheckable(true);

        gridLayout->addWidget(fillButton, 8, 4, 1, 1);

        colorLabel = new QLabel(dockWidgetContents);
        colorLabel->setObjectName(QStringLiteral("colorLabel"));
        colorLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(colorLabel, 8, 0, 1, 1);

        demodButton = new QPushButton(dockWidgetContents);
        demodButton->setObjectName(QStringLiteral("demodButton"));
        sizePolicy1.setHeightForWidth(demodButton->sizePolicy().hasHeightForWidth());
        demodButton->setSizePolicy(sizePolicy1);
        demodButton->setMinimumSize(QSize(28, 28));
        demodButton->setMaximumSize(QSize(32, 32));

        gridLayout->addWidget(demodButton, 6, 3, 1, 1);

        fftSplitSlider = new QSlider(dockWidgetContents);
        fftSplitSlider->setObjectName(QStringLiteral("fftSplitSlider"));
        sizePolicy2.setHeightForWidth(fftSplitSlider->sizePolicy().hasHeightForWidth());
        fftSplitSlider->setSizePolicy(sizePolicy2);
        fftSplitSlider->setMinimumSize(QSize(0, 22));
        fftSplitSlider->setMinimum(0);
        fftSplitSlider->setMaximum(100);
        fftSplitSlider->setValue(50);
        fftSplitSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(fftSplitSlider, 3, 1, 1, 3);

        pandLabel = new QLabel(dockWidgetContents);
        pandLabel->setObjectName(QStringLiteral("pandLabel"));
        sizePolicy1.setHeightForWidth(pandLabel->sizePolicy().hasHeightForWidth());
        pandLabel->setSizePolicy(sizePolicy1);
        pandLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(pandLabel, 3, 0, 1, 1);

        peakLabel = new QLabel(dockWidgetContents);
        peakLabel->setObjectName(QStringLiteral("peakLabel"));
        peakLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(peakLabel, 4, 0, 1, 1);

        fftAvgLabel = new QLabel(dockWidgetContents);
        fftAvgLabel->setObjectName(QStringLiteral("fftAvgLabel"));
        sizePolicy1.setHeightForWidth(fftAvgLabel->sizePolicy().hasHeightForWidth());
        fftAvgLabel->setSizePolicy(sizePolicy1);
        fftAvgLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(fftAvgLabel, 2, 0, 1, 1);

        fftAvgSlider = new QSlider(dockWidgetContents);
        fftAvgSlider->setObjectName(QStringLiteral("fftAvgSlider"));
        sizePolicy2.setHeightForWidth(fftAvgSlider->sizePolicy().hasHeightForWidth());
        fftAvgSlider->setSizePolicy(sizePolicy2);
        fftAvgSlider->setMinimumSize(QSize(0, 22));
        fftAvgSlider->setMaximum(100);
        fftAvgSlider->setValue(50);
        fftAvgSlider->setOrientation(Qt::Horizontal);
        fftAvgSlider->setInvertedAppearance(false);
        fftAvgSlider->setInvertedControls(false);

        gridLayout->addWidget(fftAvgSlider, 2, 1, 1, 3);

        fftSizeComboBox = new QComboBox(dockWidgetContents);
        fftSizeComboBox->setObjectName(QStringLiteral("fftSizeComboBox"));
        fftSizeComboBox->setEnabled(true);
        sizePolicy2.setHeightForWidth(fftSizeComboBox->sizePolicy().hasHeightForWidth());
        fftSizeComboBox->setSizePolicy(sizePolicy2);
        fftSizeComboBox->setMinimumSize(QSize(0, 0));
        fftSizeComboBox->setEditable(false);
        fftSizeComboBox->setMaxVisibleItems(15);
        fftSizeComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);

        gridLayout->addWidget(fftSizeComboBox, 0, 1, 1, 3);

        fftRateLabel = new QLabel(dockWidgetContents);
        fftRateLabel->setObjectName(QStringLiteral("fftRateLabel"));
        fftRateLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(fftRateLabel, 1, 0, 1, 1);

        peakDetectionButton = new QPushButton(dockWidgetContents);
        peakDetectionButton->setObjectName(QStringLiteral("peakDetectionButton"));
        sizePolicy1.setHeightForWidth(peakDetectionButton->sizePolicy().hasHeightForWidth());
        peakDetectionButton->setSizePolicy(sizePolicy1);
        peakDetectionButton->setMinimumSize(QSize(28, 28));
        peakDetectionButton->setMaximumSize(QSize(32, 32));
        peakDetectionButton->setCheckable(true);

        gridLayout->addWidget(peakDetectionButton, 4, 1, 1, 1);

        fftSizeLabel = new QLabel(dockWidgetContents);
        fftSizeLabel->setObjectName(QStringLiteral("fftSizeLabel"));
        fftSizeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(fftSizeLabel, 0, 0, 1, 1);

        wfLabel = new QLabel(dockWidgetContents);
        wfLabel->setObjectName(QStringLiteral("wfLabel"));
        sizePolicy1.setHeightForWidth(wfLabel->sizePolicy().hasHeightForWidth());
        wfLabel->setSizePolicy(sizePolicy1);
        wfLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(wfLabel, 3, 4, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        DockFft->setWidget(dockWidgetContents);

        retranslateUi(DockFft);

        fftRateComboBox->setCurrentIndex(3);
        fftSizeComboBox->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(DockFft);
    } // setupUi

    void retranslateUi(QDockWidget *DockFft)
    {
#ifndef QT_NO_TOOLTIP
        DockFft->setToolTip(QApplication::translate("DockFft", "FFT performance and display settings", 0));
#endif // QT_NO_TOOLTIP
        DockFft->setWindowTitle(QApplication::translate("DockFft", "FFT Settings", 0));
#ifndef QT_NO_TOOLTIP
        resetButton->setToolTip(QApplication::translate("DockFft", "Reset zoom level to 1x", 0));
#endif // QT_NO_TOOLTIP
        resetButton->setText(QApplication::translate("DockFft", "R", 0));
#ifndef QT_NO_TOOLTIP
        zoomLevelLabel->setToolTip(QApplication::translate("DockFft", "Current zoom level", 0));
#endif // QT_NO_TOOLTIP
        zoomLevelLabel->setText(QApplication::translate("DockFft", "1x", 0));
#ifndef QT_NO_TOOLTIP
        zoomLAbel->setToolTip(QApplication::translate("DockFft", "Zooming shortcuts", 0));
#endif // QT_NO_TOOLTIP
        zoomLAbel->setText(QApplication::translate("DockFft", "Zoom", 0));
        fftRateComboBox->clear();
        fftRateComboBox->insertItems(0, QStringList()
         << QApplication::translate("DockFft", "60 fps", 0)
         << QApplication::translate("DockFft", "50 fps", 0)
         << QApplication::translate("DockFft", "30 fps", 0)
         << QApplication::translate("DockFft", "25 fps", 0)
         << QApplication::translate("DockFft", "20 fps", 0)
         << QApplication::translate("DockFft", "17 fps", 0)
         << QApplication::translate("DockFft", "15 fps", 0)
         << QApplication::translate("DockFft", "10 fps", 0)
         << QApplication::translate("DockFft", "5 fps", 0)
         << QApplication::translate("DockFft", "1 fps", 0)
         << QApplication::translate("DockFft", "0 fps", 0)
        );
#ifndef QT_NO_TOOLTIP
        fftRateComboBox->setToolTip(QApplication::translate("DockFft", "FFT refresh rate", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        centerButton->setToolTip(QApplication::translate("DockFft", "Center FFT around original center frequency", 0));
#endif // QT_NO_TOOLTIP
        centerButton->setText(QApplication::translate("DockFft", "C", 0));
#ifndef QT_NO_TOOLTIP
        peakHoldButton->setToolTip(QApplication::translate("DockFft", "Toggle peak hold in FFT", 0));
#endif // QT_NO_TOOLTIP
        peakHoldButton->setText(QApplication::translate("DockFft", "H", 0));
#ifndef QT_NO_TOOLTIP
        colorPicker->setToolTip(QApplication::translate("DockFft", "Color for the FFT plot", 0));
#endif // QT_NO_TOOLTIP
        colorPicker->setText(QString());
#ifndef QT_NO_TOOLTIP
        fftZoomSlider->setToolTip(QApplication::translate("DockFft", "Zoom level for zooming in on the frequency axis.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        fillButton->setToolTip(QApplication::translate("DockFft", "Fill the area below the FFT plot with a gradient", 0));
#endif // QT_NO_TOOLTIP
        fillButton->setText(QApplication::translate("DockFft", "F", 0));
#ifndef QT_NO_TOOLTIP
        colorLabel->setToolTip(QApplication::translate("DockFft", "Color for the FFT plot", 0));
#endif // QT_NO_TOOLTIP
        colorLabel->setText(QApplication::translate("DockFft", "Color", 0));
#ifndef QT_NO_TOOLTIP
        demodButton->setToolTip(QApplication::translate("DockFft", "Center FFT around demodulator frequency", 0));
#endif // QT_NO_TOOLTIP
        demodButton->setText(QApplication::translate("DockFft", "D", 0));
#ifndef QT_NO_TOOLTIP
        fftSplitSlider->setToolTip(QApplication::translate("DockFft", "Spatial distribution between pandapter and waterfall", 0));
#endif // QT_NO_TOOLTIP
        pandLabel->setText(QApplication::translate("DockFft", "Pandapter", 0));
        peakLabel->setText(QApplication::translate("DockFft", "Peak", 0));
#ifndef QT_NO_TOOLTIP
        fftAvgLabel->setToolTip(QApplication::translate("DockFft", "FFT averaging gain", 0));
#endif // QT_NO_TOOLTIP
        fftAvgLabel->setText(QApplication::translate("DockFft", "Averaging", 0));
#ifndef QT_NO_TOOLTIP
        fftAvgSlider->setToolTip(QApplication::translate("DockFft", "FFT averaging gain", 0));
#endif // QT_NO_TOOLTIP
        fftSizeComboBox->clear();
        fftSizeComboBox->insertItems(0, QStringList()
         << QApplication::translate("DockFft", "65536", 0)
         << QApplication::translate("DockFft", "32768", 0)
         << QApplication::translate("DockFft", "16384", 0)
         << QApplication::translate("DockFft", "8192", 0)
         << QApplication::translate("DockFft", "4096", 0)
         << QApplication::translate("DockFft", "3840", 0)
         << QApplication::translate("DockFft", "2048", 0)
         << QApplication::translate("DockFft", "1024", 0)
         << QApplication::translate("DockFft", "768", 0)
         << QApplication::translate("DockFft", "512", 0)
        );
#ifndef QT_NO_TOOLTIP
        fftSizeComboBox->setToolTip(QApplication::translate("DockFft", "<html>Number of FFT points to calculate. Higher values will require more CPU time. This will not influence the number of points on the display since that parameter is adjusted automatically according to the display size.\n"
"</html>", 0));
#endif // QT_NO_TOOLTIP
        fftSizeComboBox->setCurrentText(QApplication::translate("DockFft", "16384", 0));
        fftRateLabel->setText(QApplication::translate("DockFft", "Rate", 0));
#ifndef QT_NO_TOOLTIP
        peakDetectionButton->setToolTip(QApplication::translate("DockFft", "Enable peak detection in FFT", 0));
#endif // QT_NO_TOOLTIP
        peakDetectionButton->setText(QApplication::translate("DockFft", "D", 0));
        fftSizeLabel->setText(QApplication::translate("DockFft", "FFT size", 0));
        wfLabel->setText(QApplication::translate("DockFft", "WF", 0));
    } // retranslateUi

};

namespace Ui {
    class DockFft: public Ui_DockFft {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCKFFT_H
