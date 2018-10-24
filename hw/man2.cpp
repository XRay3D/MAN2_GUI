#include "man2.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTimer>

#define Dbg 0
#define Emu 1

enum { ChannelCount = 8 };

int id1 = qRegisterMetaType<MeasuredValue_t>("MeasuredValue_t");
int id2 = qRegisterMetaType<ValuetypeEnum>("ValuetypeEnum");
int id3 = qRegisterMetaType<uint8_t>("uint8_t");
int id5 = qRegisterMetaType<QMap<int, MeasuredValue_t>>("QMap<int, MeasuredValue_t>");

MAN2::MAN2(QObject* parent)
    : QObject(parent)
{
    m_port = new SerialPort(this);
    m_port->moveToThread(&m_portThread);
    connect(&m_portThread, &QThread::finished, m_port, &QObject::deleteLater);
    connect(this, &MAN2::Open, m_port, &SerialPort::Open);
    connect(this, &MAN2::Close, m_port, &SerialPort::Close);
    connect(this, &MAN2::Write, m_port, &SerialPort::Write);
    m_portThread.start(QThread::NormalPriority);
}

MAN2::~MAN2()
{
    m_portThread.quit();
    m_portThread.wait();
}

bool MAN2::Ping(const QString& PortName)
{
    if (Emu) {
        m_connected = true;
        return true;
    }

    QMutexLocker locker(&m_mutex);
    qDebug("Ping");
    Reset();
    m_connected = false;
    do {
        emit Close();
        if (!m_semaphore.tryAcquire(1, 1000))
            break;
        if (!PortName.isEmpty())
            m_port->setPortName(PortName);
        emit Open(QIODevice::ReadWrite);
        if (!m_semaphore.tryAcquire(1, 1000))
            break;
        emit Write(Parcel(PING));
        if (!m_semaphore.tryAcquire(ChannelCount + 1, 1000)) {
            emit Close();
            break;
        }
        m_connected = true;
        QTimer::singleShot(100, Qt::CoarseTimer, this, &MAN2::Init);

    } while (0);
    return m_connected;
}

bool MAN2::GetMeasuredValue(MeasuredValue_t& value, uint8_t channel, ValuetypeEnum type)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        m_measuredValue.clear();
        emit Write(Parcel(GET_MEASURED_VALUE, static_cast<uint8_t>(type), channel));
        int delay[] = { 1000, 1000, 10000, 10000, 1000 };
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delay[type])) {
            value = m_value;
            m_result = true;
        }
    }
    return m_result;
}

bool MAN2::GetMeasuredValue(QList<MeasuredValue_t>& value, ValuetypeEnum type)
{
    if (Emu) {
        return true;
    }

    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        m_measuredValue.clear();
        value.clear();
        emit Write(Parcel(GET_MEASURED_VALUE, static_cast<uint8_t>(type), 0));
        const int delay[] = { 1000, 1000, 10000, 10000, 1000 };
        if (m_semaphore.tryAcquire(ChannelCount, delay[type])) {
            emit GetMeasuredValueSignal(m_measuredValue);
            QMapIterator<int, MeasuredValue_t> iterator(m_measuredValue);
            while (iterator.hasNext()) {
                iterator.next();
                value.insert(iterator.key() - 1, iterator.value());
            }
            m_result = true;
        }
    }
    return m_result;
}

double MAN2::GetRmsValue()
{
    if (Emu) {
        return 220.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(GetRmsMeasuredValue, 9));
        if (m_semaphore.tryAcquire(1, 500)) {
            m_result = true;
            return m_rms;
        }
    }
    return 0.0;
}

bool MAN2::SetCurrent(float Current, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        Current /= 1000.0;
        emit Write(Parcel(SET_CURRENT, Current, channel));
        const int delay[] = { 5000, 500 };
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delay[channel == 0 ? 0 : 1]))
            m_result = true;
    }
    return m_result;
}

