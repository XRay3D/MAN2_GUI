#include "mainwindow.h"

#include "settings.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QSerialPortInfo>

#include "devicemodel.h"
#include "sernummodel.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_statusBarTimer(0)
    , statusBarTime(new QLineEdit(this))
{
    new DeviceModel(this);
    new SerNumModel(this);

    setupUi(this);

    statusBarTime->setObjectName(QStringLiteral("statusBarTime"));
    statusBarTime->setReadOnly(true);
    statusBarTime->setAlignment(Qt::AlignCenter);
    statusBarTime->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    statusbar->addWidget(statusBarTime);

    connect(tabCommunications, &Communications::currentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    connect(tabCommunications, &Communications::setTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);
    connect(tabInputParameters, &PrepareForm::currentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);

    readSettings();

    m_statusBarTimer = startTimer(1000);
}

MainWindow::~MainWindow()
{
    writeSettings();
    m_manThread.quit();
    m_manThread.wait();
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_statusBarTimer) {
        statusBarTime->setText(QTime::currentTime().toString());
    }
}

void MainWindow::readSettings()
{
    MySettings settings;
    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    settings.getValueGs(this);
    settings.getValueGs(tabInputParameters->splitter);
    settings.getValueGs(tabInputParameters->splitter_2);
    settings.endGroup();

    settings.beginGroup("InputParameters");
    settings.getValue(tabInputParameters->cbxDevice, 0);
    settings.getValue(tabInputParameters->leFio, "Ф.И.О.");
    settings.endGroup();

    settings.beginGroup("Communications");
    settings.getValue(tabCommunications->cbManPort);
    tabCommunications->checkConnection();
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    MySettings settings;
    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    settings.setValueGs(this);
    settings.setValueGs(tabInputParameters->splitter);
    settings.setValueGs(tabInputParameters->splitter_2);
    settings.endGroup();

    settings.beginGroup("InputParameters");
    settings.setValue(tabInputParameters->cbxDevice);
    settings.setValue(tabInputParameters->leFio);
    settings.endGroup();

    settings.beginGroup("Communications");
    settings.setValue(tabCommunications->cbManPort);
    settings.endGroup();
}
