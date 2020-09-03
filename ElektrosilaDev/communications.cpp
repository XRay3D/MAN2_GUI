#include "communications.h"
#include "hw/interface.h"
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>
#include <QTimer>

Communications::Communications(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    QVector<QString> list;
    for (QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
        list << info.portName();
    }
    std::sort(list.begin(), list.end());
    cbManPort->addItems(list.toList());
    cbManPortScpi->addItems(list.toList());
    connect(mi::man, &MAN2::detectedAddress, [this](int address) {
        comboBox->addItem(QString::number(address));
    });
    connect(comboBox, &QComboBox::currentTextChanged, [](const QString& address) {
        mi::man->setAddress(0, address.toInt());
    });

    //CheckConnection();
    //QTimer::singleShot(1, Qt::CoarseTimer, this, &Communications::CheckConnection);
}

Communications::~Communications()
{
    qDebug() << "~Communications()";
    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.beginGroup("Communications");
    settings.setValue("comboBox", comboBox->currentIndex());
}
void Communications::on_pbManCheckConnection_clicked()
{
    comboBox->clear();
    mi::man->ping(cbManPort->currentText());
    if (mi::man->IsConnected()) {
        //        comboBox->setCurrentText(QString::number(mi::man->address()));
        //QMessageBox::information(this, "", " Связь установлена.");
        //emit SetTabBarEnabled(true);
    } else {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с МАН-2!");
        emit CurrentTabIndex(0);
        //emit SetTabBarEnabled(false);
    }

    mi::scpi->ping(cbManPortScpi->currentText());
    if (mi::scpi->IsConnected()) {
        comboBox->setCurrentText(QString::number(mi::man->address()));
        //QMessageBox::information(this, "", " Связь установлена.");
        //emit SetTabBarEnabled(true);
    } else {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с Scpi!");
        emit CurrentTabIndex(0);
        //emit SetTabBarEnabled(false);
    }
}

void Communications::CheckConnection()
{
    mi::man->ping(cbManPort->currentText());
    if (mi::man->IsConnected()) {
        comboBox->setCurrentText(QString::number(mi::man->address()));
        QSettings settings("Settings.ini", QSettings::IniFormat);
        settings.beginGroup("Communications");
        comboBox->setCurrentIndex(settings.value("comboBox").toInt());
        //emit SetTabBarEnabled(true);
        return;
    } else {
        QMessageBox::critical(0, "", "Не удалось установить связь с МАН-2!");
    }
    emit CurrentTabIndex(3);
    //emit SetTabBarEnabled(false);
}

void Communications::on_pushButton_clicked()
{
    mi::man->setAddress(spinBox->value(), comboBox->currentText().toInt());
}
