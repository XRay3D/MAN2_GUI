#include "man2.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>

#define Dbg

enum { ChannelCount = 8 };
enum { PPP = 1 }; //9

int id1 = qRegisterMetaType<MeasuredValue_t>("MeasuredValue_t");
int id2 = qRegisterMetaType<ValuetypeEnum>("ValuetypeEnum");
int id3 = qRegisterMetaType<uint8_t>("uint8_t");
int id5 = qRegisterMetaType<QMap<int, MeasuredValue_t> >("QMap<int, MeasuredValue_t>");

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
    QMutexLocker locker(&m_mutex);
    qDebug("Ping");
    m_semaphore.acquire(m_semaphore.available());
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
        if (!m_semaphore.tryAcquire(PPP, 1000)) {
            emit Close();
            break;
        }
        m_connected = true;
        QTimer::singleShot(100, Qt::CoarseTimer, this, &MAN2::Init);
    } while (0);
    return m_connected;
}

bool MAN2::IsConnected() const
{
    m_result = false;
    m_semaphore.acquire(m_semaphore.available());
    return m_connected;
}

bool MAN2::GetMeasuredValue(MeasuredValue_t& value, uint8_t channel, ValuetypeEnum type)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
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
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
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

double MAN2::GetMeasuredValueRMS()
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        emit Write(Parcel(GET_RMS_MEASURED_VALUE, 9));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
        else
            return 0.0;
        return m_rms;
    }
    return 0.0;
}

