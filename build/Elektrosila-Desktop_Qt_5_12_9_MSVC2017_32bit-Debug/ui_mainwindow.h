/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "communications.h"
#include "graduation.h"
#include "lintest.h"
#include "measurements.h"
#include "triptest.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    Communications *tabCommunications;
    Measurements *tabMeasurements;
    TripTest *tab_2;
    LinTest *tab;
    Graduation *tabGraduation;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(696, 569);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabsClosable(false);
        tabWidget->setTabBarAutoHide(false);
        tabCommunications = new Communications();
        tabCommunications->setObjectName(QString::fromUtf8("tabCommunications"));
        tabWidget->addTab(tabCommunications, QString());
        tabMeasurements = new Measurements();
        tabMeasurements->setObjectName(QString::fromUtf8("tabMeasurements"));
        tabWidget->addTab(tabMeasurements, QString());
        tab_2 = new TripTest();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        tab = new LinTest();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tabGraduation = new Graduation();
        tabGraduation->setObjectName(QString::fromUtf8("tabGraduation"));
        tabWidget->addTab(tabGraduation, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 696, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\255\320\273\320\265\320\272\321\202\321\200\320\276\321\201\320\270\320\273\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCommunications), QApplication::translate("MainWindow", "\320\241\320\262\321\217\320\267\321\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMeasurements), QApplication::translate("MainWindow", "\320\240\321\203\321\207\320\275\320\276\320\265 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "TripTest", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "LinTest", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGraduation), QApplication::translate("MainWindow", "\320\223\321\200\320\260\320\264\321\203\320\270\321\200\320\276\320\262\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
