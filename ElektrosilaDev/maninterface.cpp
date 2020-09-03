#include "maninterface.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>

#define Count 8

int id1 = qRegisterMetaType<MeasuredValue_t>("MeasuredValue_t");
int id2 = qRegisterMetaType<ValueType_t>("ValueType_t");
int id3 = qRegisterMetaType<uint8_t>("uint8_t");
int id5 = qRegisterMetaType<QMap<int, MeasuredValue_t> >("QMap<int, MeasuredValue_t>");

ManInterface::ManInterface(QObject* parent)
    : QObject(parent)
    , m_port(new SerialPort(this))
    , m_semaphore(0)
    , m_connected(false)
{
    for (int i = 0; i < 0x100; ++i) {
        m_port->m_cmdArray[i] = &ManInterface::CmdNullFunction;
    }
    m_port->m_cmdArray[PING] = &ManInterface::CmdPing;
    m_port->m_cmdArray[GET_MEASURED_VALUE] = &ManInterface::CmdGetMeasuredValue;
    m_port->m_cmdArray[SET_CURRENT] = &ManInterface::CmdSetCurrent;
    m_port->m_cmdArray[SWITCH_CURRENT] = &ManInterface::CmdSwitchCurrent;
    m_port->m_cmdArray[TRIP_CURRENT_TEST] = &ManInterface::CmdTripCurrentTest;
    m_port->m_cmdArray[SHORT_CIRCUIT_TEST] = &ManInterface::CmdShortCircuitTest;
    m_port->m_cmdArray[OSCILLOSCOPE] = &ManInterface::CmdOscilloscope;
    m_port->m_cmdArray[SET_DEFAULT_CALIBRATION_COEFFICIENTS] = &ManInterface::CmdSetDefaultCalibrationCoefficients;
    m_port->m_cmdArray[GET_CALIBRATION_COEFFICIENTS] = &ManInterface::CmdGetCalibrationCoefficients;
    m_port->m_cmdArray[SET_CALIBRATION_COEFFICIENTS] = &ManInterface::CmdSetCalibrationCoefficients;
    m_port->m_cmdArray[SAVE_CALIBRATION_COEFFICIENTS] = &ManInterface::CmdSaveCalibrationCoefficients;
    m_port->m_cmdArray[BUFFER_OVERFLOW] = &ManInterface::CmdBufferOverflow;
    m_port->m_cmdArray[WRONG_COMMAND] = &ManInterface::CmdWrongCommand;
    m_port->m_cmdArray[TEXTUAL_PARCEL] = &ManInterface::CmdTextualParcel;
    m_port->m_cmdArray[CRC_ERROR] = &ManInterface::CmdCrcError;

    m_port->moveToThread(&m_thread);
    //    connect(&m_thread, &QThread::finished, m_port, &QObject::deleteLater);
    connect(this, &ManInterface::Write, m_port, static_cast<qint64 (QSerialPort::*)(const QByteArray&)>(&QSerialPort::write));
    connect(this, &ManInterface::SetPortName, m_port, &SerialPort::setPortName);
    connect(this, &ManInterface::Open, m_port, &SerialPort::Open);
    connect(this, &ManInterface::Close, m_port, &SerialPort::close);
    m_thread.start(QThread::NormalPriority);
}

ManInterface::~ManInterface()
{
    //    qDebug() << "~ManInterface()";
    m_thread.quit();
    m_thread.wait();
}

void ManInterface::EnumeratePorts(QComboBox* comboBox)
{
    QList<QString> portNameList;
    foreach (QSerialPortInfo portInfo, QSerialPortInfo::availablePorts()) {
        portNameList.append(portInfo.portName());
    }
    qSort(portNameList);
    comboBox->addItems(portNameList);
}

bool ManInterface::IsOpen()
{
    thread()->msleep(10);
    return m_port->m_isOpen;
}

