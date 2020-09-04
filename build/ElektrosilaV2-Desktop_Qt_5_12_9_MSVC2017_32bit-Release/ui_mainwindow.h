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
#include "auto/automeasure.h"
#include "communications.h"
#include "measurements.h"
#include "preparation/prepareform.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    PrepareForm *tabInputParameters;
    AutoMeasure *tabAutomaticMeasurements;
    Measurements *tabMeasurements;
    Communications *tabCommunications;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(857, 443);
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
        tabInputParameters = new PrepareForm();
        tabInputParameters->setObjectName(QString::fromUtf8("tabInputParameters"));
        tabWidget->addTab(tabInputParameters, QString());
        tabAutomaticMeasurements = new AutoMeasure();
        tabAutomaticMeasurements->setObjectName(QString::fromUtf8("tabAutomaticMeasurements"));
        tabWidget->addTab(tabAutomaticMeasurements, QString());
        tabMeasurements = new Measurements();
        tabMeasurements->setObjectName(QString::fromUtf8("tabMeasurements"));
        tabWidget->addTab(tabMeasurements, QString());
        tabCommunications = new Communications();
        tabCommunications->setObjectName(QString::fromUtf8("tabCommunications"));
        tabWidget->addTab(tabCommunications, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 857, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\255\320\273\320\265\320\272\321\202\321\200\320\276\321\201\320\270\320\273\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabInputParameters), QApplication::translate("MainWindow", "\320\222\320\262\320\276\320\264 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAutomaticMeasurements), QApplication::translate("MainWindow", "\320\220\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \320\270\320\267\320\274\320\265\321\200\320\265\320\275\320\270\321\217", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMeasurements), QApplication::translate("MainWindow", "\320\240\321\203\321\207\320\275\320\276\320\265 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCommunications), QApplication::translate("MainWindow", "\320\241\320\262\321\217\320\267\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
