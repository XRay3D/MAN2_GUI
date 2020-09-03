/********************************************************************************
** Form generated from reading UI file 'graduation.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRADUATION_H
#define UI_GRADUATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Graduation
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_3;
    QComboBox *cbxScpi;
    QPushButton *pbPingScpi;
    QLabel *label_4;
    QComboBox *cbManCh;
    QGroupBox *groupBoxGrad;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *dsbxSet_2;
    QRadioButton *rbCurrent;
    QDoubleSpinBox *dsbxSet_1;
    QCheckBox *checkBox_5;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *checkBox_6;
    QPushButton *pbStartGrad;
    QCheckBox *checkBox_1;
    QDoubleSpinBox *dsbxMeasure_2;
    QCheckBox *checkBox_3;
    QDoubleSpinBox *dsbxMeasure_1;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_2;
    QPushButton *pbSave;
    QRadioButton *rbVoltage;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Graduation)
    {
        if (Graduation->objectName().isEmpty())
            Graduation->setObjectName(QString::fromUtf8("Graduation"));
        Graduation->resize(727, 423);
        horizontalLayout = new QHBoxLayout(Graduation);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox_8 = new QGroupBox(Graduation);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_8->sizePolicy().hasHeightForWidth());
        groupBox_8->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBox_8);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 4, 0, 1, 2);

        label_3 = new QLabel(groupBox_8);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        cbxScpi = new QComboBox(groupBox_8);
        cbxScpi->setObjectName(QString::fromUtf8("cbxScpi"));

        gridLayout->addWidget(cbxScpi, 0, 1, 1, 1);

        pbPingScpi = new QPushButton(groupBox_8);
        pbPingScpi->setObjectName(QString::fromUtf8("pbPingScpi"));

        gridLayout->addWidget(pbPingScpi, 1, 0, 1, 2);

        label_4 = new QLabel(groupBox_8);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        cbManCh = new QComboBox(groupBox_8);
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->addItem(QString());
        cbManCh->setObjectName(QString::fromUtf8("cbManCh"));

        gridLayout->addWidget(cbManCh, 2, 1, 1, 1);


        horizontalLayout->addWidget(groupBox_8);

        groupBoxGrad = new QGroupBox(Graduation);
        groupBoxGrad->setObjectName(QString::fromUtf8("groupBoxGrad"));
        groupBoxGrad->setEnabled(false);
        gridLayout_5 = new QGridLayout(groupBoxGrad);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(6, 6, 6, 6);
        dsbxSet_2 = new QDoubleSpinBox(groupBoxGrad);
        dsbxSet_2->setObjectName(QString::fromUtf8("dsbxSet_2"));
        dsbxSet_2->setAlignment(Qt::AlignCenter);
        dsbxSet_2->setReadOnly(true);
        dsbxSet_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbxSet_2->setAccelerated(true);
        dsbxSet_2->setKeyboardTracking(false);
        dsbxSet_2->setDecimals(5);

        gridLayout_5->addWidget(dsbxSet_2, 6, 1, 1, 1);

        rbCurrent = new QRadioButton(groupBoxGrad);
        rbCurrent->setObjectName(QString::fromUtf8("rbCurrent"));

        gridLayout_5->addWidget(rbCurrent, 3, 0, 1, 2);

        dsbxSet_1 = new QDoubleSpinBox(groupBoxGrad);
        dsbxSet_1->setObjectName(QString::fromUtf8("dsbxSet_1"));
        dsbxSet_1->setAlignment(Qt::AlignCenter);
        dsbxSet_1->setReadOnly(true);
        dsbxSet_1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbxSet_1->setAccelerated(true);
        dsbxSet_1->setKeyboardTracking(false);
        dsbxSet_1->setDecimals(5);
        dsbxSet_1->setMaximum(999.990000000000009);

        gridLayout_5->addWidget(dsbxSet_1, 5, 1, 1, 1);

        checkBox_5 = new QCheckBox(groupBoxGrad);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));

        gridLayout_5->addWidget(checkBox_5, 11, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 15, 0, 1, 2);

        checkBox_6 = new QCheckBox(groupBoxGrad);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

        gridLayout_5->addWidget(checkBox_6, 12, 0, 1, 2);

        pbStartGrad = new QPushButton(groupBoxGrad);
        pbStartGrad->setObjectName(QString::fromUtf8("pbStartGrad"));

        gridLayout_5->addWidget(pbStartGrad, 13, 0, 1, 2);

        checkBox_1 = new QCheckBox(groupBoxGrad);
        checkBox_1->setObjectName(QString::fromUtf8("checkBox_1"));

        gridLayout_5->addWidget(checkBox_1, 7, 0, 1, 2);

        dsbxMeasure_2 = new QDoubleSpinBox(groupBoxGrad);
        dsbxMeasure_2->setObjectName(QString::fromUtf8("dsbxMeasure_2"));
        dsbxMeasure_2->setAlignment(Qt::AlignCenter);
        dsbxMeasure_2->setReadOnly(true);
        dsbxMeasure_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbxMeasure_2->setAccelerated(true);
        dsbxMeasure_2->setKeyboardTracking(false);
        dsbxMeasure_2->setDecimals(5);

        gridLayout_5->addWidget(dsbxMeasure_2, 6, 0, 1, 1);

        checkBox_3 = new QCheckBox(groupBoxGrad);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        gridLayout_5->addWidget(checkBox_3, 9, 0, 1, 2);

        dsbxMeasure_1 = new QDoubleSpinBox(groupBoxGrad);
        dsbxMeasure_1->setObjectName(QString::fromUtf8("dsbxMeasure_1"));
        dsbxMeasure_1->setAlignment(Qt::AlignCenter);
        dsbxMeasure_1->setReadOnly(true);
        dsbxMeasure_1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dsbxMeasure_1->setAccelerated(true);
        dsbxMeasure_1->setKeyboardTracking(false);
        dsbxMeasure_1->setDecimals(5);
        dsbxMeasure_1->setMaximum(999.990000000000009);

        gridLayout_5->addWidget(dsbxMeasure_1, 5, 0, 1, 1);

        checkBox_4 = new QCheckBox(groupBoxGrad);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

        gridLayout_5->addWidget(checkBox_4, 10, 0, 1, 2);

        checkBox_2 = new QCheckBox(groupBoxGrad);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout_5->addWidget(checkBox_2, 8, 0, 1, 2);

        pbSave = new QPushButton(groupBoxGrad);
        pbSave->setObjectName(QString::fromUtf8("pbSave"));

        gridLayout_5->addWidget(pbSave, 14, 0, 1, 2);

        rbVoltage = new QRadioButton(groupBoxGrad);
        rbVoltage->setObjectName(QString::fromUtf8("rbVoltage"));
        rbVoltage->setChecked(true);

        gridLayout_5->addWidget(rbVoltage, 2, 0, 1, 2);

        label = new QLabel(groupBoxGrad);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_5->addWidget(label, 4, 0, 1, 1);

        label_2 = new QLabel(groupBoxGrad);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_5->addWidget(label_2, 4, 1, 1, 1);


        horizontalLayout->addWidget(groupBoxGrad);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(Graduation);

        QMetaObject::connectSlotsByName(Graduation);
    } // setupUi

    void retranslateUi(QWidget *Graduation)
    {
        Graduation->setWindowTitle(QApplication::translate("Graduation", "Form", nullptr));
        groupBox_8->setTitle(QApplication::translate("Graduation", "\320\241\320\262\321\217\320\267\321\214", nullptr));
        label_3->setText(QApplication::translate("Graduation", "\320\237\320\276\321\200\321\202 Agient 34401A (SCPI):", nullptr));
        pbPingScpi->setText(QApplication::translate("Graduation", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\321\214", nullptr));
        label_4->setText(QApplication::translate("Graduation", "\320\232\320\260\320\275\320\260\320\273 \320\234\320\220\320\235:", nullptr));
        cbManCh->setItemText(0, QApplication::translate("Graduation", "1", nullptr));
        cbManCh->setItemText(1, QApplication::translate("Graduation", "2", nullptr));
        cbManCh->setItemText(2, QApplication::translate("Graduation", "3", nullptr));
        cbManCh->setItemText(3, QApplication::translate("Graduation", "4", nullptr));
        cbManCh->setItemText(4, QApplication::translate("Graduation", "5", nullptr));
        cbManCh->setItemText(5, QApplication::translate("Graduation", "6", nullptr));
        cbManCh->setItemText(6, QApplication::translate("Graduation", "7", nullptr));
        cbManCh->setItemText(7, QApplication::translate("Graduation", "8", nullptr));
        cbManCh->setItemText(8, QApplication::translate("Graduation", "9", nullptr));

        groupBoxGrad->setTitle(QApplication::translate("Graduation", "\320\223\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\272\320\260 \320\234\320\220\320\235", nullptr));
        rbCurrent->setText(QApplication::translate("Graduation", "\320\242\320\276\320\272", nullptr));
        checkBox_5->setText(QApplication::translate("Graduation", "\357\277\274\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\270\320\265 2\320\276\320\271 \320\263\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\276\321\207\320\275\320\276\320\271 \321\202\320\276\321\207\320\272\320\270", nullptr));
        checkBox_6->setText(QApplication::translate("Graduation", "\357\277\274\320\227\320\260\320\277\320\270\321\201\321\214 \320\272\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\320\276\320\262", nullptr));
        pbStartGrad->setText(QApplication::translate("Graduation", "\320\223\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\272\320\260", nullptr));
        checkBox_1->setText(QApplication::translate("Graduation", "\320\227\320\260\320\277\320\270\321\201\321\214 \320\275\320\260\321\207\320\260\320\273\321\214\320\275\321\213\321\205 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262", nullptr));
        checkBox_3->setText(QApplication::translate("Graduation", "\357\277\274\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\270\320\265 1\320\276\320\271 \320\263\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\276\321\207\320\275\320\276\320\271 \321\202\320\276\321\207\320\272\320\270", nullptr));
        checkBox_4->setText(QApplication::translate("Graduation", "\357\277\274\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\260 2\320\276\320\271 \320\263\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\276\321\207\320\275\320\276\320\271 \321\202\320\276\321\207\320\272\320\270", nullptr));
        checkBox_2->setText(QApplication::translate("Graduation", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\260 1\320\276\320\271 \320\263\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\276\321\207\320\275\320\276\320\271 \321\202\320\276\321\207\320\272\320\270", nullptr));
        pbSave->setText(QApplication::translate("Graduation", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        rbVoltage->setText(QApplication::translate("Graduation", "\320\235\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265", nullptr));
        label->setText(QApplication::translate("Graduation", "M", nullptr));
        label_2->setText(QApplication::translate("Graduation", "S", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Graduation: public Ui_Graduation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRADUATION_H
