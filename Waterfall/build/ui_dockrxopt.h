/********************************************************************************
** Form generated from reading UI file 'dockrxopt.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCKRXOPT_H
#define UI_DOCKRXOPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qtgui/freqctrl.h"

QT_BEGIN_NAMESPACE

class Ui_DockRxOpt
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    CFreqCtrl *filterFreq;
    QHBoxLayout *horizontalLayout_6;
    QLabel *rxFreqLabel;
    QLabel *rxFreq;
    QFrame *hline1;
    QGridLayout *gridLayout;
    QLabel *filterLabel;
    QComboBox *filterCombo;
    QPushButton *filterButton;
    QLabel *demodLabel;
    QComboBox *modeSelector;
    QPushButton *modeButton;
    QLabel *agcLabel;
    QComboBox *agcPresetCombo;
    QPushButton *agcButton;
    QFrame *hline2;
    QHBoxLayout *horizontalLayout;
    QPushButton *nb1Button;
    QPushButton *nb2Button;
    QPushButton *nbOptButton;
    QFrame *hline3;
    QGridLayout *gridLayout_2;
    QLabel *sqlLabel;
    QLabel *sqlDbLabel;
    QSlider *sqlSlider;
    QSpacerItem *verticalSpacer;

    void setupUi(QDockWidget *DockRxOpt)
    {
        if (DockRxOpt->objectName().isEmpty())
            DockRxOpt->setObjectName(QStringLiteral("DockRxOpt"));
        DockRxOpt->resize(234, 320);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DockRxOpt->sizePolicy().hasHeightForWidth());
        DockRxOpt->setSizePolicy(sizePolicy);
        DockRxOpt->setMinimumSize(QSize(234, 320));
        DockRxOpt->setMaximumSize(QSize(524287, 524287));
        DockRxOpt->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        filterFreq = new CFreqCtrl(dockWidgetContents);
        filterFreq->setObjectName(QStringLiteral("filterFreq"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(filterFreq->sizePolicy().hasHeightForWidth());
        filterFreq->setSizePolicy(sizePolicy1);
        filterFreq->setMinimumSize(QSize(200, 32));
        filterFreq->setFrameShape(QFrame::StyledPanel);
        filterFreq->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(filterFreq);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetDefaultConstraint);
        rxFreqLabel = new QLabel(dockWidgetContents);
        rxFreqLabel->setObjectName(QStringLiteral("rxFreqLabel"));
        rxFreqLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(rxFreqLabel);

        rxFreq = new QLabel(dockWidgetContents);
        rxFreq->setObjectName(QStringLiteral("rxFreq"));
        rxFreq->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(rxFreq);


        verticalLayout->addLayout(horizontalLayout_6);

        hline1 = new QFrame(dockWidgetContents);
        hline1->setObjectName(QStringLiteral("hline1"));
        hline1->setFrameShape(QFrame::HLine);
        hline1->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(hline1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setVerticalSpacing(5);
        filterLabel = new QLabel(dockWidgetContents);
        filterLabel->setObjectName(QStringLiteral("filterLabel"));
        filterLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(filterLabel, 0, 0, 1, 1);

        filterCombo = new QComboBox(dockWidgetContents);
        filterCombo->setObjectName(QStringLiteral("filterCombo"));
        filterCombo->setEnabled(true);
        sizePolicy.setHeightForWidth(filterCombo->sizePolicy().hasHeightForWidth());
        filterCombo->setSizePolicy(sizePolicy);
        filterCombo->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(filterCombo, 0, 1, 1, 1);

        filterButton = new QPushButton(dockWidgetContents);
        filterButton->setObjectName(QStringLiteral("filterButton"));
        filterButton->setEnabled(false);
        sizePolicy1.setHeightForWidth(filterButton->sizePolicy().hasHeightForWidth());
        filterButton->setSizePolicy(sizePolicy1);
        filterButton->setMaximumSize(QSize(28, 28));
        QFont font;
        font.setPointSize(10);
        filterButton->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/settings.svg"), QSize(), QIcon::Normal, QIcon::Off);
        filterButton->setIcon(icon);
        filterButton->setIconSize(QSize(16, 16));
        filterButton->setCheckable(false);
        filterButton->setChecked(false);

        gridLayout->addWidget(filterButton, 0, 2, 1, 1);

        demodLabel = new QLabel(dockWidgetContents);
        demodLabel->setObjectName(QStringLiteral("demodLabel"));
        sizePolicy.setHeightForWidth(demodLabel->sizePolicy().hasHeightForWidth());
        demodLabel->setSizePolicy(sizePolicy);
        demodLabel->setAlignment(Qt::AlignCenter);
        demodLabel->setMargin(0);
        demodLabel->setIndent(19);

        gridLayout->addWidget(demodLabel, 1, 0, 1, 1);

        modeSelector = new QComboBox(dockWidgetContents);
        modeSelector->setObjectName(QStringLiteral("modeSelector"));
        sizePolicy.setHeightForWidth(modeSelector->sizePolicy().hasHeightForWidth());
        modeSelector->setSizePolicy(sizePolicy);
        modeSelector->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(modeSelector, 1, 1, 1, 1);

        modeButton = new QPushButton(dockWidgetContents);
        modeButton->setObjectName(QStringLiteral("modeButton"));
        sizePolicy1.setHeightForWidth(modeButton->sizePolicy().hasHeightForWidth());
        modeButton->setSizePolicy(sizePolicy1);
        modeButton->setMaximumSize(QSize(28, 28));
        modeButton->setFont(font);
        modeButton->setIcon(icon);
        modeButton->setIconSize(QSize(16, 16));
        modeButton->setCheckable(false);
        modeButton->setChecked(false);

        gridLayout->addWidget(modeButton, 1, 2, 1, 1);

        agcLabel = new QLabel(dockWidgetContents);
        agcLabel->setObjectName(QStringLiteral("agcLabel"));

        gridLayout->addWidget(agcLabel, 2, 0, 1, 1);

        agcPresetCombo = new QComboBox(dockWidgetContents);
        agcPresetCombo->setObjectName(QStringLiteral("agcPresetCombo"));
        agcPresetCombo->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(agcPresetCombo->sizePolicy().hasHeightForWidth());
        agcPresetCombo->setSizePolicy(sizePolicy2);
        agcPresetCombo->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(agcPresetCombo, 2, 1, 1, 1);

        agcButton = new QPushButton(dockWidgetContents);
        agcButton->setObjectName(QStringLiteral("agcButton"));
        sizePolicy1.setHeightForWidth(agcButton->sizePolicy().hasHeightForWidth());
        agcButton->setSizePolicy(sizePolicy1);
        agcButton->setMaximumSize(QSize(28, 28));
        agcButton->setFont(font);
        agcButton->setIcon(icon);
        agcButton->setCheckable(false);
        agcButton->setChecked(false);

        gridLayout->addWidget(agcButton, 2, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        hline2 = new QFrame(dockWidgetContents);
        hline2->setObjectName(QStringLiteral("hline2"));
        hline2->setFrameShape(QFrame::HLine);
        hline2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(hline2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        nb1Button = new QPushButton(dockWidgetContents);
        nb1Button->setObjectName(QStringLiteral("nb1Button"));
        nb1Button->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(28);
        sizePolicy3.setHeightForWidth(nb1Button->sizePolicy().hasHeightForWidth());
        nb1Button->setSizePolicy(sizePolicy3);
        nb1Button->setMinimumSize(QSize(0, 28));
        nb1Button->setMaximumSize(QSize(16777215, 16777215));
        nb1Button->setCheckable(true);

        horizontalLayout->addWidget(nb1Button);

        nb2Button = new QPushButton(dockWidgetContents);
        nb2Button->setObjectName(QStringLiteral("nb2Button"));
        nb2Button->setEnabled(true);
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(nb2Button->sizePolicy().hasHeightForWidth());
        nb2Button->setSizePolicy(sizePolicy4);
        nb2Button->setMinimumSize(QSize(0, 28));
        nb2Button->setMaximumSize(QSize(16777215, 16777215));
        nb2Button->setCheckable(true);

        horizontalLayout->addWidget(nb2Button);

        nbOptButton = new QPushButton(dockWidgetContents);
        nbOptButton->setObjectName(QStringLiteral("nbOptButton"));
        sizePolicy1.setHeightForWidth(nbOptButton->sizePolicy().hasHeightForWidth());
        nbOptButton->setSizePolicy(sizePolicy1);
        nbOptButton->setMaximumSize(QSize(28, 28));
        nbOptButton->setFont(font);
        nbOptButton->setIcon(icon);
        nbOptButton->setCheckable(false);
        nbOptButton->setChecked(false);

        horizontalLayout->addWidget(nbOptButton);


        verticalLayout->addLayout(horizontalLayout);

        hline3 = new QFrame(dockWidgetContents);
        hline3->setObjectName(QStringLiteral("hline3"));
        hline3->setFrameShape(QFrame::HLine);
        hline3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(hline3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        sqlLabel = new QLabel(dockWidgetContents);
        sqlLabel->setObjectName(QStringLiteral("sqlLabel"));
        sqlLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(sqlLabel, 0, 0, 1, 1);

        sqlDbLabel = new QLabel(dockWidgetContents);
        sqlDbLabel->setObjectName(QStringLiteral("sqlDbLabel"));
        sizePolicy.setHeightForWidth(sqlDbLabel->sizePolicy().hasHeightForWidth());
        sqlDbLabel->setSizePolicy(sizePolicy);
        sqlDbLabel->setAlignment(Qt::AlignCenter);
        sqlDbLabel->setMargin(0);
        sqlDbLabel->setIndent(1);

        gridLayout_2->addWidget(sqlDbLabel, 0, 1, 1, 1);

        sqlSlider = new QSlider(dockWidgetContents);
        sqlSlider->setObjectName(QStringLiteral("sqlSlider"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(sqlSlider->sizePolicy().hasHeightForWidth());
        sqlSlider->setSizePolicy(sizePolicy5);
        sqlSlider->setMinimumSize(QSize(0, 22));
        sqlSlider->setMinimum(-1500);
        sqlSlider->setMaximum(0);
        sqlSlider->setPageStep(100);
        sqlSlider->setValue(-1500);
        sqlSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(sqlSlider, 1, 0, 1, 2);


        verticalLayout->addLayout(gridLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        DockRxOpt->setWidget(dockWidgetContents);

        retranslateUi(DockRxOpt);

        filterCombo->setCurrentIndex(1);
        modeSelector->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(DockRxOpt);
    } // setupUi

    void retranslateUi(QDockWidget *DockRxOpt)
    {
        DockRxOpt->setWindowTitle(QApplication::translate("DockRxOpt", "Receiver Options", 0));
#ifndef QT_NO_TOOLTIP
        filterFreq->setToolTip(QApplication::translate("DockRxOpt", "<html><head/><body><p>Channel filter offset.<br/>\n"
"This is an offset from the hardware RF frequency.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        rxFreqLabel->setToolTip(QApplication::translate("DockRxOpt", "The frequency we actualy are listening on calculated\n"
"as  rf_freq + filter_offset", 0));
#endif // QT_NO_TOOLTIP
        rxFreqLabel->setText(QApplication::translate("DockRxOpt", "Receive:", 0));
#ifndef QT_NO_TOOLTIP
        rxFreq->setToolTip(QApplication::translate("DockRxOpt", "The frequency we actualy are listening on.\n"
"Calculated as  rf_freq + filter_offset", 0));
#endif // QT_NO_TOOLTIP
        rxFreq->setText(QApplication::translate("DockRxOpt", "144.500000 MHz", 0));
        filterLabel->setText(QApplication::translate("DockRxOpt", "Filter", 0));
        filterCombo->clear();
        filterCombo->insertItems(0, QStringList()
         << QApplication::translate("DockRxOpt", "Wide", 0)
         << QApplication::translate("DockRxOpt", "Normal", 0)
         << QApplication::translate("DockRxOpt", "Narrow", 0)
         << QApplication::translate("DockRxOpt", "User", 0)
        );
#ifndef QT_NO_TOOLTIP
        filterCombo->setToolTip(QApplication::translate("DockRxOpt", "Apply mode dependent filter preset", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        filterButton->setToolTip(QApplication::translate("DockRxOpt", "Channel filter settings", 0));
#endif // QT_NO_TOOLTIP
        filterButton->setText(QString());
        demodLabel->setText(QApplication::translate("DockRxOpt", "Mode", 0));
        modeSelector->clear();
        modeSelector->insertItems(0, QStringList()
         << QApplication::translate("DockRxOpt", "Demod Off", 0)
         << QApplication::translate("DockRxOpt", "Raw I/Q", 0)
         << QApplication::translate("DockRxOpt", "AM", 0)
         << QApplication::translate("DockRxOpt", "Narrow FM", 0)
         << QApplication::translate("DockRxOpt", "Wide FM (mono)", 0)
         << QApplication::translate("DockRxOpt", "Wide FM (stereo)", 0)
         << QApplication::translate("DockRxOpt", "LSB", 0)
         << QApplication::translate("DockRxOpt", "USB", 0)
         << QApplication::translate("DockRxOpt", "CW-L", 0)
         << QApplication::translate("DockRxOpt", "CW-U", 0)
        );
#ifndef QT_NO_TOOLTIP
        modeSelector->setToolTip(QApplication::translate("DockRxOpt", "Demodulator type (mode)", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        modeSelector->setStatusTip(QApplication::translate("DockRxOpt", "Demodulator type (mode)", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        modeButton->setToolTip(QApplication::translate("DockRxOpt", "Demodulator options", 0));
#endif // QT_NO_TOOLTIP
        modeButton->setText(QString());
        agcLabel->setText(QApplication::translate("DockRxOpt", "AGC", 0));
        agcPresetCombo->clear();
        agcPresetCombo->insertItems(0, QStringList()
         << QApplication::translate("DockRxOpt", "Fast", 0)
         << QApplication::translate("DockRxOpt", "Medium", 0)
         << QApplication::translate("DockRxOpt", "Slow", 0)
         << QApplication::translate("DockRxOpt", "User", 0)
         << QApplication::translate("DockRxOpt", "Off", 0)
        );
#ifndef QT_NO_TOOLTIP
        agcPresetCombo->setToolTip(QApplication::translate("DockRxOpt", "AGC presets", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        agcButton->setToolTip(QApplication::translate("DockRxOpt", "AGC options", 0));
#endif // QT_NO_TOOLTIP
        agcButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        nb1Button->setToolTip(QApplication::translate("DockRxOpt", "Noise blanker for pulse type noise", 0));
#endif // QT_NO_TOOLTIP
        nb1Button->setText(QApplication::translate("DockRxOpt", "NB1", 0));
#ifndef QT_NO_TOOLTIP
        nb2Button->setToolTip(QApplication::translate("DockRxOpt", "Noise blanker for non-pulse type noise", 0));
#endif // QT_NO_TOOLTIP
        nb2Button->setText(QApplication::translate("DockRxOpt", "NB2", 0));
#ifndef QT_NO_TOOLTIP
        nbOptButton->setToolTip(QApplication::translate("DockRxOpt", "Noise blanker options", 0));
#endif // QT_NO_TOOLTIP
        nbOptButton->setText(QString());
        sqlLabel->setText(QApplication::translate("DockRxOpt", "SQL", 0));
        sqlDbLabel->setText(QApplication::translate("DockRxOpt", "-150 dBFS", 0));
#ifndef QT_NO_TOOLTIP
        sqlSlider->setToolTip(QApplication::translate("DockRxOpt", "Squelch level in dBFS", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class DockRxOpt: public Ui_DockRxOpt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCKRXOPT_H
