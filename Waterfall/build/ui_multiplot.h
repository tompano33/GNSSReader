/********************************************************************************
** Form generated from reading UI file 'multiplot.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIPLOT_H
#define UI_MULTIPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <qtgui/plotter.h>
#include "qtgui/freqctrl.h"
#include "qtgui/meter.h"

QT_BEGIN_NAMESPACE

class Ui_MultiPlot
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *plotLayout;
    CPlotter *plotter;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    CFreqCtrl *freqCtrl;
    CMeter *sMeter;
    QComboBox *windowTypeSelect;
    QProgressBar *bufferProgress;
    QComboBox *numBlocksSelect;
    QLabel *numBlocksLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *progressLabel;
    QLabel *fileStreamLabel;
    QMenuBar *menubar;
    QMenu *menuView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MultiPlot)
    {
        if (MultiPlot->objectName().isEmpty())
            MultiPlot->setObjectName(QStringLiteral("MultiPlot"));
        MultiPlot->resize(1091, 989);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MultiPlot->sizePolicy().hasHeightForWidth());
        MultiPlot->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MultiPlot);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 50, 1051, 881));
        plotLayout = new QGridLayout(gridLayoutWidget);
        plotLayout->setSpacing(0);
        plotLayout->setObjectName(QStringLiteral("plotLayout"));
        plotLayout->setSizeConstraint(QLayout::SetNoConstraint);
        plotLayout->setContentsMargins(0, 0, 0, 0);
        plotter = new CPlotter(gridLayoutWidget);
        plotter->setObjectName(QStringLiteral("plotter"));
        sizePolicy.setHeightForWidth(plotter->sizePolicy().hasHeightForWidth());
        plotter->setSizePolicy(sizePolicy);
        plotter->setSizeIncrement(QSize(0, 0));
        plotter->setAcceptDrops(false);
        plotter->setLayoutDirection(Qt::LeftToRight);
        plotter->setAutoFillBackground(false);
        plotter->setFrameShape(QFrame::StyledPanel);
        plotter->setFrameShadow(QFrame::Raised);

        plotLayout->addWidget(plotter, 0, 0, 1, 1);

        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 1011, 43));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        freqCtrl = new CFreqCtrl(layoutWidget);
        freqCtrl->setObjectName(QStringLiteral("freqCtrl"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(freqCtrl->sizePolicy().hasHeightForWidth());
        freqCtrl->setSizePolicy(sizePolicy1);
        freqCtrl->setMinimumSize(QSize(371, 41));
        freqCtrl->setFrameShape(QFrame::StyledPanel);
        freqCtrl->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(freqCtrl);

        sMeter = new CMeter(layoutWidget);
        sMeter->setObjectName(QStringLiteral("sMeter"));
        sizePolicy1.setHeightForWidth(sMeter->sizePolicy().hasHeightForWidth());
        sMeter->setSizePolicy(sizePolicy1);
        sMeter->setMinimumSize(QSize(171, 41));
        sMeter->setFrameShape(QFrame::StyledPanel);
        sMeter->setFrameShadow(QFrame::Raised);
        windowTypeSelect = new QComboBox(sMeter);
        windowTypeSelect->setObjectName(QStringLiteral("windowTypeSelect"));
        windowTypeSelect->setGeometry(QRect(32, 0, 171, 41));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(windowTypeSelect->sizePolicy().hasHeightForWidth());
        windowTypeSelect->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(sMeter);

        bufferProgress = new QProgressBar(layoutWidget);
        bufferProgress->setObjectName(QStringLiteral("bufferProgress"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(bufferProgress->sizePolicy().hasHeightForWidth());
        bufferProgress->setSizePolicy(sizePolicy3);
        bufferProgress->setValue(24);

        horizontalLayout->addWidget(bufferProgress);

        numBlocksSelect = new QComboBox(layoutWidget);
        numBlocksSelect->setObjectName(QStringLiteral("numBlocksSelect"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(numBlocksSelect->sizePolicy().hasHeightForWidth());
        numBlocksSelect->setSizePolicy(sizePolicy4);

        horizontalLayout->addWidget(numBlocksSelect);

        numBlocksLabel = new QLabel(layoutWidget);
        numBlocksLabel->setObjectName(QStringLiteral("numBlocksLabel"));

        horizontalLayout->addWidget(numBlocksLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        progressLabel = new QLabel(centralwidget);
        progressLabel->setObjectName(QStringLiteral("progressLabel"));
        progressLabel->setGeometry(QRect(1020, 170, 47, 13));
        fileStreamLabel = new QLabel(centralwidget);
        fileStreamLabel->setObjectName(QStringLiteral("fileStreamLabel"));
        fileStreamLabel->setGeometry(QRect(24, 940, 81, 20));
        MultiPlot->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MultiPlot);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1091, 26));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MultiPlot->setMenuBar(menubar);
        statusbar = new QStatusBar(MultiPlot);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MultiPlot->setStatusBar(statusbar);

        menubar->addAction(menuView->menuAction());

        retranslateUi(MultiPlot);

        windowTypeSelect->setCurrentIndex(3);
        numBlocksSelect->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MultiPlot);
    } // setupUi

    void retranslateUi(QMainWindow *MultiPlot)
    {
        MultiPlot->setWindowTitle(QApplication::translate("MultiPlot", "Multi Plots", 0));
        windowTypeSelect->clear();
        windowTypeSelect->insertItems(0, QStringList()
         << QApplication::translate("MultiPlot", "Hamming", 0)
         << QApplication::translate("MultiPlot", "Hanning", 0)
         << QApplication::translate("MultiPlot", "Blackman-Harris", 0)
         << QApplication::translate("MultiPlot", "None", 0)
        );
        numBlocksSelect->clear();
        numBlocksSelect->insertItems(0, QStringList()
         << QApplication::translate("MultiPlot", "1", 0)
         << QApplication::translate("MultiPlot", "2", 0)
         << QApplication::translate("MultiPlot", "5", 0)
         << QApplication::translate("MultiPlot", "10", 0)
         << QApplication::translate("MultiPlot", "50", 0)
         << QApplication::translate("MultiPlot", "100", 0)
         << QApplication::translate("MultiPlot", "500", 0)
         << QApplication::translate("MultiPlot", "1000", 0)
        );
        numBlocksLabel->setText(QApplication::translate("MultiPlot", " Number of Blocks", 0));
        progressLabel->setText(QString());
        fileStreamLabel->setText(QApplication::translate("MultiPlot", "Currently streaming:", 0));
        menuView->setTitle(QApplication::translate("MultiPlot", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class MultiPlot: public Ui_MultiPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIPLOT_H
