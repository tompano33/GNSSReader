/********************************************************************************
** Form generated from reading UI file 'nb_options.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NB_OPTIONS_H
#define UI_NB_OPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_CNbOptions
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *nb1Threshold;
    QDoubleSpinBox *nb2Threshold;
    QLabel *nb1Label;
    QLabel *nb2Label;

    void setupUi(QDialog *CNbOptions)
    {
        if (CNbOptions->objectName().isEmpty())
            CNbOptions->setObjectName(QStringLiteral("CNbOptions"));
        CNbOptions->resize(176, 105);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/signal.svg"), QSize(), QIcon::Normal, QIcon::Off);
        CNbOptions->setWindowIcon(icon);
        gridLayout = new QGridLayout(CNbOptions);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        nb1Threshold = new QDoubleSpinBox(CNbOptions);
        nb1Threshold->setObjectName(QStringLiteral("nb1Threshold"));
        nb1Threshold->setDecimals(1);
        nb1Threshold->setMinimum(1);
        nb1Threshold->setMaximum(20);
        nb1Threshold->setSingleStep(0.1);
        nb1Threshold->setValue(3.3);

        gridLayout->addWidget(nb1Threshold, 0, 1, 1, 1);

        nb2Threshold = new QDoubleSpinBox(CNbOptions);
        nb2Threshold->setObjectName(QStringLiteral("nb2Threshold"));
        nb2Threshold->setDecimals(1);
        nb2Threshold->setMaximum(15);
        nb2Threshold->setSingleStep(0.1);
        nb2Threshold->setValue(2.5);

        gridLayout->addWidget(nb2Threshold, 1, 1, 1, 1);

        nb1Label = new QLabel(CNbOptions);
        nb1Label->setObjectName(QStringLiteral("nb1Label"));
        nb1Label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(nb1Label, 0, 0, 1, 1);

        nb2Label = new QLabel(CNbOptions);
        nb2Label->setObjectName(QStringLiteral("nb2Label"));
        nb2Label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(nb2Label, 1, 0, 1, 1);


        retranslateUi(CNbOptions);

        QMetaObject::connectSlotsByName(CNbOptions);
    } // setupUi

    void retranslateUi(QDialog *CNbOptions)
    {
        CNbOptions->setWindowTitle(QApplication::translate("CNbOptions", "Noise blanker options", 0));
        nb1Label->setText(QApplication::translate("CNbOptions", "NB1 threshold", 0));
        nb2Label->setText(QApplication::translate("CNbOptions", "NB2 threshold", 0));
    } // retranslateUi

};

namespace Ui {
    class CNbOptions: public Ui_CNbOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NB_OPTIONS_H
