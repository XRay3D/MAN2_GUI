#include "communications.h"
#include "hw/interface.h"
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QTimer>

Communications::Communications(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    QVector<QString> list;
    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
        list << info.portName();
    }
    std::sort(list.begin(), list.end());
    cbManPort->addItems(list.toList());
    QTimer::singleShot(100, Qt::CoarseTimer, this, &Communications::CheckConnection);
}

Communications::~Communications()
{
    qDebug() << "~Communications()";
}

void Communications::on_pbManCheckConnection_clicked()
{
    mi::man->ping(cbManPort->currentText());
    if (mi::man->isConnected()) {
        QMessageBox::information(this, "", " Связь установлена.");
        emit SetTabBarEnabled(true);
    } else {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с МАН-2!");
        emit CurrentTabIndex(3);
        emit SetTabBarEnabled(false);
    }
}

void Communications::CheckConnection()
{
    mi::man->ping(cbManPort->currentText());
    if (mi::man->isConnected()) {
        emit SetTabBarEnabled(true);
        return;
    } else {
        QMessageBox::critical(0, "", "Не удалось установить связь с МАН-2!");
    }
    emit CurrentTabIndex(3);
    emit SetTabBarEnabled(false);
}
