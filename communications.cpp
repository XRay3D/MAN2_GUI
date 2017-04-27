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
    QTimer::singleShot(100, Qt::CoarseTimer, this, &Communications::CheckConnection);
}

Communications::~Communications()
{
    qDebug() << "~Communications()";
}

void Communications::on_pbManCheckConnection_clicked()
{
    MI::man()->Ping(cbManPort->currentText());
    if (MI::man()->IsConnected()) {
        QMessageBox::information(this, "", " Связь установлена.");
        if (MI::irt()->IsConnected()) {
            emit SetTabBarEnabled(true);
        }
    }
    else {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с МАН-2!");
        emit CurrentTabIndex(3);
        emit SetTabBarEnabled(false);
    }
}

void Communications::on_pbIrtCheckConnection_clicked()
{
    MI::irt()->Ping(cbIrtPort->currentText());
    if (MI::irt()->IsConnected()) {
        QMessageBox::information(this, "", " Связь установлена.");
        if (MI::man()->IsConnected()) {
            emit SetTabBarEnabled(true);
        }
    }
    else {
        QMessageBox::critical(sender() != nullptr ? this : nullptr, "", "Не удалось установить связь с ИРТ59ХХ!");
        emit CurrentTabIndex(3);
        emit SetTabBarEnabled(false);
    }
}

void Communications::CheckConnection()
{
    MI::man()->Ping(cbManPort->currentText());
    MI::irt()->Ping(cbIrtPort->currentText());
    if (MI::man()->IsConnected() && MI::irt()->IsConnected()) {
        emit SetTabBarEnabled(true);
        return;
    }
    if (!MI::man()->IsConnected()) {
        static QMessageBox critical(0);
        critical.setIcon(QMessageBox::Critical);
        critical.setText("Не удалось установить связь с МАН-2!");
        critical.show();
    }
    if (!MI::irt()->IsConnected()) {
        static QMessageBox critical(0);
        critical.setIcon(QMessageBox::Critical);
        critical.setText("Не удалось установить связь с ИРТ59ХХ!");
        critical.show();
    }
    emit CurrentTabIndex(3);
    emit SetTabBarEnabled(false);
}
