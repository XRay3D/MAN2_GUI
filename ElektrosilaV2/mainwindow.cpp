#include "mainwindow.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>

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

    connect(tabCommunications, &Communications::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    connect(tabCommunications, &Communications::SetTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);
    connect(tabInputParameters, &PrepareForm::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    //    connect(tabAutomaticMeasurements, &AutoMeasure::SetTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);

    readSettings();

    m_statusBarTimer = startTimer(1000);
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow()";
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
    QSettings settings;
    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();

    settings.beginGroup("InputParameters");
    tabInputParameters->cbxDevice->setCurrentIndex(settings.value("cbDevice", 0).toInt());
    tabInputParameters->leFio->setText(settings.value("leFioOtk", "Ф.И.О.").toString());
    settings.endGroup();

    settings.beginGroup("Communications");
    tabCommunications->cbManPort->setCurrentIndex(settings.value("cbManPort").toInt());
    tabCommunications->CheckConnection();
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();

    settings.beginGroup("InputParameters");
    settings.setValue("cbDevice", tabInputParameters->cbxDevice->currentIndex());
    settings.setValue("leFioOtk", tabInputParameters->leFio->text());
    settings.endGroup();

    settings.beginGroup("Communications");
    settings.setValue("cbManPort", tabCommunications->cbManPort->currentIndex());
    settings.endGroup();
}
