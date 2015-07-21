/********************************************************************************
** Form generated from reading UI file 'agc_options.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AGC_OPTIONS_H
#define UI_AGC_OPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_CAgcOptions
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label3;
    QLabel *label2;
    QSlider *slopeSlider;
    QLabel *thresholdLabel;
    QLabel *label4;
    QPushButton *hangButton;
    QLabel *gainLabel;
    QSlider *decaySlider;
    QLabel *decayLabel;
    QSlider *gainSlider;
    QLabel *label1;
    QLabel *slopeLabel;
    QLabel *label5;
    QSlider *thresholdSlider;

    void setupUi(QDialog *CAgcOptions)
    {
        if (CAgcOptions->objectName().isEmpty())
            CAgcOptions->setObjectName(QStringLiteral("CAgcOptions"));
        CAgcOptions->resize(263, 197);
        CAgcOptions->setFocusPolicy(Qt::StrongFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/signal.svg"), QSize(), QIcon::Normal, QIcon::Off);
        CAgcOptions->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(CAgcOptions);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(CAgcOptions);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setVerticalSpacing(5);
        label3 = new QLabel(groupBox);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setEnabled(false);
        label3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label3, 4, 0, 1, 1);

        label2 = new QLabel(groupBox);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label2, 2, 0, 1, 1);

        slopeSlider = new QSlider(groupBox);
        slopeSlider->setObjectName(QStringLiteral("slopeSlider"));
        slopeSlider->setEnabled(false);
        slopeSlider->setFocusPolicy(Qt::StrongFocus);
        slopeSlider->setMaximum(10);
        slopeSlider->setPageStep(1);
        slopeSlider->setValue(2);
        slopeSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slopeSlider, 4, 1, 1, 1);

        thresholdLabel = new QLabel(groupBox);
        thresholdLabel->setObjectName(QStringLiteral("thresholdLabel"));
        thresholdLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(thresholdLabel, 2, 2, 1, 1);

        label4 = new QLabel(groupBox);
        label4->setObjectName(QStringLiteral("label4"));
        label4->setEnabled(false);
        label4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label4, 5, 0, 1, 1);

        hangButton = new QPushButton(groupBox);
        hangButton->setObjectName(QStringLiteral("hangButton"));
        hangButton->setCheckable(true);
        hangButton->setAutoDefault(false);
        hangButton->setFlat(false);

        gridLayout->addWidget(hangButton, 6, 1, 1, 1);

        gainLabel = new QLabel(groupBox);
        gainLabel->setObjectName(QStringLiteral("gainLabel"));
        gainLabel->setEnabled(false);
        gainLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(gainLabel, 0, 2, 1, 1);

        decaySlider = new QSlider(groupBox);
        decaySlider->setObjectName(QStringLiteral("decaySlider"));
        decaySlider->setEnabled(false);
        decaySlider->setFocusPolicy(Qt::StrongFocus);
        decaySlider->setMinimum(50);
        decaySlider->setMaximum(5000);
        decaySlider->setSingleStep(10);
        decaySlider->setPageStep(50);
        decaySlider->setValue(100);
        decaySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(decaySlider, 5, 1, 1, 1);

        decayLabel = new QLabel(groupBox);
        decayLabel->setObjectName(QStringLiteral("decayLabel"));
        decayLabel->setEnabled(false);
        decayLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(decayLabel, 5, 2, 1, 1);

        gainSlider = new QSlider(groupBox);
        gainSlider->setObjectName(QStringLiteral("gainSlider"));
        gainSlider->setEnabled(false);
        gainSlider->setFocusPolicy(Qt::StrongFocus);
        gainSlider->setMaximum(100);
        gainSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(gainSlider, 0, 1, 1, 1);

        label1 = new QLabel(groupBox);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setEnabled(false);
        label1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label1, 0, 0, 1, 1);

        slopeLabel = new QLabel(groupBox);
        slopeLabel->setObjectName(QStringLiteral("slopeLabel"));
        slopeLabel->setEnabled(false);
        slopeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(slopeLabel, 4, 2, 1, 1);

        label5 = new QLabel(groupBox);
        label5->setObjectName(QStringLiteral("label5"));
        label5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label5, 6, 0, 1, 1);

        thresholdSlider = new QSlider(groupBox);
        thresholdSlider->setObjectName(QStringLiteral("thresholdSlider"));
        thresholdSlider->setFocusPolicy(Qt::StrongFocus);
        thresholdSlider->setMinimum(-160);
        thresholdSlider->setMaximum(0);
        thresholdSlider->setValue(-100);
        thresholdSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(thresholdSlider, 2, 1, 1, 1);


        horizontalLayout->addWidget(groupBox);

        QWidget::setTabOrder(gainSlider, thresholdSlider);
        QWidget::setTabOrder(thresholdSlider, slopeSlider);
        QWidget::setTabOrder(slopeSlider, decaySlider);
        QWidget::setTabOrder(decaySlider, hangButton);

        retranslateUi(CAgcOptions);

        QMetaObject::connectSlotsByName(CAgcOptions);
    } // setupUi

    void retranslateUi(QDialog *CAgcOptions)
    {
        CAgcOptions->setWindowTitle(QApplication::translate("CAgcOptions", "AGC Settings", 0));
        groupBox->setTitle(QApplication::translate("CAgcOptions", "AGC settings", 0));
        label3->setText(QApplication::translate("CAgcOptions", "Slope", 0));
        label2->setText(QApplication::translate("CAgcOptions", "Threshold", 0));
#ifndef QT_NO_TOOLTIP
        slopeSlider->setToolTip(QApplication::translate("CAgcOptions", "AGC slope", 0));
#endif // QT_NO_TOOLTIP
        thresholdLabel->setText(QApplication::translate("CAgcOptions", "-100 dB", 0));
        label4->setText(QApplication::translate("CAgcOptions", "Decay", 0));
#ifndef QT_NO_TOOLTIP
        hangButton->setToolTip(QApplication::translate("CAgcOptions", "Enable / disable AGC hang", 0));
#endif // QT_NO_TOOLTIP
        hangButton->setText(QApplication::translate("CAgcOptions", "Disabled", 0));
#ifndef QT_NO_TOOLTIP
        gainLabel->setToolTip(QApplication::translate("CAgcOptions", "Manual gain. Used when AGC is switched off", 0));
#endif // QT_NO_TOOLTIP
        gainLabel->setText(QApplication::translate("CAgcOptions", "0 dB", 0));
#ifndef QT_NO_TOOLTIP
        decaySlider->setToolTip(QApplication::translate("CAgcOptions", "AGC decay time", 0));
#endif // QT_NO_TOOLTIP
        decayLabel->setText(QApplication::translate("CAgcOptions", "100 ms", 0));
#ifndef QT_NO_TOOLTIP
        gainSlider->setToolTip(QApplication::translate("CAgcOptions", "Manual gain. Used when AGC is switched off", 0));
#endif // QT_NO_TOOLTIP
        label1->setText(QApplication::translate("CAgcOptions", "Gain", 0));
        slopeLabel->setText(QApplication::translate("CAgcOptions", "2 dB", 0));
        label5->setText(QApplication::translate("CAgcOptions", "Hang", 0));
#ifndef QT_NO_TOOLTIP
        thresholdSlider->setToolTip(QApplication::translate("CAgcOptions", "AGC threshold (aka. knee)", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class CAgcOptions: public Ui_CAgcOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AGC_OPTIONS_H
