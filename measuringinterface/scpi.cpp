#include "scpi.h"
#include <QDebug>

SCPI::SCPI(QObject* parent)
    : QSerialPort(parent)
    , m_isFound(false)
{
    setBaudRate(QSerialPort::Baud9600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
}

bool SCPI::Ping(const QString& potName)
{
    if (isOpen()) {
        close();
    }
    m_isFound = false;
    setPortName(potName);
    if (open(ReadWrite)) {
        setDataTerminalReady(true);
        setRequestToSend(false);
        qDebug() << WriteRead("*IDN?\r\n");
        if (m_data.contains('\r') && m_data.contains('\n')) {
            qDebug() << WriteRead("SYSTem:REMote\r\n", 1);
            qDebug() << WriteRead("*RST\r\n", 1);
            qDebug() << WriteRead("*CLS\r\n", 1);
            m_isFound = true;
        }
    }
    return m_isFound;
}

QByteArray& SCPI::WriteRead(const QByteArray& data, int timeout)
{
    m_data.clear();
    m_counter = 0;
    write(data);
    while (++m_counter < timeout && !(m_data.contains('\n') && m_data.contains('\n'))) {
        waitForReadyRead(100);
        m_data.append(readAll());
    }
    return m_data;
}

bool SCPI::IsConnected() const
{
    return m_isFound;
}

double SCPI::GetVoltage()
{
    WriteRead("SYSTem:REMote\r\n", 1);
    WriteRead("DETector:BANDwidth 3\r\n");
    WriteRead("MEASure:VOLTage:DC?\r\n");
    if (m_data.contains('\r') && m_data.contains('\n')) {
        return QString(m_data).toDouble();
    }
    return 0.0;
}

double SCPI::GetCurrent()
{
    WriteRead("SYSTem:REMote\r\n", 1);
    WriteRead("DETector:BANDwidth 3\r\n");
    WriteRead("MEASure:CURRent:DC?\r\n");
    if (m_data.contains('\r') && m_data.contains('\n')) {
        return QString(m_data).toDouble();
    }
    return 0.0;
}
