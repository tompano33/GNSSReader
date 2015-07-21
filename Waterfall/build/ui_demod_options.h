/********************************************************************************
** Form generated from reading UI file 'demod_options.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEMOD_OPTIONS_H
#define UI_DEMOD_OPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CDemodOptions
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *demodOptions;
    QWidget *demodNoOpt;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QWidget *demodFmOpt;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *pagedemodFormLayout2;
    QLabel *maxdevLabel;
    QComboBox *maxdevSelector;
    QLabel *emphLabel;
    QComboBox *emphSelector;
    QWidget *demodAmOpt;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *dcrCheckBox;

    void setupUi(QDialog *CDemodOptions)
    {
        if (CDemodOptions->objectName().isEmpty())
            CDemodOptions->setObjectName(QStringLiteral("CDemodOptions"));
        CDemodOptions->resize(223, 110);
        verticalLayout = new QVBoxLayout(CDemodOptions);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        demodOptions = new QStackedWidget(CDemodOptions);
        demodOptions->setObjectName(QStringLiteral("demodOptions"));
        demodOptions->setMinimumSize(QSize(0, 72));
        demodNoOpt = new QWidget();
        demodNoOpt->setObjectName(QStringLiteral("demodNoOpt"));
        horizontalLayout = new QHBoxLayout(demodNoOpt);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(demodNoOpt);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        demodOptions->addWidget(demodNoOpt);
        demodFmOpt = new QWidget();
        demodFmOpt->setObjectName(QStringLiteral("demodFmOpt"));
        verticalLayout_2 = new QVBoxLayout(demodFmOpt);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        pagedemodFormLayout2 = new QFormLayout();
        pagedemodFormLayout2->setObjectName(QStringLiteral("pagedemodFormLayout2"));
        pagedemodFormLayout2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        pagedemodFormLayout2->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pagedemodFormLayout2->setHorizontalSpacing(10);
        pagedemodFormLayout2->setVerticalSpacing(5);
        pagedemodFormLayout2->setContentsMargins(5, -1, 5, 5);
        maxdevLabel = new QLabel(demodFmOpt);
        maxdevLabel->setObjectName(QStringLiteral("maxdevLabel"));
        maxdevLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        pagedemodFormLayout2->setWidget(0, QFormLayout::LabelRole, maxdevLabel);

        maxdevSelector = new QComboBox(demodFmOpt);
        maxdevSelector->setObjectName(QStringLiteral("maxdevSelector"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(maxdevSelector->sizePolicy().hasHeightForWidth());
        maxdevSelector->setSizePolicy(sizePolicy);
        maxdevSelector->setMinimumSize(QSize(0, 24));

        pagedemodFormLayout2->setWidget(0, QFormLayout::FieldRole, maxdevSelector);

        emphLabel = new QLabel(demodFmOpt);
        emphLabel->setObjectName(QStringLiteral("emphLabel"));
        emphLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        pagedemodFormLayout2->setWidget(1, QFormLayout::LabelRole, emphLabel);

        emphSelector = new QComboBox(demodFmOpt);
        emphSelector->setObjectName(QStringLiteral("emphSelector"));
        sizePolicy.setHeightForWidth(emphSelector->sizePolicy().hasHeightForWidth());
        emphSelector->setSizePolicy(sizePolicy);
        emphSelector->setMinimumSize(QSize(0, 24));
        emphSelector->setEditable(false);

        pagedemodFormLayout2->setWidget(1, QFormLayout::FieldRole, emphSelector);


        verticalLayout_2->addLayout(pagedemodFormLayout2);

        demodOptions->addWidget(demodFmOpt);
        demodAmOpt = new QWidget();
        demodAmOpt->setObjectName(QStringLiteral("demodAmOpt"));
        verticalLayout_3 = new QVBoxLayout(demodAmOpt);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        dcrCheckBox = new QCheckBox(demodAmOpt);
        dcrCheckBox->setObjectName(QStringLiteral("dcrCheckBox"));
        dcrCheckBox->setChecked(true);

        verticalLayout_3->addWidget(dcrCheckBox);

        demodOptions->addWidget(demodAmOpt);

        verticalLayout->addWidget(demodOptions);


        retranslateUi(CDemodOptions);

        demodOptions->setCurrentIndex(1);
        maxdevSelector->setCurrentIndex(1);
        emphSelector->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(CDemodOptions);
    } // setupUi

    void retranslateUi(QDialog *CDemodOptions)
    {
        CDemodOptions->setWindowTitle(QApplication::translate("CDemodOptions", "Mode options", 0));
        label->setText(QApplication::translate("CDemodOptions", "No options for\n"
"this demodulator", 0));
        maxdevLabel->setText(QApplication::translate("CDemodOptions", "Max dev", 0));
        maxdevSelector->clear();
        maxdevSelector->insertItems(0, QStringList()
         << QApplication::translate("CDemodOptions", "Voice (2.5k)", 0)
         << QApplication::translate("CDemodOptions", "Voice (5k)", 0)
         << QApplication::translate("CDemodOptions", "APT (17k)", 0)
         << QApplication::translate("CDemodOptions", "BC (75k)", 0)
        );
#ifndef QT_NO_TOOLTIP
        maxdevSelector->setToolTip(QApplication::translate("CDemodOptions", "Maximum FM deviation", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        maxdevSelector->setStatusTip(QApplication::translate("CDemodOptions", "Maximum FM deviation", 0));
#endif // QT_NO_STATUSTIP
        emphLabel->setText(QApplication::translate("CDemodOptions", "Tau", 0));
        emphSelector->clear();
        emphSelector->insertItems(0, QStringList()
         << QApplication::translate("CDemodOptions", "Off", 0)
         << QApplication::translate("CDemodOptions", "25 \316\274s", 0)
         << QApplication::translate("CDemodOptions", "50 \316\274s", 0)
         << QApplication::translate("CDemodOptions", "75 \316\274s", 0)
         << QApplication::translate("CDemodOptions", "100 \316\274s", 0)
         << QApplication::translate("CDemodOptions", "250 \316\274s", 0)
         << QApplication::translate("CDemodOptions", "530 \316\274s", 0)
         << QApplication::translate("CDemodOptions", "1 ms", 0)
        );
#ifndef QT_NO_TOOLTIP
        emphSelector->setToolTip(QApplication::translate("CDemodOptions", "Time constant for the FM de-emphasis IIR filter.\n"
"For narrow band FM use 530 \316\274s.\n"
"50 \316\274s and 75 \316\274s are used for broadcast FM depending on region.\n"
"For digital modes it is best to switch it off.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        emphSelector->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        dcrCheckBox->setToolTip(QApplication::translate("CDemodOptions", "Enable/disable DC removal.", 0));
#endif // QT_NO_TOOLTIP
        dcrCheckBox->setText(QApplication::translate("CDemodOptions", "DCR", 0));
    } // retranslateUi

};

namespace Ui {
    class CDemodOptions: public Ui_CDemodOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEMOD_OPTIONS_H
