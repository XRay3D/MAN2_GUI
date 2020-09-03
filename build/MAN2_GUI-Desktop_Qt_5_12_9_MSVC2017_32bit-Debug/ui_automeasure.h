/********************************************************************************
** Form generated from reading UI file 'automeasure.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOMEASURE_H
#define UI_AUTOMEASURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "tableview.h"

QT_BEGIN_NAMESPACE

class Ui_AutoMeasure
{
public:
    QGridLayout *gridLayout;
    QPushButton *pbStartStop;
    QProgressBar *progressBar;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QTableView *tvMeasure;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    TableView *tableView;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTableView *tvSerNum;

    void setupUi(QWidget *AutoMeasure)
    {
        if (AutoMeasure->objectName().isEmpty())
            AutoMeasure->setObjectName(QString::fromUtf8("AutoMeasure"));
        AutoMeasure->resize(1022, 552);
        gridLayout = new QGridLayout(AutoMeasure);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        pbStartStop = new QPushButton(AutoMeasure);
        pbStartStop->setObjectName(QString::fromUtf8("pbStartStop"));
        pbStartStop->setMinimumSize(QSize(0, 50));
        pbStartStop->setCheckable(true);

        gridLayout->addWidget(pbStartStop, 3, 0, 1, 2);

        progressBar = new QProgressBar(AutoMeasure);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 2, 0, 1, 2);

        groupBox_3 = new QGroupBox(AutoMeasure);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 6, 6);
        tvMeasure = new QTableView(groupBox_3);
        tvMeasure->setObjectName(QString::fromUtf8("tvMeasure"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tvMeasure->sizePolicy().hasHeightForWidth());
        tvMeasure->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(tvMeasure);


        gridLayout->addWidget(groupBox_3, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(AutoMeasure);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(6, 6, 6, 6);
        tableView = new TableView(groupBox_4);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout_4->addWidget(tableView);


        gridLayout->addWidget(groupBox_4, 1, 1, 1, 1);

        groupBox_2 = new QGroupBox(AutoMeasure);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        tvSerNum = new QTableView(groupBox_2);
        tvSerNum->setObjectName(QString::fromUtf8("tvSerNum"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tvSerNum->sizePolicy().hasHeightForWidth());
        tvSerNum->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(tvSerNum);


        gridLayout->addWidget(groupBox_2, 0, 0, 2, 1);

        gridLayout->setRowStretch(0, 2);
        gridLayout->setRowStretch(1, 3);
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 4);

        retranslateUi(AutoMeasure);

        QMetaObject::connectSlotsByName(AutoMeasure);
    } // setupUi

    void retranslateUi(QWidget *AutoMeasure)
    {
        AutoMeasure->setWindowTitle(QApplication::translate("AutoMeasure", "Form", nullptr));
        pbStartStop->setText(QApplication::translate("AutoMeasure", "\320\235\320\260\321\207\320\260\321\202\321\214 \320\270\320\267\320\274\320\265\321\200\320\265\320\275\320\270\321\217", nullptr));
        groupBox_3->setTitle(QApplication::translate("AutoMeasure", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\234\320\220\320\235", nullptr));
        groupBox_4->setTitle(QApplication::translate("AutoMeasure", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\321\213\320\265 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217", nullptr));
        groupBox_2->setTitle(QApplication::translate("AutoMeasure", "\320\241\320\265\321\200\320\270\320\271\320\275\321\213\320\265 \320\275\320\276\320\274\320\265\321\200\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AutoMeasure: public Ui_AutoMeasure {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOMEASURE_H
