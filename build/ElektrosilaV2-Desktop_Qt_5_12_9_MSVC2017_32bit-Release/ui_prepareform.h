/********************************************************************************
** Form generated from reading UI file 'prepareform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREPAREFORM_H
#define UI_PREPAREFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrepareForm
{
public:
    QGridLayout *gridLayout;
    QPushButton *pbStatrtMeasure;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTableView *tvSerNum;
    QPushButton *pbClearSerNum;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTableView *tvParams;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *cbxDevice;
    QLabel *label_2;
    QLineEdit *leFio;

    void setupUi(QWidget *PrepareForm)
    {
        if (PrepareForm->objectName().isEmpty())
            PrepareForm->setObjectName(QString::fromUtf8("PrepareForm"));
        PrepareForm->resize(1026, 482);
        gridLayout = new QGridLayout(PrepareForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        pbStatrtMeasure = new QPushButton(PrepareForm);
        pbStatrtMeasure->setObjectName(QString::fromUtf8("pbStatrtMeasure"));
        pbStatrtMeasure->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pbStatrtMeasure, 2, 0, 1, 3);

        groupBox_2 = new QGroupBox(PrepareForm);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        tvSerNum = new QTableView(groupBox_2);
        tvSerNum->setObjectName(QString::fromUtf8("tvSerNum"));

        verticalLayout_2->addWidget(tvSerNum);

        pbClearSerNum = new QPushButton(groupBox_2);
        pbClearSerNum->setObjectName(QString::fromUtf8("pbClearSerNum"));

        verticalLayout_2->addWidget(pbClearSerNum);


        gridLayout->addWidget(groupBox_2, 1, 1, 1, 1);

        groupBox = new QGroupBox(PrepareForm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        tvParams = new QTableView(groupBox);
        tvParams->setObjectName(QString::fromUtf8("tvParams"));

        verticalLayout->addWidget(tvParams);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(PrepareForm);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        cbxDevice = new QComboBox(PrepareForm);
        cbxDevice->setObjectName(QString::fromUtf8("cbxDevice"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cbxDevice);

        label_2 = new QLabel(PrepareForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        leFio = new QLineEdit(PrepareForm);
        leFio->setObjectName(QString::fromUtf8("leFio"));

        formLayout->setWidget(1, QFormLayout::FieldRole, leFio);


        gridLayout->addLayout(formLayout, 0, 0, 1, 3);


        retranslateUi(PrepareForm);

        QMetaObject::connectSlotsByName(PrepareForm);
    } // setupUi

    void retranslateUi(QWidget *PrepareForm)
    {
        PrepareForm->setWindowTitle(QApplication::translate("PrepareForm", "Form", nullptr));
        pbStatrtMeasure->setText(QApplication::translate("PrepareForm", "\320\235\320\260\321\207\320\260\321\202\321\214 \320\270\320\267\320\274\320\265\321\200\320\265\320\275\320\270\321\217", nullptr));
        groupBox_2->setTitle(QApplication::translate("PrepareForm", "\320\241\320\265\321\200\320\270\320\271\320\275\321\213\320\265 \320\275\320\276\320\274\320\265\321\200\320\260", nullptr));
        pbClearSerNum->setText(QApplication::translate("PrepareForm", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \321\201\320\265\321\200\320\270\320\271\320\275\321\213\320\265 \320\275\320\276\320\274\320\265\321\200\320\260", nullptr));
        groupBox->setTitle(QApplication::translate("PrepareForm", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \320\221\320\237", nullptr));
        label->setText(QApplication::translate("PrepareForm", "\320\234\320\276\320\264\320\270\321\204\320\270\320\272\320\260\321\206\320\270\321\217 \320\277\321\200\320\270\320\261\320\276\321\200\320\260:", nullptr));
        label_2->setText(QApplication::translate("PrepareForm", "\320\237\321\200\320\265\320\264\321\201\321\202\320\260\320\262\320\270\321\202\320\265\320\273\321\214 \320\236\320\242\320\232:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrepareForm: public Ui_PrepareForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREPAREFORM_H
