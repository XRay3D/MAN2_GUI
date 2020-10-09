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
    for (auto& info : QSerialPortInfo::availablePorts()) {
        list << info.portName();
    }
    std::sort(list.begin(), list.end());
    cbManPort->addItems(list.toList());
    connect(pbCheckConnection, &QPushButton::clicked, this, &Communications::checkConnection);
}

void Communications::checkConnection()
{
    mi::man->ping(cbManPort->currentText());
    if (mi::man->isConnected()) {
        mi::osc->ping();
        if (mi::osc->isConnected()) {
            leOscIdn->setText(mi::osc->idn());
            emit setTabBarEnabled(true);
            emit currentTabIndex(0);
            return;
        } else {
            QMessageBox::critical(0, "", "Не удалось установить связь с Digital Osc!");
        }
    } else {
        QMessageBox::critical(0, "", "Не удалось установить связь с МАН-2!");
    }
    emit currentTabIndex(2);
    emit setTabBarEnabled(false);
}
