#include "mainwindow.h"

#include <QSettings>
#include <QMessageBox>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_statusBarTimer(0)
    , statusBarTime(new QLineEdit(this))
{
    setupUi(this);

    statusBarTime->setObjectName(QStringLiteral("statusBarTime"));
    statusBarTime->setReadOnly(true);
    statusBarTime->setAlignment(Qt::AlignCenter);
    statusBarTime->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    statusbar->addWidget(statusBarTime);

    connect(tabCommunications, &Communications::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    connect(tabCommunications, &Communications::SetTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);

    connect(tabInputParameters, &InputParameters::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    connect(tabInputParameters, &InputParameters::ScanSettingsSignal, tabAutomaticMeasurements, &AutomaticMeasurements::ScanSettingsSlot);
    connect(tabInputParameters, &InputParameters::SerialNumberChanged, tabAutomaticMeasurements, &AutomaticMeasurements::SerialNumberChanged);

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
    tabInputParameters->leFioOtk->setText(settings.value("leFioOtk", "Ф.И.О.").toString());
    QFile file("modify.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, "", "Не найден файл \"modify.txt\" с параметрами блоков питания!");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        tabInputParameters->m_listScanSettings.append(new ScanSettings(in.readLine().split(';')));
        tabInputParameters->cbDevice->addItem(tabInputParameters->m_listScanSettings.last()->Type + " (" + tabInputParameters->m_listScanSettings.last()->Cipher + ")");
    }
    tabInputParameters->cbDevice->setCurrentIndex(settings.value("cbDevice", 0).toInt());

    tabInputParameters->leSerialNumberDevice_1->setText(settings.value("leSerialNumberDevice_1", "").toString());
    tabInputParameters->leSerialNumberDevice_2->setText(settings.value("leSerialNumberDevice_2", "").toString());
    tabInputParameters->leSerialNumberDevice_3->setText(settings.value("leSerialNumberDevice_3", "").toString());
    tabInputParameters->leSerialNumberDevice_4->setText(settings.value("leSerialNumberDevice_4", "").toString());
    tabInputParameters->leSerialNumberDevice_5->setText(settings.value("leSerialNumberDevice_5", "").toString());
    tabInputParameters->leSerialNumberDevice_6->setText(settings.value("leSerialNumberDevice_6", "").toString());
    tabInputParameters->leSerialNumberDevice_7->setText(settings.value("leSerialNumberDevice_7", "").toString());
    tabInputParameters->leSerialNumberDevice_8->setText(settings.value("leSerialNumberDevice_8", "").toString());
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
    settings.setValue("cbDevice", tabInputParameters->cbDevice->currentIndex());
    settings.setValue("leFioOtk", tabInputParameters->leFioOtk->text());

    settings.setValue("leSerialNumberDevice_1", tabInputParameters->leSerialNumberDevice_1->text());
    settings.setValue("leSerialNumberDevice_2", tabInputParameters->leSerialNumberDevice_2->text());
    settings.setValue("leSerialNumberDevice_3", tabInputParameters->leSerialNumberDevice_3->text());
    settings.setValue("leSerialNumberDevice_4", tabInputParameters->leSerialNumberDevice_4->text());
    settings.setValue("leSerialNumberDevice_5", tabInputParameters->leSerialNumberDevice_5->text());
    settings.setValue("leSerialNumberDevice_6", tabInputParameters->leSerialNumberDevice_6->text());
    settings.setValue("leSerialNumberDevice_7", tabInputParameters->leSerialNumberDevice_7->text());
    settings.setValue("leSerialNumberDevice_8", tabInputParameters->leSerialNumberDevice_8->text());
    settings.endGroup();
}
