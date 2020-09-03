/********************************************************************************
** Form generated from reading UI file 'measurements.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREMENTS_H
#define UI_MEASUREMENTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Measurements
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *dsbVoltage_1;
    QLabel *label_9;
    QCheckBox *chbx_3;
    QDoubleSpinBox *dsbVoltage_3;
    QCheckBox *chbx_1;
    QLabel *label_6;
    QLabel *label_7;
    QCheckBox *chbx_2;
    QPushButton *pbCurrent_1;
    QPushButton *pbShort_1;
    QPushButton *pbOsc_1;
    QDoubleSpinBox *dsbSetCurrent_1;
    QComboBox *cbxSt;
    QLabel *label_3;
    QLabel *label_5;
    QComboBox *cbxValType;
    QDoubleSpinBox *dsbVoltage_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbStart;
    QLabel *label;
    QSpinBox *sbTimeout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbClear;

    void setupUi(QWidget *Measurements)
    {
        if (Measurements->objectName().isEmpty())
            Measurements->setObjectName(QString::fromUtf8("Measurements"));
        Measurements->resize(668, 483);
        gridLayout = new QGridLayout(Measurements);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dsbVoltage_1 = new QDoubleSpinBox(Measurements);
        dsbVoltage_1->setObjectName(QString::fromUtf8("dsbVoltage_1"));
        dsbVoltage_1->setAlignment(Qt::AlignCenter);
        dsbVoltage_1->setReadOnly(true);
        dsbVoltage_1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_1->setDecimals(3);
        dsbVoltage_1->setMinimum(-9999.000000000000000);
        dsbVoltage_1->setMaximum(9999.000000000000000);
        dsbVoltage_1->setValue(0.000000000000000);

        gridLayout->addWidget(dsbVoltage_1, 1, 1, 1, 2);

        label_9 = new QLabel(Measurements);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 0, 0, 1, 1);

        chbx_3 = new QCheckBox(Measurements);
        chbx_3->setObjectName(QString::fromUtf8("chbx_3"));
        chbx_3->setLayoutDirection(Qt::RightToLeft);
        chbx_3->setAutoFillBackground(false);
        chbx_3->setChecked(true);

        gridLayout->addWidget(chbx_3, 3, 0, 1, 1);

        dsbVoltage_3 = new QDoubleSpinBox(Measurements);
        dsbVoltage_3->setObjectName(QString::fromUtf8("dsbVoltage_3"));
        dsbVoltage_3->setAlignment(Qt::AlignCenter);
        dsbVoltage_3->setReadOnly(true);
        dsbVoltage_3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_3->setDecimals(3);
        dsbVoltage_3->setMinimum(-9999.000000000000000);
        dsbVoltage_3->setMaximum(9999.000000000000000);
        dsbVoltage_3->setSingleStep(1.000000000000000);

        gridLayout->addWidget(dsbVoltage_3, 3, 1, 1, 2);

        chbx_1 = new QCheckBox(Measurements);
        chbx_1->setObjectName(QString::fromUtf8("chbx_1"));
        chbx_1->setLayoutDirection(Qt::RightToLeft);
        chbx_1->setAutoFillBackground(false);
        chbx_1->setChecked(true);

        gridLayout->addWidget(chbx_1, 1, 0, 1, 1);

        label_6 = new QLabel(Measurements);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 9, 0, 1, 1);

        label_7 = new QLabel(Measurements);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 10, 0, 1, 1);

        chbx_2 = new QCheckBox(Measurements);
        chbx_2->setObjectName(QString::fromUtf8("chbx_2"));
        chbx_2->setLayoutDirection(Qt::RightToLeft);
        chbx_2->setAutoFillBackground(false);
        chbx_2->setChecked(true);

        gridLayout->addWidget(chbx_2, 2, 0, 1, 1);

        pbCurrent_1 = new QPushButton(Measurements);
        pbCurrent_1->setObjectName(QString::fromUtf8("pbCurrent_1"));
        pbCurrent_1->setCheckable(true);

        gridLayout->addWidget(pbCurrent_1, 8, 1, 1, 2);

        pbShort_1 = new QPushButton(Measurements);
        pbShort_1->setObjectName(QString::fromUtf8("pbShort_1"));
        pbShort_1->setCheckable(true);

        gridLayout->addWidget(pbShort_1, 9, 1, 1, 1);

        pbOsc_1 = new QPushButton(Measurements);
        pbOsc_1->setObjectName(QString::fromUtf8("pbOsc_1"));
        pbOsc_1->setCheckable(true);

        gridLayout->addWidget(pbOsc_1, 10, 1, 1, 2);

        dsbSetCurrent_1 = new QDoubleSpinBox(Measurements);
        dsbSetCurrent_1->setObjectName(QString::fromUtf8("dsbSetCurrent_1"));
        dsbSetCurrent_1->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_1->setDecimals(1);
        dsbSetCurrent_1->setMaximum(4000.000000000000000);
        dsbSetCurrent_1->setSingleStep(1.000000000000000);

        gridLayout->addWidget(dsbSetCurrent_1, 7, 1, 1, 2);

        cbxSt = new QComboBox(Measurements);
        cbxSt->addItem(QString());
        cbxSt->addItem(QString());
        cbxSt->addItem(QString());
        cbxSt->setObjectName(QString::fromUtf8("cbxSt"));

        gridLayout->addWidget(cbxSt, 9, 2, 1, 1);

        label_3 = new QLabel(Measurements);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 7, 0, 1, 1);

        label_5 = new QLabel(Measurements);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 8, 0, 1, 1);

        cbxValType = new QComboBox(Measurements);
        cbxValType->addItem(QString());
        cbxValType->addItem(QString());
        cbxValType->addItem(QString());
        cbxValType->addItem(QString());
        cbxValType->addItem(QString());
        cbxValType->setObjectName(QString::fromUtf8("cbxValType"));

        gridLayout->addWidget(cbxValType, 0, 1, 1, 2);

        dsbVoltage_2 = new QDoubleSpinBox(Measurements);
        dsbVoltage_2->setObjectName(QString::fromUtf8("dsbVoltage_2"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dsbVoltage_2->sizePolicy().hasHeightForWidth());
        dsbVoltage_2->setSizePolicy(sizePolicy);
        dsbVoltage_2->setAlignment(Qt::AlignCenter);
        dsbVoltage_2->setReadOnly(true);
        dsbVoltage_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_2->setDecimals(3);
        dsbVoltage_2->setMinimum(-9999.000000000000000);
        dsbVoltage_2->setMaximum(9999.000000000000000);

        gridLayout->addWidget(dsbVoltage_2, 2, 1, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbStart = new QPushButton(Measurements);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setCheckable(true);

        horizontalLayout->addWidget(pbStart);

        label = new QLabel(Measurements);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        sbTimeout = new QSpinBox(Measurements);
        sbTimeout->setObjectName(QString::fromUtf8("sbTimeout"));
        sbTimeout->setMinimum(200);
        sbTimeout->setMaximum(10000);
        sbTimeout->setSingleStep(10);

        horizontalLayout->addWidget(sbTimeout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbClear = new QPushButton(Measurements);
        pbClear->setObjectName(QString::fromUtf8("pbClear"));

        horizontalLayout->addWidget(pbClear);


        gridLayout->addLayout(horizontalLayout, 12, 0, 1, 3);


        retranslateUi(Measurements);

        QMetaObject::connectSlotsByName(Measurements);
    } // setupUi

    void retranslateUi(QWidget *Measurements)
    {
        Measurements->setWindowTitle(QApplication::translate("Measurements", "Form", nullptr));
        dsbVoltage_1->setSuffix(QString());
        label_9->setText(QApplication::translate("Measurements", "TextLabel", nullptr));
        chbx_3->setText(QApplication::translate("Measurements", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\320\276\320\265 3:", nullptr));
        dsbVoltage_3->setSuffix(QString());
        chbx_1->setText(QApplication::translate("Measurements", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\320\276\320\265 1:", nullptr));
        label_6->setText(QApplication::translate("Measurements", "\320\232\320\276\321\200\320\276\321\202\320\272\320\276\320\265 \320\267\320\260\320\274\321\213\320\272\320\260\320\275\320\270\320\265:", nullptr));
        label_7->setText(QApplication::translate("Measurements", "\320\236\321\201\321\206\320\270\320\273\320\273\320\276\320\263\321\200\320\260\321\204:", nullptr));
        chbx_2->setText(QApplication::translate("Measurements", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\320\276\320\265 2:", nullptr));
        pbCurrent_1->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_1->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_1->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        dsbSetCurrent_1->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        cbxSt->setItemText(0, QApplication::translate("Measurements", "GND", nullptr));
        cbxSt->setItemText(1, QApplication::translate("Measurements", "SHUNT", nullptr));
        cbxSt->setItemText(2, QApplication::translate("Measurements", "GND_AND_SHUNT", nullptr));

        label_3->setText(QApplication::translate("Measurements", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275\320\275\321\213\320\271 \321\202\320\276\320\272:", nullptr));
        label_5->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \321\202\320\276\320\272:", nullptr));
        cbxValType->setItemText(0, QApplication::translate("Measurements", "CURRENT_MEASURED_VALUE", nullptr));
        cbxValType->setItemText(1, QApplication::translate("Measurements", "VALUE_TRIP_CURRENT", nullptr));
        cbxValType->setItemText(2, QApplication::translate("Measurements", "CALIB_VOLTAGE", nullptr));
        cbxValType->setItemText(3, QApplication::translate("Measurements", "CALIB_CURRENT", nullptr));
        cbxValType->setItemText(4, QApplication::translate("Measurements", "RAW_DATA", nullptr));

        dsbVoltage_2->setSuffix(QString());
        pbStart->setText(QApplication::translate("Measurements", "\320\241\321\202\320\260\321\200\321\202", nullptr));
        label->setText(QApplication::translate("Measurements", "\320\230\320\275\321\202\320\265\321\200\320\262\320\260\320\273 \320\276\320\277\321\200\320\276\321\201\320\260, \320\274\321\201:", nullptr));
        pbClear->setText(QApplication::translate("Measurements", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Measurements: public Ui_Measurements {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREMENTS_H
