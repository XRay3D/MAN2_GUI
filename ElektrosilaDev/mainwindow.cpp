#include "mainwindow.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>

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

    connect(mi::man, &MAN2::detectedAddress, [this](int address) {
        if (static QSet<int> set; !set.contains(address)) {
            set.insert(address);
            cbxAddress->addItem(QString::number(address));
        }
    });
    connect(cbxAddress, &QComboBox::currentTextChanged, [](const QString& address) {
        mi::man->address() = address.toInt();
    });

    //    connect(tabInputParameters, &PrepareForm::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    //    connect(tabInputParameters, &Preparation::ScanSettingsSignal, tabAutomaticMeasurements, &AutomaticMeasurements::ScanSettingsSlot);
    //    connect(tabInputParameters, &Preparation::SerialNumberChanged, tabAutomaticMeasurements, &AutomaticMeasurements::SerialNumberChanged);
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
    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();

    settings.beginGroup("Communications");
    tabCommunications->cbManPortScpi->setCurrentIndex(settings.value("cbxScpi").toInt());
    tabCommunications->cbManPort->setCurrentIndex(settings.value("cbManPort").toInt());
    tabCommunications->CheckConnection();
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup("MainWindow");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();

    settings.beginGroup("Communications");
    settings.setValue("cbxScpi", tabCommunications->cbManPortScpi->currentIndex());
    settings.setValue("cbManPort", tabCommunications->cbManPort->currentIndex());
    settings.endGroup();
}

void MainWindow::on_pushButton_clicked()
{
    mi::man->setAddress(cbxAddress->currentText().toInt(), spinBox->value());
}
