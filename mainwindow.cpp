#include "mainwindow.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>

#include "preparation/sernummodel.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_statusBarTimer(0)
    , statusBarTime(new QLineEdit(this))
{
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
    //    connect(tabInputParameters, &Preparation::ScanSettingsSignal, tabAutomaticMeasurements, &AutomaticMeasurements::ScanSettingsSlot);
    //    connect(tabInputParameters, &Preparation::SerialNumberChanged, tabAutomaticMeasurements, &AutomaticMeasurements::SerialNumberChanged);
    connect(tabAutomaticMeasurements, &AutomaticMeasurements::SetTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);

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
    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    settings.beginGroup("Communications");
    tabCommunications->cbManPort->setCurrentIndex(settings.value("cbManPort").toInt());
    settings.endGroup();

    settings.beginGroup("Graduation");
    tabGraduation->cbxScpi->setCurrentIndex(settings.value("cbxScpi").toInt());
    settings.endGroup();

    settings.beginGroup("InputParameters");
    tabInputParameters->cbxDevice->setCurrentIndex(settings.value("cbDevice", 0).toInt());
    tabInputParameters->leFio->setText(settings.value("leFioOtk", "Ф.И.О.").toString());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("Settings.ini", QSettings::IniFormat);

    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("Communications");
    settings.setValue("cbManPort", tabCommunications->cbManPort->currentIndex());
    settings.endGroup();

    settings.beginGroup("Graduation");
    settings.setValue("cbxScpi", tabGraduation->cbxScpi->currentIndex());
    settings.endGroup();

    settings.beginGroup("InputParameters");
    settings.setValue("cbDevice", tabInputParameters->cbxDevice->currentIndex());
    settings.setValue("leFioOtk", tabInputParameters->leFio->text());
    settings.endGroup();
}
