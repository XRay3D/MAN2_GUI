#include "communications.h"
#include "measuringinterface/measuringinterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>

Communications::Communications(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    QStringList list;
    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
        list << info.portName();
    }
    qSort(list);
    cbManPort->addItems(list);
    cbIrtPort->addItems(list);
}

Communications::~Communications()
{
    qDebug() << "~Communications()";
}

void Communications::on_pbManCheckConnection_clicked()
{
    MI::man()->Ping(cbManPort->currentText());
    if (!MI::man()->IsConnected()) {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с МАН-2!");
        emit CurrentTabIndex(3);
        emit SetTabBarEnabled(false);
    }
    else {
        if (sender() != nullptr) {
            QMessageBox::information(this, "", " Связь установлена.");
        }
        emit SetTabBarEnabled(true);
    }
}

void Communications::on_pbIrtCheckConnection_clicked()
{
    MI::irt()->Ping(cbIrtPort->currentText());
    if (!MI::irt()->IsConnected()) {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с ИРТ59ХХ!");
        emit CurrentTabIndex(3);
        emit SetTabBarEnabled(false);
    }
    else {
        if (sender() != nullptr) {
            QMessageBox::information(this, "", " Связь установлена.");
        }
        emit SetTabBarEnabled(true);
    }
}
