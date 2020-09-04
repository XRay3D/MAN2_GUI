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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Measurements
{
public:
    QVBoxLayout *verticalLayout_11;
    QGridLayout *gridLayout;
    QGroupBox *gbChannelAll;
    QVBoxLayout *verticalLayout;
    QDoubleSpinBox *dsbVoltage_9;
    QSpacerItem *verticalSpacer_3;
    QDoubleSpinBox *dsbSetCurrentAll;
    QPushButton *pbCurrentAll;
    QPushButton *pbShortAll;
    QComboBox *cbOsc;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QGroupBox *gbChannel_1;
    QVBoxLayout *verticalLayout_2;
    QDoubleSpinBox *dsbVoltage_1;
    QDoubleSpinBox *dsbCurrent_1;
    QDoubleSpinBox *dsbSetCurrent_1;
    QPushButton *pbCurrent_1;
    QPushButton *pbShort_1;
    QPushButton *pbOsc_1;
    QGroupBox *gbChannel_2;
    QVBoxLayout *verticalLayout_3;
    QDoubleSpinBox *dsbVoltage_2;
    QDoubleSpinBox *dsbCurrent_2;
    QDoubleSpinBox *dsbSetCurrent_2;
    QPushButton *pbCurrent_2;
    QPushButton *pbShort_2;
    QPushButton *pbOsc_2;
    QGroupBox *gbChannel_3;
    QVBoxLayout *verticalLayout_4;
    QDoubleSpinBox *dsbVoltage_3;
    QDoubleSpinBox *dsbCurrent_3;
    QDoubleSpinBox *dsbSetCurrent_3;
    QPushButton *pbCurrent_3;
    QPushButton *pbShort_3;
    QPushButton *pbOsc_3;
    QGroupBox *gbChannel_4;
    QVBoxLayout *verticalLayout_5;
    QDoubleSpinBox *dsbVoltage_4;
    QDoubleSpinBox *dsbCurrent_4;
    QDoubleSpinBox *dsbSetCurrent_4;
    QPushButton *pbCurrent_4;
    QPushButton *pbShort_4;
    QPushButton *pbOsc_4;
    QGroupBox *gbChannel_5;
    QVBoxLayout *verticalLayout_6;
    QDoubleSpinBox *dsbVoltage_5;
    QDoubleSpinBox *dsbCurrent_5;
    QDoubleSpinBox *dsbSetCurrent_5;
    QPushButton *pbCurrent_5;
    QPushButton *pbShort_5;
    QPushButton *pbOsc_5;
    QGroupBox *gbChannel_6;
    QVBoxLayout *verticalLayout_7;
    QDoubleSpinBox *dsbVoltage_6;
    QDoubleSpinBox *dsbCurrent_6;
    QDoubleSpinBox *dsbSetCurrent_6;
    QPushButton *pbCurrent_6;
    QPushButton *pbShort_6;
    QPushButton *pbOsc_6;
    QGroupBox *gbChannel_7;
    QVBoxLayout *verticalLayout_8;
    QDoubleSpinBox *dsbVoltage_7;
    QDoubleSpinBox *dsbCurrent_7;
    QDoubleSpinBox *dsbSetCurrent_7;
    QPushButton *pbCurrent_7;
    QPushButton *pbShort_7;
    QPushButton *pbOsc_7;
    QGroupBox *gbChannel_8;
    QVBoxLayout *verticalLayout_9;
    QDoubleSpinBox *dsbVoltage_8;
    QDoubleSpinBox *dsbCurrent_8;
    QDoubleSpinBox *dsbSetCurrent_8;
    QPushButton *pbCurrent_8;
    QPushButton *pbShort_8;
    QPushButton *pbOsc_8;
    QPushButton *pbStart;
    QLabel *label;
    QSpinBox *sbTimeout;
    QPushButton *pbClear;

    void setupUi(QWidget *Measurements)
    {
        if (Measurements->objectName().isEmpty())
            Measurements->setObjectName(QString::fromUtf8("Measurements"));
        Measurements->resize(1004, 250);
        verticalLayout_11 = new QVBoxLayout(Measurements);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 0);
        gbChannelAll = new QGroupBox(Measurements);
        gbChannelAll->setObjectName(QString::fromUtf8("gbChannelAll"));
        verticalLayout = new QVBoxLayout(gbChannelAll);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_9 = new QDoubleSpinBox(gbChannelAll);
        dsbVoltage_9->setObjectName(QString::fromUtf8("dsbVoltage_9"));
        dsbVoltage_9->setAlignment(Qt::AlignCenter);
        dsbVoltage_9->setReadOnly(true);
        dsbVoltage_9->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_9->setDecimals(2);
        dsbVoltage_9->setMinimum(0.000000000000000);
        dsbVoltage_9->setMaximum(500.000000000000000);
        dsbVoltage_9->setValue(0.000000000000000);

        verticalLayout->addWidget(dsbVoltage_9);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_3);

        dsbSetCurrentAll = new QDoubleSpinBox(gbChannelAll);
        dsbSetCurrentAll->setObjectName(QString::fromUtf8("dsbSetCurrentAll"));
        dsbSetCurrentAll->setAlignment(Qt::AlignCenter);
        dsbSetCurrentAll->setDecimals(1);
        dsbSetCurrentAll->setMaximum(4000.000000000000000);
        dsbSetCurrentAll->setSingleStep(1.000000000000000);

        verticalLayout->addWidget(dsbSetCurrentAll);

        pbCurrentAll = new QPushButton(gbChannelAll);
        pbCurrentAll->setObjectName(QString::fromUtf8("pbCurrentAll"));
        pbCurrentAll->setCheckable(true);

        verticalLayout->addWidget(pbCurrentAll);

        pbShortAll = new QPushButton(gbChannelAll);
        pbShortAll->setObjectName(QString::fromUtf8("pbShortAll"));
        pbShortAll->setCheckable(true);

        verticalLayout->addWidget(pbShortAll);

        cbOsc = new QComboBox(gbChannelAll);
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->addItem(QString());
        cbOsc->setObjectName(QString::fromUtf8("cbOsc"));

        verticalLayout->addWidget(cbOsc);


        gridLayout->addWidget(gbChannelAll, 0, 0, 1, 1);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 18, -1, 9);
        label_2 = new QLabel(Measurements);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_10->addWidget(label_2);

        label_4 = new QLabel(Measurements);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_10->addWidget(label_4);

        label_3 = new QLabel(Measurements);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_10->addWidget(label_3);

        label_5 = new QLabel(Measurements);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_10->addWidget(label_5);

        label_6 = new QLabel(Measurements);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_10->addWidget(label_6);

        label_7 = new QLabel(Measurements);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_10->addWidget(label_7);


        gridLayout->addLayout(verticalLayout_10, 0, 1, 1, 1);

        gbChannel_1 = new QGroupBox(Measurements);
        gbChannel_1->setObjectName(QString::fromUtf8("gbChannel_1"));
        gbChannel_1->setCheckable(true);
        verticalLayout_2 = new QVBoxLayout(gbChannel_1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_1 = new QDoubleSpinBox(gbChannel_1);
        dsbVoltage_1->setObjectName(QString::fromUtf8("dsbVoltage_1"));
        dsbVoltage_1->setAlignment(Qt::AlignCenter);
        dsbVoltage_1->setReadOnly(true);
        dsbVoltage_1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_1->setDecimals(3);
        dsbVoltage_1->setMinimum(-99.000000000000000);
        dsbVoltage_1->setMaximum(99.000000000000000);

        verticalLayout_2->addWidget(dsbVoltage_1);

        dsbCurrent_1 = new QDoubleSpinBox(gbChannel_1);
        dsbCurrent_1->setObjectName(QString::fromUtf8("dsbCurrent_1"));
        dsbCurrent_1->setAlignment(Qt::AlignCenter);
        dsbCurrent_1->setReadOnly(true);
        dsbCurrent_1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_1->setDecimals(1);
        dsbCurrent_1->setMaximum(4000.000000000000000);
        dsbCurrent_1->setSingleStep(1.000000000000000);

        verticalLayout_2->addWidget(dsbCurrent_1);

        dsbSetCurrent_1 = new QDoubleSpinBox(gbChannel_1);
        dsbSetCurrent_1->setObjectName(QString::fromUtf8("dsbSetCurrent_1"));
        dsbSetCurrent_1->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_1->setDecimals(1);
        dsbSetCurrent_1->setMaximum(4000.000000000000000);
        dsbSetCurrent_1->setSingleStep(1.000000000000000);

        verticalLayout_2->addWidget(dsbSetCurrent_1);

        pbCurrent_1 = new QPushButton(gbChannel_1);
        pbCurrent_1->setObjectName(QString::fromUtf8("pbCurrent_1"));
        pbCurrent_1->setCheckable(true);

        verticalLayout_2->addWidget(pbCurrent_1);

        pbShort_1 = new QPushButton(gbChannel_1);
        pbShort_1->setObjectName(QString::fromUtf8("pbShort_1"));
        pbShort_1->setCheckable(true);

        verticalLayout_2->addWidget(pbShort_1);

        pbOsc_1 = new QPushButton(gbChannel_1);
        pbOsc_1->setObjectName(QString::fromUtf8("pbOsc_1"));
        pbOsc_1->setCheckable(true);

        verticalLayout_2->addWidget(pbOsc_1);


        gridLayout->addWidget(gbChannel_1, 0, 2, 1, 1);

        gbChannel_2 = new QGroupBox(Measurements);
        gbChannel_2->setObjectName(QString::fromUtf8("gbChannel_2"));
        gbChannel_2->setCheckable(true);
        verticalLayout_3 = new QVBoxLayout(gbChannel_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_2 = new QDoubleSpinBox(gbChannel_2);
        dsbVoltage_2->setObjectName(QString::fromUtf8("dsbVoltage_2"));
        dsbVoltage_2->setAlignment(Qt::AlignCenter);
        dsbVoltage_2->setReadOnly(true);
        dsbVoltage_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_2->setDecimals(3);
        dsbVoltage_2->setMinimum(-99.000000000000000);
        dsbVoltage_2->setMaximum(99.000000000000000);

        verticalLayout_3->addWidget(dsbVoltage_2);

        dsbCurrent_2 = new QDoubleSpinBox(gbChannel_2);
        dsbCurrent_2->setObjectName(QString::fromUtf8("dsbCurrent_2"));
        dsbCurrent_2->setAlignment(Qt::AlignCenter);
        dsbCurrent_2->setReadOnly(true);
        dsbCurrent_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_2->setDecimals(1);
        dsbCurrent_2->setMaximum(4000.000000000000000);
        dsbCurrent_2->setSingleStep(1.000000000000000);

        verticalLayout_3->addWidget(dsbCurrent_2);

        dsbSetCurrent_2 = new QDoubleSpinBox(gbChannel_2);
        dsbSetCurrent_2->setObjectName(QString::fromUtf8("dsbSetCurrent_2"));
        dsbSetCurrent_2->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_2->setDecimals(1);
        dsbSetCurrent_2->setMaximum(4000.000000000000000);
        dsbSetCurrent_2->setSingleStep(1.000000000000000);

        verticalLayout_3->addWidget(dsbSetCurrent_2);

        pbCurrent_2 = new QPushButton(gbChannel_2);
        pbCurrent_2->setObjectName(QString::fromUtf8("pbCurrent_2"));
        pbCurrent_2->setCheckable(true);

        verticalLayout_3->addWidget(pbCurrent_2);

        pbShort_2 = new QPushButton(gbChannel_2);
        pbShort_2->setObjectName(QString::fromUtf8("pbShort_2"));
        pbShort_2->setCheckable(true);

        verticalLayout_3->addWidget(pbShort_2);

        pbOsc_2 = new QPushButton(gbChannel_2);
        pbOsc_2->setObjectName(QString::fromUtf8("pbOsc_2"));
        pbOsc_2->setCheckable(true);

        verticalLayout_3->addWidget(pbOsc_2);

        dsbCurrent_2->raise();
        pbShort_2->raise();
        pbCurrent_2->raise();
        pbOsc_2->raise();
        dsbVoltage_2->raise();
        dsbSetCurrent_2->raise();

        gridLayout->addWidget(gbChannel_2, 0, 3, 1, 1);

        gbChannel_3 = new QGroupBox(Measurements);
        gbChannel_3->setObjectName(QString::fromUtf8("gbChannel_3"));
        gbChannel_3->setCheckable(true);
        verticalLayout_4 = new QVBoxLayout(gbChannel_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_3 = new QDoubleSpinBox(gbChannel_3);
        dsbVoltage_3->setObjectName(QString::fromUtf8("dsbVoltage_3"));
        dsbVoltage_3->setAlignment(Qt::AlignCenter);
        dsbVoltage_3->setReadOnly(true);
        dsbVoltage_3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_3->setDecimals(3);
        dsbVoltage_3->setMinimum(-99.000000000000000);
        dsbVoltage_3->setMaximum(99.000000000000000);

        verticalLayout_4->addWidget(dsbVoltage_3);

        dsbCurrent_3 = new QDoubleSpinBox(gbChannel_3);
        dsbCurrent_3->setObjectName(QString::fromUtf8("dsbCurrent_3"));
        dsbCurrent_3->setAlignment(Qt::AlignCenter);
        dsbCurrent_3->setReadOnly(true);
        dsbCurrent_3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_3->setDecimals(1);
        dsbCurrent_3->setMaximum(4000.000000000000000);
        dsbCurrent_3->setSingleStep(1.000000000000000);

        verticalLayout_4->addWidget(dsbCurrent_3);

        dsbSetCurrent_3 = new QDoubleSpinBox(gbChannel_3);
        dsbSetCurrent_3->setObjectName(QString::fromUtf8("dsbSetCurrent_3"));
        dsbSetCurrent_3->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_3->setDecimals(1);
        dsbSetCurrent_3->setMaximum(4000.000000000000000);
        dsbSetCurrent_3->setSingleStep(1.000000000000000);

        verticalLayout_4->addWidget(dsbSetCurrent_3);

        pbCurrent_3 = new QPushButton(gbChannel_3);
        pbCurrent_3->setObjectName(QString::fromUtf8("pbCurrent_3"));
        pbCurrent_3->setCheckable(true);

        verticalLayout_4->addWidget(pbCurrent_3);

        pbShort_3 = new QPushButton(gbChannel_3);
        pbShort_3->setObjectName(QString::fromUtf8("pbShort_3"));
        pbShort_3->setCheckable(true);

        verticalLayout_4->addWidget(pbShort_3);

        pbOsc_3 = new QPushButton(gbChannel_3);
        pbOsc_3->setObjectName(QString::fromUtf8("pbOsc_3"));
        pbOsc_3->setCheckable(true);

        verticalLayout_4->addWidget(pbOsc_3);


        gridLayout->addWidget(gbChannel_3, 0, 4, 1, 1);

        gbChannel_4 = new QGroupBox(Measurements);
        gbChannel_4->setObjectName(QString::fromUtf8("gbChannel_4"));
        gbChannel_4->setCheckable(true);
        verticalLayout_5 = new QVBoxLayout(gbChannel_4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_4 = new QDoubleSpinBox(gbChannel_4);
        dsbVoltage_4->setObjectName(QString::fromUtf8("dsbVoltage_4"));
        dsbVoltage_4->setAlignment(Qt::AlignCenter);
        dsbVoltage_4->setReadOnly(true);
        dsbVoltage_4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_4->setDecimals(3);
        dsbVoltage_4->setMinimum(-99.000000000000000);
        dsbVoltage_4->setMaximum(99.000000000000000);

        verticalLayout_5->addWidget(dsbVoltage_4);

        dsbCurrent_4 = new QDoubleSpinBox(gbChannel_4);
        dsbCurrent_4->setObjectName(QString::fromUtf8("dsbCurrent_4"));
        dsbCurrent_4->setAlignment(Qt::AlignCenter);
        dsbCurrent_4->setReadOnly(true);
        dsbCurrent_4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_4->setDecimals(1);
        dsbCurrent_4->setMaximum(4000.000000000000000);
        dsbCurrent_4->setSingleStep(1.000000000000000);

        verticalLayout_5->addWidget(dsbCurrent_4);

        dsbSetCurrent_4 = new QDoubleSpinBox(gbChannel_4);
        dsbSetCurrent_4->setObjectName(QString::fromUtf8("dsbSetCurrent_4"));
        dsbSetCurrent_4->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_4->setDecimals(1);
        dsbSetCurrent_4->setMaximum(4000.000000000000000);
        dsbSetCurrent_4->setSingleStep(1.000000000000000);

        verticalLayout_5->addWidget(dsbSetCurrent_4);

        pbCurrent_4 = new QPushButton(gbChannel_4);
        pbCurrent_4->setObjectName(QString::fromUtf8("pbCurrent_4"));
        pbCurrent_4->setCheckable(true);

        verticalLayout_5->addWidget(pbCurrent_4);

        pbShort_4 = new QPushButton(gbChannel_4);
        pbShort_4->setObjectName(QString::fromUtf8("pbShort_4"));
        pbShort_4->setCheckable(true);

        verticalLayout_5->addWidget(pbShort_4);

        pbOsc_4 = new QPushButton(gbChannel_4);
        pbOsc_4->setObjectName(QString::fromUtf8("pbOsc_4"));
        pbOsc_4->setCheckable(true);

        verticalLayout_5->addWidget(pbOsc_4);


        gridLayout->addWidget(gbChannel_4, 0, 5, 1, 1);

        gbChannel_5 = new QGroupBox(Measurements);
        gbChannel_5->setObjectName(QString::fromUtf8("gbChannel_5"));
        gbChannel_5->setCheckable(true);
        verticalLayout_6 = new QVBoxLayout(gbChannel_5);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_5 = new QDoubleSpinBox(gbChannel_5);
        dsbVoltage_5->setObjectName(QString::fromUtf8("dsbVoltage_5"));
        dsbVoltage_5->setAlignment(Qt::AlignCenter);
        dsbVoltage_5->setReadOnly(true);
        dsbVoltage_5->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_5->setDecimals(3);
        dsbVoltage_5->setMinimum(-99.000000000000000);
        dsbVoltage_5->setMaximum(99.000000000000000);

        verticalLayout_6->addWidget(dsbVoltage_5);

        dsbCurrent_5 = new QDoubleSpinBox(gbChannel_5);
        dsbCurrent_5->setObjectName(QString::fromUtf8("dsbCurrent_5"));
        dsbCurrent_5->setAlignment(Qt::AlignCenter);
        dsbCurrent_5->setReadOnly(true);
        dsbCurrent_5->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_5->setDecimals(1);
        dsbCurrent_5->setMaximum(4000.000000000000000);
        dsbCurrent_5->setSingleStep(1.000000000000000);

        verticalLayout_6->addWidget(dsbCurrent_5);

        dsbSetCurrent_5 = new QDoubleSpinBox(gbChannel_5);
        dsbSetCurrent_5->setObjectName(QString::fromUtf8("dsbSetCurrent_5"));
        dsbSetCurrent_5->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_5->setDecimals(1);
        dsbSetCurrent_5->setMaximum(4000.000000000000000);
        dsbSetCurrent_5->setSingleStep(1.000000000000000);

        verticalLayout_6->addWidget(dsbSetCurrent_5);

        pbCurrent_5 = new QPushButton(gbChannel_5);
        pbCurrent_5->setObjectName(QString::fromUtf8("pbCurrent_5"));
        pbCurrent_5->setCheckable(true);

        verticalLayout_6->addWidget(pbCurrent_5);

        pbShort_5 = new QPushButton(gbChannel_5);
        pbShort_5->setObjectName(QString::fromUtf8("pbShort_5"));
        pbShort_5->setCheckable(true);

        verticalLayout_6->addWidget(pbShort_5);

        pbOsc_5 = new QPushButton(gbChannel_5);
        pbOsc_5->setObjectName(QString::fromUtf8("pbOsc_5"));
        pbOsc_5->setCheckable(true);

        verticalLayout_6->addWidget(pbOsc_5);


        gridLayout->addWidget(gbChannel_5, 0, 6, 1, 1);

        gbChannel_6 = new QGroupBox(Measurements);
        gbChannel_6->setObjectName(QString::fromUtf8("gbChannel_6"));
        gbChannel_6->setCheckable(true);
        verticalLayout_7 = new QVBoxLayout(gbChannel_6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_6 = new QDoubleSpinBox(gbChannel_6);
        dsbVoltage_6->setObjectName(QString::fromUtf8("dsbVoltage_6"));
        dsbVoltage_6->setAlignment(Qt::AlignCenter);
        dsbVoltage_6->setReadOnly(true);
        dsbVoltage_6->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_6->setDecimals(3);
        dsbVoltage_6->setMinimum(-99.000000000000000);
        dsbVoltage_6->setMaximum(99.000000000000000);

        verticalLayout_7->addWidget(dsbVoltage_6);

        dsbCurrent_6 = new QDoubleSpinBox(gbChannel_6);
        dsbCurrent_6->setObjectName(QString::fromUtf8("dsbCurrent_6"));
        dsbCurrent_6->setAlignment(Qt::AlignCenter);
        dsbCurrent_6->setReadOnly(true);
        dsbCurrent_6->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_6->setDecimals(1);
        dsbCurrent_6->setMaximum(4000.000000000000000);
        dsbCurrent_6->setSingleStep(1.000000000000000);

        verticalLayout_7->addWidget(dsbCurrent_6);

        dsbSetCurrent_6 = new QDoubleSpinBox(gbChannel_6);
        dsbSetCurrent_6->setObjectName(QString::fromUtf8("dsbSetCurrent_6"));
        dsbSetCurrent_6->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_6->setDecimals(1);
        dsbSetCurrent_6->setMaximum(4000.000000000000000);
        dsbSetCurrent_6->setSingleStep(1.000000000000000);

        verticalLayout_7->addWidget(dsbSetCurrent_6);

        pbCurrent_6 = new QPushButton(gbChannel_6);
        pbCurrent_6->setObjectName(QString::fromUtf8("pbCurrent_6"));
        pbCurrent_6->setCheckable(true);

        verticalLayout_7->addWidget(pbCurrent_6);

        pbShort_6 = new QPushButton(gbChannel_6);
        pbShort_6->setObjectName(QString::fromUtf8("pbShort_6"));
        pbShort_6->setCheckable(true);

        verticalLayout_7->addWidget(pbShort_6);

        pbOsc_6 = new QPushButton(gbChannel_6);
        pbOsc_6->setObjectName(QString::fromUtf8("pbOsc_6"));
        pbOsc_6->setCheckable(true);

        verticalLayout_7->addWidget(pbOsc_6);


        gridLayout->addWidget(gbChannel_6, 0, 7, 1, 1);

        gbChannel_7 = new QGroupBox(Measurements);
        gbChannel_7->setObjectName(QString::fromUtf8("gbChannel_7"));
        gbChannel_7->setCheckable(true);
        verticalLayout_8 = new QVBoxLayout(gbChannel_7);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_7 = new QDoubleSpinBox(gbChannel_7);
        dsbVoltage_7->setObjectName(QString::fromUtf8("dsbVoltage_7"));
        dsbVoltage_7->setAlignment(Qt::AlignCenter);
        dsbVoltage_7->setReadOnly(true);
        dsbVoltage_7->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_7->setDecimals(3);
        dsbVoltage_7->setMinimum(-99.000000000000000);
        dsbVoltage_7->setMaximum(99.000000000000000);

        verticalLayout_8->addWidget(dsbVoltage_7);

        dsbCurrent_7 = new QDoubleSpinBox(gbChannel_7);
        dsbCurrent_7->setObjectName(QString::fromUtf8("dsbCurrent_7"));
        dsbCurrent_7->setAlignment(Qt::AlignCenter);
        dsbCurrent_7->setReadOnly(true);
        dsbCurrent_7->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_7->setDecimals(1);
        dsbCurrent_7->setMaximum(4000.000000000000000);
        dsbCurrent_7->setSingleStep(1.000000000000000);

        verticalLayout_8->addWidget(dsbCurrent_7);

        dsbSetCurrent_7 = new QDoubleSpinBox(gbChannel_7);
        dsbSetCurrent_7->setObjectName(QString::fromUtf8("dsbSetCurrent_7"));
        dsbSetCurrent_7->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_7->setDecimals(1);
        dsbSetCurrent_7->setMaximum(4000.000000000000000);
        dsbSetCurrent_7->setSingleStep(1.000000000000000);

        verticalLayout_8->addWidget(dsbSetCurrent_7);

        pbCurrent_7 = new QPushButton(gbChannel_7);
        pbCurrent_7->setObjectName(QString::fromUtf8("pbCurrent_7"));
        pbCurrent_7->setCheckable(true);

        verticalLayout_8->addWidget(pbCurrent_7);

        pbShort_7 = new QPushButton(gbChannel_7);
        pbShort_7->setObjectName(QString::fromUtf8("pbShort_7"));
        pbShort_7->setCheckable(true);

        verticalLayout_8->addWidget(pbShort_7);

        pbOsc_7 = new QPushButton(gbChannel_7);
        pbOsc_7->setObjectName(QString::fromUtf8("pbOsc_7"));
        pbOsc_7->setCheckable(true);

        verticalLayout_8->addWidget(pbOsc_7);


        gridLayout->addWidget(gbChannel_7, 0, 8, 1, 1);

        gbChannel_8 = new QGroupBox(Measurements);
        gbChannel_8->setObjectName(QString::fromUtf8("gbChannel_8"));
        gbChannel_8->setCheckable(true);
        verticalLayout_9 = new QVBoxLayout(gbChannel_8);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(6, 6, 6, 6);
        dsbVoltage_8 = new QDoubleSpinBox(gbChannel_8);
        dsbVoltage_8->setObjectName(QString::fromUtf8("dsbVoltage_8"));
        dsbVoltage_8->setAlignment(Qt::AlignCenter);
        dsbVoltage_8->setReadOnly(true);
        dsbVoltage_8->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbVoltage_8->setDecimals(3);
        dsbVoltage_8->setMinimum(-99.000000000000000);
        dsbVoltage_8->setMaximum(99.000000000000000);

        verticalLayout_9->addWidget(dsbVoltage_8);

        dsbCurrent_8 = new QDoubleSpinBox(gbChannel_8);
        dsbCurrent_8->setObjectName(QString::fromUtf8("dsbCurrent_8"));
        dsbCurrent_8->setAlignment(Qt::AlignCenter);
        dsbCurrent_8->setReadOnly(true);
        dsbCurrent_8->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbCurrent_8->setDecimals(1);
        dsbCurrent_8->setMaximum(4000.000000000000000);
        dsbCurrent_8->setSingleStep(1.000000000000000);

        verticalLayout_9->addWidget(dsbCurrent_8);

        dsbSetCurrent_8 = new QDoubleSpinBox(gbChannel_8);
        dsbSetCurrent_8->setObjectName(QString::fromUtf8("dsbSetCurrent_8"));
        dsbSetCurrent_8->setAlignment(Qt::AlignCenter);
        dsbSetCurrent_8->setDecimals(1);
        dsbSetCurrent_8->setMaximum(4000.000000000000000);
        dsbSetCurrent_8->setSingleStep(1.000000000000000);

        verticalLayout_9->addWidget(dsbSetCurrent_8);

        pbCurrent_8 = new QPushButton(gbChannel_8);
        pbCurrent_8->setObjectName(QString::fromUtf8("pbCurrent_8"));
        pbCurrent_8->setCheckable(true);

        verticalLayout_9->addWidget(pbCurrent_8);

        pbShort_8 = new QPushButton(gbChannel_8);
        pbShort_8->setObjectName(QString::fromUtf8("pbShort_8"));
        pbShort_8->setCheckable(true);

        verticalLayout_9->addWidget(pbShort_8);

        pbOsc_8 = new QPushButton(gbChannel_8);
        pbOsc_8->setObjectName(QString::fromUtf8("pbOsc_8"));
        pbOsc_8->setCheckable(true);

        verticalLayout_9->addWidget(pbOsc_8);


        gridLayout->addWidget(gbChannel_8, 0, 9, 1, 1);

        pbStart = new QPushButton(Measurements);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setCheckable(true);

        gridLayout->addWidget(pbStart, 1, 0, 1, 1);

        label = new QLabel(Measurements);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        sbTimeout = new QSpinBox(Measurements);
        sbTimeout->setObjectName(QString::fromUtf8("sbTimeout"));
        sbTimeout->setMinimum(200);
        sbTimeout->setMaximum(10000);
        sbTimeout->setSingleStep(10);

        gridLayout->addWidget(sbTimeout, 1, 2, 1, 1);

        pbClear = new QPushButton(Measurements);
        pbClear->setObjectName(QString::fromUtf8("pbClear"));

        gridLayout->addWidget(pbClear, 1, 9, 1, 1);


        verticalLayout_11->addLayout(gridLayout);


        retranslateUi(Measurements);

        QMetaObject::connectSlotsByName(Measurements);
    } // setupUi

    void retranslateUi(QWidget *Measurements)
    {
        Measurements->setWindowTitle(QApplication::translate("Measurements", "Form", nullptr));
        gbChannelAll->setTitle(QApplication::translate("Measurements", "\320\224\320\273\321\217 \320\262\321\201\320\265\321\205", nullptr));
        dsbVoltage_9->setPrefix(QApplication::translate("Measurements", "~", nullptr));
        dsbVoltage_9->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbSetCurrentAll->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrentAll->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShortAll->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        cbOsc->setItemText(0, QApplication::translate("Measurements", "0", nullptr));
        cbOsc->setItemText(1, QApplication::translate("Measurements", "1", nullptr));
        cbOsc->setItemText(2, QApplication::translate("Measurements", "2", nullptr));
        cbOsc->setItemText(3, QApplication::translate("Measurements", "3", nullptr));
        cbOsc->setItemText(4, QApplication::translate("Measurements", "4", nullptr));
        cbOsc->setItemText(5, QApplication::translate("Measurements", "5", nullptr));
        cbOsc->setItemText(6, QApplication::translate("Measurements", "6", nullptr));
        cbOsc->setItemText(7, QApplication::translate("Measurements", "7", nullptr));
        cbOsc->setItemText(8, QApplication::translate("Measurements", "8", nullptr));

        label_2->setText(QApplication::translate("Measurements", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\320\276\320\265 \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265:", nullptr));
        label_4->setText(QApplication::translate("Measurements", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\321\213\320\271 \321\202\320\276\320\272:", nullptr));
        label_3->setText(QApplication::translate("Measurements", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275\320\275\321\213\320\271 \321\202\320\276\320\272:", nullptr));
        label_5->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \321\202\320\276\320\272:", nullptr));
        label_6->setText(QApplication::translate("Measurements", "\320\232\320\276\321\200\320\276\321\202\320\272\320\276\320\265 \320\267\320\260\320\274\321\213\320\272\320\260\320\275\320\270\320\265:", nullptr));
        label_7->setText(QApplication::translate("Measurements", "\320\236\321\201\321\206\320\270\320\273\320\273\320\276\320\263\321\200\320\260\321\204:", nullptr));
        gbChannel_1->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 1", nullptr));
        dsbVoltage_1->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_1->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_1->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_1->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_1->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_1->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_2->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 2", nullptr));
        dsbVoltage_2->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_2->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_2->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_2->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_2->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_2->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_3->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 3", nullptr));
        dsbVoltage_3->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_3->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_3->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_3->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_3->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_3->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_4->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 4", nullptr));
        dsbVoltage_4->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_4->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_4->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_4->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_4->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_4->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_5->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 5", nullptr));
        dsbVoltage_5->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_5->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_5->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_5->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_5->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_5->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_6->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 6", nullptr));
        dsbVoltage_6->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_6->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_6->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_6->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_6->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_6->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_7->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 7", nullptr));
        dsbVoltage_7->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_7->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_7->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_7->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_7->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_7->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        gbChannel_8->setTitle(QApplication::translate("Measurements", "\320\232\320\260\320\275\320\260\320\273 8", nullptr));
        dsbVoltage_8->setSuffix(QApplication::translate("Measurements", " \320\222", nullptr));
        dsbCurrent_8->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        dsbSetCurrent_8->setSuffix(QApplication::translate("Measurements", " \320\274A", nullptr));
        pbCurrent_8->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbShort_8->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
        pbOsc_8->setText(QApplication::translate("Measurements", "\320\222\320\272\320\273.", nullptr));
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