bool MAN2::SwitchCurrent(uint8_t Enable, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(SWITCH_CURRENT, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::TripCurrentTest()
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(TRIP_CURRENT_TEST));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::ShortCircuitTest(uint8_t Enable, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(SHORT_CIRCUIT_TEST, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::Oscilloscope(uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(OSCILLOSCOPE, channel));
        if (m_semaphore.tryAcquire(ChannelCount, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::SetDefaultCalibrationCoefficients(uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(SET_DEFAULT_CALIBRATION_COEFFICIENTS, channel));
        if (m_semaphore.tryAcquire(1, 500)) {
            m_result = true;
            if (channel == 9) {
                m_rmsCoeff.AdcCh1Offset = 0.0;
                m_rmsCoeff.AdcCh2Offset = 1.0;
            }
        }
    }
    return m_result;
}

bool MAN2::GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(GET_CALIBRATION_COEFFICIENTS, channel));
        if (m_semaphore.tryAcquire(1, 1000)) {
            GradCoeff = m_coeff;
            m_result = true;
        }
    }
    return m_result;
}

bool MAN2::SetCalibrationCoefficients(const GradCoeff_t& GradCoeff, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        float data[2]{ GradCoeff.AdcCh1Offset, GradCoeff.AdcCh1Scale };
        if (channel == 9)
            emit Write(Parcel(SET_CALIBRATION_COEFFICIENTS, data, channel));
        else
            emit Write(Parcel(SET_CALIBRATION_COEFFICIENTS, GradCoeff, channel));
        if (m_semaphore.tryAcquire(1, 500)) {
            if (channel == 9)
                m_rmsCoeff = GradCoeff;
            m_result = true;
        }
    }
    return m_result;
}

bool MAN2::SaveCalibrationCoefficients(uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(Parcel(SAVE_CALIBRATION_COEFFICIENTS, channel));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::DisableAll()
{
    if (Emu) {
        return true;
    }
    while (IsConnected()) {
        Reset();
        if (!ShortCircuitTest(Off))
            break;
        if (!SwitchCurrent(Off))
            break;
        if (!SetCurrent(0.0))
            break;
        if (!Oscilloscope(Off))
            break;
        m_result = true;
        break;
    }
    return m_result;
}

void MAN2::GetMeasuredValueSlot(ValuetypeEnum type, uint8_t channel)
{
    if (Emu) {
        return;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {

        uint8_t tmp = type;
        m_measuredValue.clear();
        emit Write(Parcel(GET_MEASURED_VALUE, tmp, channel));
        const int delay[] = { 1000, 1000, 10000, 10000, 1000 };
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delay[type]))
            m_result = true;
        emit GetMeasuredValueSignal(m_measuredValue);
    }
}

void MAN2::Init()
{
    GetCalibrationCoefficients(m_rmsCoeff, 9);
    qDebug() << m_rmsCoeff.AdcCh1Offset; //-46.0562
    qDebug() << m_rmsCoeff.AdcCh1Scale; //0.402455
    //-46.3256
    //0.402617
}

void MAN2::Reset()
{
    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
}