QList<QString> ManInterface::Ping(const QString& PortName)
{
    //    qDebug() << "Ping";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    //m_timer.start();
    m_connected = false;
    m_counter = 0;
    QList<QString> list;
    m_semaphore.acquire(m_semaphore.available());
    m_channels.clear();
    if (IsOpen()) {
        Close();
        thread()->msleep(10);
    }
    if (!PortName.isEmpty()) {
        emit SetPortName(PortName);
    }
    Open(QSerialPort::ReadWrite);
    if (!IsOpen()) {
        //    qDebug() << m_port->errorString();
        return list;
    }
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(PING));
    if (m_semaphore.tryAcquire(Count, 1000)) {
        m_semaphore.acquire(m_semaphore.available());
        m_channels.append(0);
        qSort(m_channels);
        foreach (int i, m_channels) {
            list.append(QString().setNum(i));
        }
        m_connected = true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return list;
}

bool ManInterface::GetMeasuredValue(MeasuredValue_t& value, uint8_t channel, ValueType_t type)
{
    //    qDebug() << "GetMeasuredValue";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    uint8_t tmp = type;
    m_measuredValue.clear();
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(GET_MEASURED_VALUE, &tmp, sizeof(uint8_t), channel));
    int delay[] = { 1000, 1000, 10000, 10000, 1000 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[type])) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        value = m_value;
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

QList<MeasuredValue_t> ManInterface::GetMeasuredValue(ValueType_t type, uint8_t channel)
{
    //    qDebug() << "GetMeasuredValue";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    uint8_t tmp = type;
    m_measuredValue.clear();
    m_list.clear();
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(GET_MEASURED_VALUE, &tmp, sizeof(uint8_t), channel));
    const int delay[] = { 10000, 10000, 10000, 10000, 10000 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[type])) {
        m_semaphore.acquire(m_semaphore.available());
        QMapIterator<int, MeasuredValue_t> iterator(m_measuredValue);
        while (iterator.hasNext()) {
            iterator.next();
            qDebug() << "key(" << iterator.key() << ") = " << iterator.value().Value1;
            m_list.insert(iterator.key() - 1, iterator.value());
        }
        //        for (int i = 1; i < m_measuredValue.size(); ++i) {
        //            list.append(m_measuredValue[i]);
        //        }
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return m_list;
}

