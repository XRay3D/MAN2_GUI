#include "maninterface.h"

#include <QSerialPortInfo>
#include <QDebug>

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

    m_port->moveToThread(&m_portThread);
    connect(&m_portThread, &QThread::finished, m_port, &QObject::deleteLater);
    connect(this, &ManInterface::Write, m_port, static_cast<qint64 (QSerialPort::*)(const QByteArray&)>(&QSerialPort::write));
    connect(this, &ManInterface::SetPortName, m_port, &SerialPort::setPortName);
    connect(this, &ManInterface::Open, m_port, &SerialPort::Open);
    connect(this, &ManInterface::Close, m_port, &SerialPort::close);
    m_portThread.start(QThread::NormalPriority);
}

ManInterface::~ManInterface()
{
    qDebug() << "~ManInterface()";
    m_portThread.quit();
    m_portThread.wait();
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

bool ManInterface::Ping(const QString& PortName)
{
    qDebug() << "Ping";
    QMutexLocker Locker(&m_mutex);
    m_connected = false;
    if (IsOpen()) {
        Close();
        thread()->msleep(10);
    }
    if (!PortName.isEmpty()) {
        emit SetPortName(PortName);
    }
    Open(QSerialPort::ReadWrite);
    if (!IsOpen()) {
        qDebug() << m_port->errorString();
        return m_connected;
    }
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(PING));
    if (m_semaphore.tryAcquire(Count, 1000)) {
        m_connected = true;
    }
    qDebug() << m_timer.elapsed();
    return m_connected;
}

