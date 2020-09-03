/********************************************************************************
** Form generated from reading UI file 'lintest.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINTEST_H
#define UI_LINTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LinTest
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *dsbxMeasMan;
    QDoubleSpinBox *dsbxCurrent;
    QLabel *label_3;
    QDoubleSpinBox *dsbxStep;
    QDoubleSpinBox *dsbxMeasScpi;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *LinTest)
    {
        if (LinTest->objectName().isEmpty())
            LinTest->setObjectName(QString::fromUtf8("LinTest"));
        LinTest->resize(640, 480);
        gridLayout = new QGridLayout(LinTest);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dsbxMeasMan = new QDoubleSpinBox(LinTest);
        dsbxMeasMan->setObjectName(QString::fromUtf8("dsbxMeasMan"));
        dsbxMeasMan->setDecimals(3);
        dsbxMeasMan->setMinimum(-9999.000000000000000);
        dsbxMeasMan->setMaximum(9999.000000000000000);

        gridLayout->addWidget(dsbxMeasMan, 1, 2, 1, 1);

        dsbxCurrent = new QDoubleSpinBox(LinTest);
        dsbxCurrent->setObjectName(QString::fromUtf8("dsbxCurrent"));
        dsbxCurrent->setDecimals(3);
        dsbxCurrent->setMinimum(-9999.000000000000000);
        dsbxCurrent->setMaximum(9999.000000000000000);

        gridLayout->addWidget(dsbxCurrent, 1, 0, 1, 1);

        label_3 = new QLabel(LinTest);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        dsbxStep = new QDoubleSpinBox(LinTest);
        dsbxStep->setObjectName(QString::fromUtf8("dsbxStep"));
        dsbxStep->setDecimals(3);
        dsbxStep->setMinimum(-9999.000000000000000);
        dsbxStep->setMaximum(9999.000000000000000);

        gridLayout->addWidget(dsbxStep, 1, 1, 1, 1);

        dsbxMeasScpi = new QDoubleSpinBox(LinTest);
        dsbxMeasScpi->setObjectName(QString::fromUtf8("dsbxMeasScpi"));
        dsbxMeasScpi->setDecimals(3);
        dsbxMeasScpi->setMinimum(-9999.000000000000000);
        dsbxMeasScpi->setMaximum(9999.000000000000000);

        gridLayout->addWidget(dsbxMeasScpi, 1, 3, 1, 1);

        label_2 = new QLabel(LinTest);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        label_4 = new QLabel(LinTest);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        label = new QLabel(LinTest);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        pushButton = new QPushButton(LinTest);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setCheckable(true);

        gridLayout->addWidget(pushButton, 2, 0, 1, 4);


        retranslateUi(LinTest);

        QMetaObject::connectSlotsByName(LinTest);
    } // setupUi

    void retranslateUi(QWidget *LinTest)
    {
        LinTest->setWindowTitle(QApplication::translate("LinTest", "Form", nullptr));
        label_3->setText(QApplication::translate("LinTest", "man i meas ", nullptr));
        label_2->setText(QApplication::translate("LinTest", "scpi i meas ", nullptr));
        label_4->setText(QApplication::translate("LinTest", "current", nullptr));
        label->setText(QApplication::translate("LinTest", "step mA", nullptr));
        pushButton->setText(QApplication::translate("LinTest", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LinTest: public Ui_LinTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINTEST_H
