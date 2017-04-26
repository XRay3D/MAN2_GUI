#include "mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_statusBarTimer(0)
    , statusBarTime(new QLineEdit(this))
{
    setupUi(this);

    //    int i = QFontDatabase::addApplicationFont(":/ubuntu-font-family-0.83/Ubuntu-R.ttf");
    //    QFont font =QFontDatabase::;
    //    font.setPointSize(10);
    //    tabWidget->setFont(font);

    //    //    QFile file1(":/protocol/Шапка.htm");
    //    //    QFile file2(":/protocol/Строка.htm");
    //    //    QFile file3(":/protocol/Подвал.htm");
    //    QFile file1(qApp->applicationDirPath().append("/").append("protocol/Шапка.htm"));
    //    QFile file2(qApp->applicationDirPath().append("/").append("protocol/Строка.htm"));
    //    QFile file3(qApp->applicationDirPath().append("/").append("protocol/Подвал.htm"));

    //    qDebug() << file1.open(QFile::ReadOnly);
    //    qDebug() << file2.open(QFile::ReadOnly);
    //    qDebug() << file3.open(QFile::ReadOnly);

    //    qDebug() << QString("%1 %2 %3").arg(1).arg("i").arg("");

    //    QString protocol;

    //    QString cap = QString().fromLocal8Bit(file1.readAll());
    //    QString row = QString().fromLocal8Bit(file2.readAll());
    //    QString basement = QString().fromLocal8Bit(file3.readAll());

    //    protocol.append(cap
    //                        .arg(1)
    //                        .arg(2)
    //                        .arg(3)
    //                        .arg(4)
    //                        .arg(5)
    //                        .arg(6)
    //                        .arg(7)
    //                        .arg(8)
    //                        .arg(9)
    //                        .arg(10)
    //                        .arg(11));

    //    for (int i = 0; i < 10; ++i) {
    //        protocol.append(row.arg(i + 1)
    //                            .arg(2)
    //                            .arg(3)
    //                            .arg(4)
    //                            .arg(5)
    //                            .arg(6)
    //                            .arg(7)
    //                            .arg(8)
    //                            .arg("; color:red")
    //                            .arg("; color:red")
    //                            .arg("; color:red")
    //                            .arg("; color:red")
    //                            .arg("; color:red")
    //                            .arg("; color:red")
    //                            .arg("; color:red"));
    //    }
    //    protocol.append(basement
    //                        .arg(QDate::currentDate().toString("dd.MM.yyyy"))
    //                        .arg("Ф.И.О."));

    //    QFile file4(qApp->applicationDirPath().append("/").append("123.htm"));
    //    qDebug() << file4.open(QFile::WriteOnly);
    //    qDebug() << file4.write(protocol.toLocal8Bit());
    //    file4.close();
    //    protocol;

    //    MyDialog* Dialog = new MyDialog(this, "123");
    //    Dialog->LoadFile(qApp->applicationDirPath().append("/").append("123.htm"));

    statusBarTime->setObjectName(QStringLiteral("statusBarTime"));
    statusBarTime->setReadOnly(true);
    statusBarTime->setAlignment(Qt::AlignCenter);
    statusBarTime->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    statusbar->addWidget(statusBarTime);

    tabCommunications->connect(tabCommunications, &Communications::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    tabCommunications->connect(tabCommunications, &Communications::SetTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);

    tabInputParameters->connect(tabInputParameters, &InputParameters::CurrentTabIndex, tabWidget, &QTabWidget::setCurrentIndex);
    tabInputParameters->connect(tabInputParameters, &InputParameters::ScanSettingsSignal, tabAutomaticMeasurements, &AutomaticMeasurements::ScanSettingsSlot);
    tabInputParameters->connect(tabInputParameters, &InputParameters::SerialNumberChanged, tabAutomaticMeasurements, &AutomaticMeasurements::SerialNumberChanged);

    tabAutomaticMeasurements->connect(tabAutomaticMeasurements, &AutomaticMeasurements::SetTabBarEnabled, tabWidget->tabBar(), &QTabBar::setEnabled);

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

    settings.beginGroup("InputParameters");
    settings.setValue("cbDevice", tabInputParameters->cbDevice->currentIndex());
    settings.setValue("leFioOtk", tabInputParameters->leFioOtk->text());
    settings.endGroup();
}