bool ManInterface::GetMeasuredValue(MeasuredValue_t& value, uint8_t channel, ValueType_t type)
{
    qDebug() << "GetMeasuredValue";
    QMutexLocker Locker(&m_mutex);
    m_measuredValue.clear();
    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(GET_MEASURED_VALUE, (uint8_t*)&type, sizeof(uint8_t), channel));
    int delay[] = { 1000, 1000, 10000, 10000, 1000 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[type])) {

        value = m_value;
        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::GetMeasuredValue(QList<MeasuredValue_t>& value, uint8_t channel, ValueType_t type)
{
    qDebug() << "GetMeasuredValue";
    QMutexLocker Locker(&m_mutex);
    m_measuredValue.clear();
    value.clear();
    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(GET_MEASURED_VALUE, (uint8_t*)&type, sizeof(uint8_t), channel));
    const int delay[] = { 10000, 10000, 10000, 10000, 10000 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[type])) {
        QMapIterator<int, MeasuredValue_t> iterator(m_measuredValue);
        while (iterator.hasNext()) {
            iterator.next();
            value.insert(iterator.key() - 1, iterator.value());
        }
        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::SetCurrent(float Current, uint8_t channel)
{
    qDebug() << "SetCurrent";
    QMutexLocker Locker(&m_mutex);
    Current /= 1000.0;
    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(SET_CURRENT, (uint8_t*)&Current, sizeof(float), channel));
    const int delay[] = { 5000, 500 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[channel == 0 ? 0 : 1])) {
        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::SwitchCurrent(uint8_t Enable, uint8_t channel)
{
    qDebug() << "SwitchCurrent";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(SWITCH_CURRENT, &Enable, sizeof(uint8_t), channel));
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::TripCurrentTest()
{
    qDebug() << "TripCurrentTest";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(TRIP_CURRENT_TEST));
    if (m_semaphore.tryAcquire(1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::ShortCircuitTest(uint8_t Enable, uint8_t channel)
{
    qDebug() << "ShortCircuitTest";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(SHORT_CIRCUIT_TEST, &Enable, sizeof(uint8_t), channel));
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::Oscilloscope(uint8_t channel)
{
    qDebug() << "Oscilloscope";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(OSCILLOSCOPE, channel));
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::SetDefaultCalibrationCoefficients(uint8_t channel)
{
    qDebug() << "SetDefaultCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(SET_DEFAULT_CALIBRATION_COEFFICIENTS, channel));
    if (m_semaphore.tryAcquire(1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    qDebug() << "GetCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(GET_CALIBRATION_COEFFICIENTS, channel));
    if (m_semaphore.tryAcquire(1, 1000)) {

        GradCoeff = m_GradCoeff;
        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::SetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    qDebug() << "SetCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);
    m_GradCoeff = GradCoeff;

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(SET_CALIBRATION_COEFFICIENTS, (uint8_t*)&GradCoeff, sizeof(GradCoeff_t), channel));
    if (m_semaphore.tryAcquire(1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

bool ManInterface::SaveCalibrationCoefficients(uint8_t channel)
{
    qDebug() << "SaveCalibrationCoefficients";
    QMutexLocker Locker(&m_mutex);

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(SAVE_CALIBRATION_COEFFICIENTS, channel));
    if (m_semaphore.tryAcquire(1, 500)) {

        m_result = true;
    }
    qDebug() << m_timer.elapsed();
    return m_result;
}

void ManInterface::GetMeasuredValueSlot(ValueType_t type, uint8_t channel)
{
    qDebug() << "GetMeasuredValueSlot";
    QMutexLocker Locker(&m_mutex);

    uint8_t tmp = type;
    m_measuredValue.clear();

    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    m_timer.start();
    emit Write(Parcel(GET_MEASURED_VALUE, &tmp, sizeof(uint8_t), channel));
    const int delay[] = { 1000, 1000, 10000, 10000, 1000 };
    if (m_semaphore.tryAcquire(channel == 0 ? Count : 1, delay[type])) {
    }
    GetMeasuredValueSignal(m_measuredValue);
}

bool ManInterface::IsConnected() const
{
    return m_connected;
}

void ManInterface::CmdPing(const QByteArray& data)
{
    qDebug() << "CmdPing" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdGetMeasuredValue(const QByteArray& data)
{
    //    memcpy(&m_value, (MeasuredValue_t*)&data.constData()[5], sizeof(MeasuredValue_t));
    m_value = *(MeasuredValue_t*)(data.constData() + 5);
    switch (m_value.Type) {
    case CURRENT_MEASURED_VALUE: // Напряжение, ток и уставка.
        m_value.Value2 *= 1000.0;
        m_value.Value3 *= 1000.0;
        break;
    case VALUE_TRIP_CURRENT:
        //        m_value.Value1 = 123;
        //        m_value.Value2 = 456;
        //        m_value.Value3 = 789;
        break;
    case CALIB_VOLTAGE:
        break;
    case CALIB_CURRENT:
        //        m_value.Value1 = Ad779xSingleConversionFloat() * 0.01;
        break;
    case RAW_DATA:
        m_value.Value3 *= 1000.0;
        break;
    default:
        return;
    }
    m_measuredValue[(uint8_t)data[3]] = m_value;
    qDebug() << "CmdGetMeasuredValue" << data.toHex().toUpper() << m_value.Value1 << m_value.Value2 << m_value.Value3;
    m_semaphore.release();
}

void ManInterface::CmdSetCurrent(const QByteArray& data)
{
    qDebug() << "CmdCurrentSetting" << data.toHex().toUpper();
    //    memcpy(&m_value, (MeasuredValue_t*)&data.constData()[5], sizeof(MeasuredValue_t));
    m_value = *(MeasuredValue_t*)(data.constData() + 5);
    switch (m_value.Type) {
    case CURRENT_MEASURED_VALUE: // Напряжение, ток и уставка.
        m_value.Value2 *= 1000.0;
        m_value.Value3 *= 1000.0;
        break;
    case VALUE_TRIP_CURRENT:
        //        m_value.Value1 = 123;
        //        m_value.Value2 = 456;
        //        m_value.Value3 = 789;
        break;
    case CALIB_VOLTAGE:
        break;
    case CALIB_CURRENT:
        //        m_value.Value1 = Ad779xSingleConversionFloat() * 0.01;
        break;
    case RAW_DATA:
        m_value.Value3 *= 1000.0;
        break;
    default:
        return;
    }
    m_semaphore.release();
}

void ManInterface::CmdSwitchCurrent(const QByteArray& data)
{
    qDebug() << "CmdSwitchCurrent" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdTripCurrentTest(const QByteArray& data)
{
    qDebug() << "CmdTripCurrentTest" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdShortCircuitTest(const QByteArray& data)
{
    qDebug() << "CmdShortCircuitTest" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdOscilloscope(const QByteArray& data)
{
    qDebug() << "CmdOscilloscope" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdSetDefaultCalibrationCoefficients(const QByteArray& data)
{
    qDebug() << "CmdSetDefaultCalibrationCoefficients" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdGetCalibrationCoefficients(const QByteArray& data)
{
    qDebug() << "CmdGetCalibrationCoefficients" << data.toHex().toUpper();
    //    memcpy(&m_GradCoeff, (GradCoeff_t*)&data.constData()[5], sizeof(GradCoeff_t));
    m_GradCoeff = *(GradCoeff_t*)(data.constData() + 5);
    m_semaphore.release();
}

void ManInterface::CmdSetCalibrationCoefficients(const QByteArray& data)
{
    qDebug() << "CmdSetCalibrationCoefficients" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdSaveCalibrationCoefficients(const QByteArray& data)
{
    qDebug() << "CmdSaveCalibrationCoefficients" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdBufferOverflow(const QByteArray& data)
{
    qDebug() << "CmdBufferOverflow" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdWrongCommand(const QByteArray& data)
{
    qDebug() << "CmdWrongCommand" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdTextualParcel(const QByteArray& data)
{
    qDebug() << "CmdTextualParcel" << QString::fromLocal8Bit(data.data(), data.size()).mid(5, data.size() - 6);
    m_semaphore.release();
}

void ManInterface::CmdCrcError(const QByteArray& data)
{
    qDebug() << "CmdCrcError" << data.toHex().toUpper();
    m_semaphore.release();
}

void ManInterface::CmdNullFunction(const QByteArray& data)
{
    qDebug() << "CmdNullFunction" << data.toHex().toUpper();
    m_semaphore.release();
}

//////////////////////////////////
/// \brief SerialPort::SerialPort
/// \param manInterface
///
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
    //qDebug() << "ReadyRead()" << m_data.toHex().toUpper();
    for (int i = 0; i >= 0 && i < m_data.size() - 3; ++i) {
        if (m_data.at(i) == -86 && m_data.at(i + 1) == 85) {
            if ((i + m_data[i + 2]) <= m_data.size()) {
                m_tmpData = m_data.mid(i, m_data[i + 2]);
                if (CheckData(m_tmpData)) {
                    (m_manInterface->*m_cmdArray[*(uint8_t*)(m_tmpData.constData() + 4)])(m_tmpData);
                }
                else {
                    (m_manInterface->*m_cmdArray[CRC_ERROR])(m_tmpData);
                    qDebug() << m_data.size();
                    m_data.remove(0, m_tmpData.size());
                }
                m_data.remove(0, i + m_data[i + 2]);
                i = -1;
            }
        }
    }
}