bool MAN2::SetCurrent(float Current, uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {

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
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        emit Write(Parcel(SWITCH_CURRENT, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::TripCurrentTest()
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        emit Write(Parcel(TRIP_CURRENT_TEST));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::ShortCircuitTest(uint8_t Enable, uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        emit Write(Parcel(SHORT_CIRCUIT_TEST, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::Oscilloscope(uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        emit Write(Parcel(OSCILLOSCOPE, channel));
        if (m_semaphore.tryAcquire(ChannelCount, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::SetDefaultCalibrationCoefficients(uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        if (channel == 9) {
            m_rmsScale.AdcCh1Offset = 1.0;
            m_rmsScale.AdcCh2Offset = 0.0;
        }
        else {
            emit Write(Parcel(SET_DEFAULT_CALIBRATION_COEFFICIENTS, channel));
            if (m_semaphore.tryAcquire(1, 500))
                m_result = true;
        }
    }
    return m_result;
}

bool MAN2::GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        if (channel == 9) {
            emit Write(Parcel(GET_CALIBRATION_COEFFICIENTS, 1));
            if (m_semaphore.tryAcquire(1, 1000)) {
                m_rmsScale = GradCoeff = m_GradCoeff;
                m_result = true;
            }
        }
        else {
            emit Write(Parcel(GET_CALIBRATION_COEFFICIENTS, channel));
            if (m_semaphore.tryAcquire(1, 1000)) {
                GradCoeff = m_GradCoeff;
                m_result = true;
            }
        }
    }
    return m_result;
}

bool MAN2::SetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        if (channel == 9) {
            m_rmsScale = GradCoeff;
            emit Write(Parcel(SET_CALIBRATION_COEFFICIENTS, GradCoeff, 1));
            if (m_semaphore.tryAcquire(1, 500))
                m_result = true;
        }
        else {
            m_GradCoeff = GradCoeff;
            emit Write(Parcel(SET_CALIBRATION_COEFFICIENTS, GradCoeff, channel));
            if (m_semaphore.tryAcquire(1, 500))
                m_result = true;
        }
    }
    return m_result;
}

bool MAN2::SaveCalibrationCoefficients(uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        if (channel == 9)
            channel = 1;
        emit Write(Parcel(SAVE_CALIBRATION_COEFFICIENTS, channel));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::DisableAll()
{
    while (IsConnected()) {
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
    GradCoeff_t tmp;
    GetCalibrationCoefficients(tmp, 9);
}

void MAN2::RxPing(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "PING" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxGetMeasuredValue(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
    m_value = *(MeasuredValue_t*)(data.constData() + 5);
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
    m_measuredValue[(uint8_t)data[3]] = m_value;
#ifdef Dbg
    qDebug() << "GET_MEASURED_VALUE" /*<< data.toHex().toUpper()*/ << m_value.Value1 << m_value.Value2 << m_value.Value3;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxSetCurrent(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "SET_CURRENT" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
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

void MAN2::RxSwitchCurrent(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "SWITCH_CURRENT" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxTripCurrentTest(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "TRIP_CURRENT_TEST" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxShortCircuitTest(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "SHORT_CIRCUIT_TEST" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxOscilloscope(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "OSCILLOSCOPE" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxSetDefaultCalibrationCoefficients(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "SET_DEFAULT_CALIBRATION_COEFFICIENTS" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxGetCalibrationCoefficients(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "GET_CALIBRATION_COEFFICIENTS" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif //    memcpy(&m_GradCoeff, (GradCoeff_t*)&data.constData()[5], sizeof(GradCoeff_t));
    m_GradCoeff = *reinterpret_cast<const GradCoeff_t*>(data.constData() + 5);
    m_semaphore.release();
}

void MAN2::RxSetCalibrationCoefficients(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "SET_CALIBRATION_COEFFICIENTS" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxSaveCalibrationCoefficients(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "SAVE_CALIBRATION_COEFFICIENTS" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxGetMeasuredRmsValue(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "GET_RMS_MEASURED_VALUE" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    uint32_t tmp = *reinterpret_cast<const uint32_t*>(data.data() + 5);
    m_rms = sqrt(tmp / 400.0) * m_rmsScale.AdcCh1Offset + m_rmsScale.AdcCh2Offset;
    m_semaphore.release();
}

void MAN2::RxBufferOverflow(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "BUFFER_OVERFLOW" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxWrongCommand(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "WRONG_COMMAND" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxTextualParcel(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "TEXTUAL_PARCEL" << p->cannel << QString::fromLocal8Bit(data.data(), data.size()).mid(5, data.size() - 6);
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxCrcError(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "CRC_ERROR" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
}

void MAN2::RxNullFunction(const QByteArray& data)
{
    const Parcel_t* p = reinterpret_cast<const Parcel_t*>(data.constData());
#ifdef Dbg
    qDebug() << "RxNullFunction" << p->cannel /*<< data.toHex().toUpper()*/;
#else
    Q_UNUSED(p)
#endif
    m_semaphore.release();
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
    m_f[GET_RMS_MEASURED_VALUE] = &MAN2::RxGetMeasuredRmsValue;
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
    qDebug() << "Write" << data.toHex().toUpper();
    write(data);
}

void SerialPort::ReadyRead()
{
    QMutexLocker locker(&m_mutex);
    m_data.append(readAll());
    qDebug() << m_data.toHex().toUpper();
    for (int i = 0; i < m_data.size() - 3; ++i) {
        const Parcel_t* p = reinterpret_cast<const Parcel_t*>(m_data.constData() + i);
        if (p->start == RX) {
            if ((i + p->len) <= m_data.size()) {
                m_tmpData = m_data.mid(i, p->len);
                if (CheckData(m_tmpData))
                    (m_man->*m_f[p->cmd])(m_tmpData);
                else {
                    (m_man->*m_f[CRC_ERROR])(m_tmpData);
                    m_data.clear();
                }
                m_data.remove(0, i + p->len);
                i = -1;
            }
        }
    }
    //    QMutexLocker Locker(&m_mutex);
    //    m_data.append(readAll());
    //#ifdef DBG1
    //    qDebug() << "ReadyRead()" << uint(m_data[3]) << m_data.toHex().toUpper();
    //#endif
    //    for (int i = 0; i >= 0 && i < m_data.size() - 3; ++i) {
    //        if (m_data.at(i) == -86 && m_data.at(i + 1) == 85) {
    //            if ((i + m_data[i + 2]) <= m_data.size()) {
    //                m_tmpData = m_data.mid(i, m_data[i + 2]);
    //                if (CheckData(m_tmpData)) {
    //                    uint cmd = *(uint8_t*)(m_tmpData.constData() + 4);
    //                    switch (cmd) {
    //                    case PING:
    //                        m_man->RxPing(m_tmpData);
    //                        break;
    //                    case GET_MEASURED_VALUE:
    //                        m_man->RxGetMeasuredValue(m_tmpData);
    //                        break;
    //                    case SET_CURRENT:
    //                        m_man->RxSetCurrent(m_tmpData);
    //                        break;
    //                    case SWITCH_CURRENT:
    //                        m_man->RxSwitchCurrent(m_tmpData);
    //                        break;
    //                    case TRIP_CURRENT_TEST:
    //                        m_man->RxTripCurrentTest(m_tmpData);
    //                        break;
    //                    case SHORT_CIRCUIT_TEST:
    //                        m_man->RxShortCircuitTest(m_tmpData);
    //                        break;
    //                    case OSCILLOSCOPE:
    //                        m_man->RxOscilloscope(m_tmpData);
    //                        break;
    //                    case SET_DEFAULT_CALIBRATION_COEFFICIENTS:
    //                        m_man->RxSetDefaultCalibrationCoefficients(m_tmpData);
    //                        break;
    //                    case GET_CALIBRATION_COEFFICIENTS:
    //                        m_man->RxGetCalibrationCoefficients(m_tmpData);
    //                        break;
    //                    case SET_CALIBRATION_COEFFICIENTS:
    //                        m_man->RxSetCalibrationCoefficients(m_tmpData);
    //                        break;
    //                    case SAVE_CALIBRATION_COEFFICIENTS:
    //                        m_man->RxSaveCalibrationCoefficients(m_tmpData);
    //                        break;
    //                    case BUFFER_OVERFLOW:
    //                        m_man->RxBufferOverflow(m_tmpData);
    //                        break;
    //                    case WRONG_COMMAND:
    //                        m_man->RxWrongCommand(m_tmpData);
    //                        break;
    //                    case TEXTUAL_PARCEL:
    //                        m_man->RxTextualParcel(m_tmpData);
    //                        break;
    //                    case CRC_ERROR:
    //                        m_man->RxCrcError(m_tmpData);
    //                        break;
    //                    default:
    //                        qDebug() << "default" << cmd << m_data.size();
    //                        break;
    //                    }
    //                    //(m_manInterface->*m_cmdArray[*(uint8_t*)(m_tmpData.constData() + 4)])(m_tmpData);
    //                }
    //                else {
    //                    (m_man->*m_f[CRC_ERROR])(m_tmpData);
    //#ifdef DBG1
    //                    qDebug() << m_data.size();
    //#endif
    //                    m_data.remove(0, m_tmpData.size());
    //                }
    //                m_data.remove(0, i + m_data[i + 2]);
    //                i = -1;
    //            }
    //        }
    //    }
}