void MAN2::RxPing(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "PING" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxGetMeasuredValue(const Parcel_t& data)
{
    m_value = data.value<MeasuredValue_t>();
    switch (m_value.Type) {
    case CURRENT_MEASURED_VALUE: // Напряжение, ток и уставка.
        m_value.Value2 *= 1000.0;
        m_value.Value3 *= 1000.0;
        break;
    case VALUE_TRIP_CURRENT:
    case CALIB_VOLTAGE:
    case CALIB_CURRENT:
        break;
    case RAW_DATA:
        m_value.Value3 *= 1000.0;
        break;
    default:
        return;
    }
    m_measuredValue[data.addres] = m_value;
    if (Dbg)
        qDebug() << "GET_MEASURED_VALUE" /*<< data.toHex().toUpper()*/ << m_value.Value1 << m_value.Value2 << m_value.Value3;
    m_semaphore.release();
}

void MAN2::RxSetCurrent(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "SET_CURRENT" << data.addres;
    m_value = data.value<MeasuredValue_t>();
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

void MAN2::RxSwitchCurrent(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "SWITCH_CURRENT" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxTripCurrentTest(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "TRIP_CURRENT_TEST" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxShortCircuitTest(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "SHORT_CIRCUIT_TEST" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxOscilloscope(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "OSCILLOSCOPE" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSetDefaultCalibrationCoefficients(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "SET_DEFAULT_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxGetCalibrationCoefficients(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "GET_CALIBRATION_COEFFICIENTS" << data.addres;
    m_coeff = data.value<GradCoeff_t>();
    m_semaphore.release();
}

void MAN2::RxSetCalibrationCoefficients(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "SET_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSaveCalibrationCoefficients(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "SAVE_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxGetRmsValue(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "GET_RMS_MEASURED_VALUE" << data.addres;
    uint32_t tmp = data.value<uint32_t>();
    m_rms = sqrt(tmp) * m_rmsCoeff.AdcCh1Scale + m_rmsCoeff.AdcCh1Offset;
    m_semaphore.release();
}

void MAN2::RxBufferOverflow(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "BUFFER_OVERFLOW" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxWrongCommand(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "WRONG_COMMAND" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxTextualParcel(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "TEXTUAL_PARCEL" << data.addres << data.text();
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxCrcError(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "CRC_ERROR" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxNullFunction(const Parcel_t& data)
{
    if (Dbg)
        qDebug() << "RxNullFunction" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

////////////////////////////////
/// \brief SerialPort::SerialPort
/// \param manInterface
///
SerialPort::SerialPort(MAN2* manInterface)
    : m_isOpen(false)
    , m_man(manInterface)
    , m_f(QVector<SerialPort::func>(0x100, &MAN2::RxNullFunction))
{
    m_f[PING] = &MAN2::RxPing;
    m_f[GET_MEASURED_VALUE] = &MAN2::RxGetMeasuredValue;
    m_f[SET_CURRENT] = &MAN2::RxSetCurrent;
    m_f[SWITCH_CURRENT] = &MAN2::RxSwitchCurrent;
    m_f[TRIP_CURRENT_TEST] = &MAN2::RxTripCurrentTest;
    m_f[SHORT_CIRCUIT_TEST] = &MAN2::RxShortCircuitTest;
    m_f[OSCILLOSCOPE] = &MAN2::RxOscilloscope;
    m_f[SET_DEFAULT_CALIBRATION_COEFFICIENTS] = &MAN2::RxSetDefaultCalibrationCoefficients;
    m_f[GET_CALIBRATION_COEFFICIENTS] = &MAN2::RxGetCalibrationCoefficients;
    m_f[SET_CALIBRATION_COEFFICIENTS] = &MAN2::RxSetCalibrationCoefficients;
    m_f[SAVE_CALIBRATION_COEFFICIENTS] = &MAN2::RxSaveCalibrationCoefficients;
    m_f[GetRmsMeasuredValue] = &MAN2::RxGetRmsValue;
    m_f[BUFFER_OVERFLOW] = &MAN2::RxBufferOverflow;
    m_f[WRONG_COMMAND] = &MAN2::RxWrongCommand;
    m_f[TEXTUAL_PARCEL] = &MAN2::RxTextualParcel;
    m_f[CRC_ERROR] = &MAN2::RxCrcError;

    setBaudRate(QSerialPort::Baud57600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &SerialPort::ReadyRead);
}

SerialPort::~SerialPort() {}

void SerialPort::Open(int mode)
{
    if (open(static_cast<OpenMode>(mode)))
        m_man->m_semaphore.release();
}

void SerialPort::Close()
{
    close();
    m_man->m_semaphore.release();
}

void SerialPort::Write(const QByteArray& data)
{
    write(data);
}

void SerialPort::ReadyRead()
{
    QMutexLocker locker(&m_mutex);
    m_data.append(readAll());
    for (int i = 0; i < m_data.size() - 3; ++i) {
        const Parcel_t* const parcel = reinterpret_cast<const Parcel_t*>(m_data.constData() + i);
        if (parcel->start == RX) {
            if ((parcel->length + i) <= m_data.size()) {
                m_tmpData = m_data.mid(i, parcel->length);
                if (CheckData(m_tmpData))
                    (m_man->*m_f[parcel->command])(*parcel);
                else {
                    (m_man->*m_f[CRC_ERROR])(*parcel);
                    m_data.clear();
                }
                m_data.remove(0, i + parcel->length);
                i = -1;
            }
        }
    }
}