bool ManInterface::SetCurrent(float Current, uint8_t channel)
{
    //    qDebug() << "SetCurrent";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(SET_CURRENT, (uint8_t*)&Current, sizeof(float), channel));
    const int delay[] = { 5000, 500 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[channel == 0 ? 0 : 1])) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::SwitchCurrent(uint8_t Enable, uint8_t channel)
{
    //    qDebug() << "SwitchCurrent";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(SWITCH_CURRENT, &Enable, sizeof(uint8_t), channel));
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::TripCurrentTest()
{
    //    qDebug() << "TripCurrentTest";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(TRIP_CURRENT_TEST));
    if (m_semaphore.tryAcquire(1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::ShortCircuitTest(uint8_t Enable, uint8_t channel)
{
    //    qDebug() << "ShortCircuitTest";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(SHORT_CIRCUIT_TEST, &Enable, sizeof(uint8_t), channel));
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::Oscilloscope(uint8_t channel)
{
    //    qDebug() << "Oscilloscope";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(OSCILLOSCOPE, channel));
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::SetDefaultCalibrationCoefficients(uint8_t channel)
{
    //    qDebug() << "SetDefaultCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(SET_DEFAULT_CALIBRATION_COEFFICIENTS, channel));
    if (m_semaphore.tryAcquire(1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    //    qDebug() << "GetCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(GET_CALIBRATION_COEFFICIENTS, channel));
    if (m_semaphore.tryAcquire(1, 1000)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        GradCoeff = m_GradCoeff;
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::SetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    //    qDebug() << "SetCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_GradCoeff = GradCoeff;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(SET_CALIBRATION_COEFFICIENTS, (uint8_t*)&GradCoeff, sizeof(GradCoeff_t), channel));
    if (m_semaphore.tryAcquire(1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

bool ManInterface::SaveCalibrationCoefficients(uint8_t channel)
{
    //    qDebug() << "SaveCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(SAVE_CALIBRATION_COEFFICIENTS, channel));
    if (m_semaphore.tryAcquire(1, 500)) {
        m_semaphore.acquire(m_semaphore.available());
        //    qDebug() << m_counter << m_timer.elapsed();
        return true;
    }
    //    qDebug() << m_counter << m_timer.elapsed();
    return false;
}

void ManInterface::GetMeasuredValueSlot(ValueType_t type, uint8_t channel)
{
    //    qDebug() << "GetMeasuredValueSlot";
    QMutexLocker Locker(&m_mutex);
    //m_timer.start();
    m_counter = 0;
    uint8_t tmp = type;
    m_measuredValue.clear();
    m_semaphore.acquire(m_semaphore.available());
    emit Write(Parcel(GET_MEASURED_VALUE, &tmp, sizeof(uint8_t), channel));
    const int delay[] = { 1000, 1000, 10000, 10000, 1000 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[type])) {
        m_semaphore.acquire(m_semaphore.available());

        //        for (int i = 1; i < m_measuredValue.size(); ++i) {
        //            list.append(m_measuredValue[i]);
        //        }
    }
    GetMeasuredValueSignal(m_measuredValue);
}

bool ManInterface::IsConnected() const
{
    return m_connected;
}

void ManInterface::CmdPing(const QByteArray& data)
{
    //qDebug() << ++counter << "CmdPing" << data.toHex().toUpper();
    m_channels.append((uint8_t)data[3]);
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdGetMeasuredValue(const QByteArray& data)
{
    memcpy(&m_value, (MeasuredValue_t*)&data.constData()[5], sizeof(MeasuredValue_t));
    m_measuredValue[(uint8_t)data[3]] = m_value;
    //qDebug() << "CmdGetMeasuredValue" << data.size() << sizeof(MeasuredValue_t);
    //    emit GetMeasuredValueSignal(m_value, (uint8_t)data[3]);
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdSetCurrent(const QByteArray& data)
{
    //qDebug() << ++counter << "CmdCurrentSetting" << data.toHex().toUpper();
    memcpy(&m_value, (MeasuredValue_t*)&data.constData()[5], sizeof(MeasuredValue_t));
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdSwitchCurrent(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdSwitchCurrent" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdTripCurrentTest(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdTripCurrentTest" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdShortCircuitTest(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdShortCircuitTest" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdOscilloscope(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdOscilloscope" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdSetDefaultCalibrationCoefficients(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdSetDefaultCalibrationCoefficients" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdGetCalibrationCoefficients(const QByteArray& data)
{
    //qDebug() << ++counter << "CmdGetCalibrationCoefficients" << data.toHex().toUpper();
    memcpy(&m_GradCoeff, (GradCoeff_t*)&data.constData()[5], sizeof(GradCoeff_t));
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdSetCalibrationCoefficients(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdSetCalibrationCoefficients" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdSaveCalibrationCoefficients(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdSaveCalibrationCoefficients" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdBufferOverflow(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdBufferOverflow" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdWrongCommand(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdWrongCommand" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdTextualParcel(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdTextualParcel" << QString::fromLocal8Bit(data.data(), data.size()).mid(5, data.size() - 6);
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdCrcError(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdCrcError" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

void ManInterface::CmdNullFunction(const QByteArray& data)
{
    Q_UNUSED(data)
    //qDebug() << ++counter << "CmdNullFunction" << data.toHex().toUpper();
    ++m_counter;
    m_semaphore.release(1);
}

SerialPort::SerialPort(ManInterface* manInterface)
    : m_isOpen(false)
    , m_manInterface(manInterface)
{
    setBaudRate(QSerialPort::Baud57600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &SerialPort::ReadyRead);
}

SerialPort::~SerialPort() {}

bool SerialPort::Open(int mode)
{
    m_isOpen = open((QSerialPort::OpenMode)mode);
    return m_isOpen;
}

void SerialPort::ReadyRead()
{
    QMutexLocker Locker(&m_mutex);
    m_data.append(readAll());
    for (int i = 0; i >= 0 && i < m_data.size() - 3; ++i) {
        if (m_data.at(i) == -86 && m_data.at(i + 1) == 85) {
            if ((i + m_data[i + 2]) <= m_data.size()) {
                m_tmpData = m_data.mid(i, m_data[i + 2]);
                if (CheckData(m_tmpData)) {
                    (m_manInterface->*m_cmdArray[*(uint8_t*)(m_tmpData.constData() + 4)])(m_tmpData);
                }
                else {
                    (m_manInterface->*m_cmdArray[CRC_ERROR])(m_tmpData);
                }
                m_data.remove(0, i + m_data[i + 2]);
                i = -1;
            }
        }
    }
}
