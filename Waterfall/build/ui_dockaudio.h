/********************************************************************************
** Form generated from reading UI file 'dockaudio.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCKAUDIO_H
#define UI_DOCKAUDIO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qtgui/plotter.h"

QT_BEGIN_NAMESPACE

class Ui_DockAudio
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    CPlotter *audioSpectrum;
    QGridLayout *gridGainLayout;
    QLabel *audioGainLabel;
    QLabel *audioGainDbLabel;
    QSlider *audioGainSlider;
    QHBoxLayout *audioRecLayout;
    QPushButton *audioRecConfButton;
    QPushButton *audioRecButton;
    QPushButton *audioPlayButton;
    QLabel *audioRecLabel;

    void setupUi(QDockWidget *DockAudio)
    {
        if (DockAudio->objectName().isEmpty())
            DockAudio->setObjectName(QStringLiteral("DockAudio"));
        DockAudio->resize(200, 239);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(200);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DockAudio->sizePolicy().hasHeightForWidth());
        DockAudio->setSizePolicy(sizePolicy);
        DockAudio->setMinimumSize(QSize(200, 220));
        DockAudio->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        audioSpectrum = new CPlotter(dockWidgetContents);
        audioSpectrum->setObjectName(QStringLiteral("audioSpectrum"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(audioSpectrum->sizePolicy().hasHeightForWidth());
        audioSpectrum->setSizePolicy(sizePolicy1);
        audioSpectrum->setMinimumSize(QSize(48, 48));
        audioSpectrum->setFrameShape(QFrame::StyledPanel);
        audioSpectrum->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(audioSpectrum);

        gridGainLayout = new QGridLayout();
        gridGainLayout->setObjectName(QStringLiteral("gridGainLayout"));
        gridGainLayout->setHorizontalSpacing(6);
        gridGainLayout->setVerticalSpacing(-1);
        audioGainLabel = new QLabel(dockWidgetContents);
        audioGainLabel->setObjectName(QStringLiteral("audioGainLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(audioGainLabel->sizePolicy().hasHeightForWidth());
        audioGainLabel->setSizePolicy(sizePolicy2);
        audioGainLabel->setAlignment(Qt::AlignCenter);

        gridGainLayout->addWidget(audioGainLabel, 0, 0, 1, 1);

        audioGainDbLabel = new QLabel(dockWidgetContents);
        audioGainDbLabel->setObjectName(QStringLiteral("audioGainDbLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(audioGainDbLabel->sizePolicy().hasHeightForWidth());
        audioGainDbLabel->setSizePolicy(sizePolicy3);
        audioGainDbLabel->setAlignment(Qt::AlignCenter);
        audioGainDbLabel->setMargin(0);
        audioGainDbLabel->setIndent(1);

        gridGainLayout->addWidget(audioGainDbLabel, 0, 1, 1, 1);

        audioGainSlider = new QSlider(dockWidgetContents);
        audioGainSlider->setObjectName(QStringLiteral("audioGainSlider"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(audioGainSlider->sizePolicy().hasHeightForWidth());
        audioGainSlider->setSizePolicy(sizePolicy4);
        audioGainSlider->setMinimumSize(QSize(0, 22));
        audioGainSlider->setMinimum(-800);
        audioGainSlider->setMaximum(500);
        audioGainSlider->setValue(-200);
        audioGainSlider->setOrientation(Qt::Horizontal);

        gridGainLayout->addWidget(audioGainSlider, 1, 0, 1, 2);


        verticalLayout->addLayout(gridGainLayout);

        audioRecLayout = new QHBoxLayout();
        audioRecLayout->setSpacing(0);
        audioRecLayout->setObjectName(QStringLiteral("audioRecLayout"));
        audioRecConfButton = new QPushButton(dockWidgetContents);
        audioRecConfButton->setObjectName(QStringLiteral("audioRecConfButton"));
        audioRecConfButton->setEnabled(false);
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(32);
        sizePolicy5.setVerticalStretch(32);
        sizePolicy5.setHeightForWidth(audioRecConfButton->sizePolicy().hasHeightForWidth());
        audioRecConfButton->setSizePolicy(sizePolicy5);
        audioRecConfButton->setMinimumSize(QSize(32, 32));
        audioRecConfButton->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Waterfall\\icons\\settings.svg"), QSize(), QIcon::Normal, QIcon::Off);
        audioRecConfButton->setIcon(icon);

        audioRecLayout->addWidget(audioRecConfButton);

        audioRecButton = new QPushButton(dockWidgetContents);
        audioRecButton->setObjectName(QStringLiteral("audioRecButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(audioRecButton->sizePolicy().hasHeightForWidth());
        audioRecButton->setSizePolicy(sizePolicy6);
        audioRecButton->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QStringLiteral("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Waterfall\\icons\\record.svg"), QSize(), QIcon::Normal, QIcon::Off);
        audioRecButton->setIcon(icon1);
        audioRecButton->setCheckable(true);

        audioRecLayout->addWidget(audioRecButton);

        audioPlayButton = new QPushButton(dockWidgetContents);
        audioPlayButton->setObjectName(QStringLiteral("audioPlayButton"));
        audioPlayButton->setEnabled(false);
        sizePolicy6.setHeightForWidth(audioPlayButton->sizePolicy().hasHeightForWidth());
        audioPlayButton->setSizePolicy(sizePolicy6);
        audioPlayButton->setMinimumSize(QSize(32, 32));
        audioPlayButton->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QStringLiteral("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Waterfall\\icons\\play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        audioPlayButton->setIcon(icon2);
        audioPlayButton->setCheckable(true);

        audioRecLayout->addWidget(audioPlayButton);

        audioRecLabel = new QLabel(dockWidgetContents);
        audioRecLabel->setObjectName(QStringLiteral("audioRecLabel"));
        QSizePolicy sizePolicy7(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(audioRecLabel->sizePolicy().hasHeightForWidth());
        audioRecLabel->setSizePolicy(sizePolicy7);
        audioRecLabel->setAlignment(Qt::AlignCenter);

        audioRecLayout->addWidget(audioRecLabel);


        verticalLayout->addLayout(audioRecLayout);

        DockAudio->setWidget(dockWidgetContents);

        retranslateUi(DockAudio);

        QMetaObject::connectSlotsByName(DockAudio);
    } // setupUi

    void retranslateUi(QDockWidget *DockAudio)
    {
        DockAudio->setWindowTitle(QApplication::translate("DockAudio", "Audio", 0));
#ifndef QT_NO_TOOLTIP
        audioSpectrum->setToolTip(QApplication::translate("DockAudio", "Audio spectrum", 0));
#endif // QT_NO_TOOLTIP
        audioGainLabel->setText(QApplication::translate("DockAudio", "Gain:", 0));
        audioGainDbLabel->setText(QApplication::translate("DockAudio", " -20 dB", 0));
#ifndef QT_NO_TOOLTIP
        audioGainSlider->setToolTip(QApplication::translate("DockAudio", "Audio gain", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        audioGainSlider->setStatusTip(QApplication::translate("DockAudio", "Audio gain", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        audioRecConfButton->setToolTip(QApplication::translate("DockAudio", "Configure audio recording and playback", 0));
#endif // QT_NO_TOOLTIP
        audioRecConfButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        audioRecButton->setToolTip(QApplication::translate("DockAudio", "Record audio", 0));
#endif // QT_NO_TOOLTIP
        audioRecButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        audioPlayButton->setToolTip(QApplication::translate("DockAudio", "Playback previously recorded audio", 0));
#endif // QT_NO_TOOLTIP
        audioPlayButton->setText(QString());
        audioRecLabel->setText(QApplication::translate("DockAudio", "--", 0));
    } // retranslateUi

};

namespace Ui {
    class DockAudio: public Ui_DockAudio {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCKAUDIO_H
