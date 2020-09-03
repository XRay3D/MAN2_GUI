/********************************************************************************
** Form generated from reading UI file 'triptest.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIPTEST_H
#define UI_TRIPTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TripTest
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *dsbxStart;
    QLabel *label_2;
    QDoubleSpinBox *dsbxStop;
    QLabel *label_3;
    QDoubleSpinBox *dsbxStep;
    QPushButton *pushButton;

    void setupUi(QWidget *TripTest)
    {
        if (TripTest->objectName().isEmpty())
            TripTest->setObjectName(QString::fromUtf8("TripTest"));
        TripTest->resize(640, 480);
        verticalLayout = new QVBoxLayout(TripTest);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(TripTest);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        dsbxStart = new QDoubleSpinBox(TripTest);
        dsbxStart->setObjectName(QString::fromUtf8("dsbxStart"));
        dsbxStart->setMaximum(9999.000000000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, dsbxStart);

        label_2 = new QLabel(TripTest);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        dsbxStop = new QDoubleSpinBox(TripTest);
        dsbxStop->setObjectName(QString::fromUtf8("dsbxStop"));
        dsbxStop->setMaximum(9999.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, dsbxStop);

        label_3 = new QLabel(TripTest);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        dsbxStep = new QDoubleSpinBox(TripTest);
        dsbxStep->setObjectName(QString::fromUtf8("dsbxStep"));
        dsbxStep->setMaximum(9999.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, dsbxStep);


        verticalLayout->addLayout(formLayout);

        pushButton = new QPushButton(TripTest);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(TripTest);

        QMetaObject::connectSlotsByName(TripTest);
    } // setupUi

    void retranslateUi(QWidget *TripTest)
    {
        TripTest->setWindowTitle(QApplication::translate("TripTest", "Form", nullptr));
        label->setText(QApplication::translate("TripTest", "start", nullptr));
        dsbxStart->setSuffix(QApplication::translate("TripTest", " A", nullptr));
        label_2->setText(QApplication::translate("TripTest", "stop", nullptr));
        dsbxStop->setSuffix(QApplication::translate("TripTest", " A", nullptr));
        label_3->setText(QApplication::translate("TripTest", "step", nullptr));
        dsbxStep->setSuffix(QApplication::translate("TripTest", " A", nullptr));
        pushButton->setText(QApplication::translate("TripTest", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TripTest: public Ui_TripTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIPTEST_H
