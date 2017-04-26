#include "communications.h"
#include "measuringinterface/maninterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>

Communications::Communications(QWidget* parent)
    : QWidget(parent)
    , m_man(nullptr)
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
    if (m_man != nullptr) {
        m_man->Ping(cbManPort->currentText());
        if (!m_man->IsConnected()) {
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
}

void Communications::setMan(ManInterface* man)
{
    m_man = man;
    on_pbManCheckConnection_clicked();
}

void Communications::on_pbIrtCheckConnection_clicked()
{

}
