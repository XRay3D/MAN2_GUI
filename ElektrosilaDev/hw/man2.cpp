#include "man2.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTimer>

#define Dbg 0

enum { ChannelCount = 9 };

const int id1 = qRegisterMetaType<MeasuredValue>("MeasuredValue");
const int id2 = qRegisterMetaType<ValueType>("ValueType");
const int id3 = qRegisterMetaType<uint8_t>("uint8_t");
const int id5 = qRegisterMetaType<QMap<int, MeasuredValue>>("QMap<int, MeasuredValue>");
const int id6 = qRegisterMetaType<MeasuredValue>("MeasuredValue");

class SerialPort : public QSerialPort, private MyProtokol {
public:
    SerialPort(MAN2* manInterface);
    ~SerialPort();
    void Open(int mode);
    void Close();
    void Write(const QByteArray& data);

    bool m_isOpen;

    MAN2* m_man;
    //    typedef void (MAN2::*pFunc)(const QByteArray&);
    //    pFunc m_f[0x100];
    //    void (MAN2::*m_dataReady)(const QByteArray&);
    typedef void (MAN2::*func)(const Parcel&);
    QVector<func> m_f;

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
};

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

bool MAN2::ping(const QString& PortName)
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
        emit Write(createParcel(Ping, 0));
        if (!m_semaphore.tryAcquire(ChannelCount /*+ 1*/, 2000)) { /////////////////////////////// with rms
            emit Close();
            break;
        }
        m_connected = true;
        //QTimer::singleShot(100, Qt::CoarseTimer, this, &MAN2::Init);

    } while (0);
    return m_connected;
}

bool MAN2::getMeasuredValue(MeasuredValue& value, uint8_t channel, ValueType type)
{
    if (Emu) {
        m_measuredValue.clear();
        value.value1 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
        value.value2 = 12.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
        value.value3 = 6.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
        m_measuredValue[channel] = value;
        //GetMeasuredValueSignal(m_measuredValue);
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        m_measuredValue.clear();
        emit Write(createParcel(GetMeasuredValue, static_cast<uint8_t>(type), channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delayMs[type])) {
            value = m_value;
            m_result = true;
        }
    }
    return m_result;
}

bool MAN2::getMeasuredValue(QVector<MeasuredValue>& value, ValueType type)
{
    if (Emu) {
        value.clear();
        m_measuredValue.clear();

        for (int i = 0; i < 8; ++i) {
            MeasuredValue val;
            val.value1 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
            val.value2 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
            val.value3 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
            value.append(val);
            m_measuredValue[i + 1] = val;
        }
        //GetMeasuredValueSignal(m_measuredValue);
        return true;
    }

    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        m_measuredValue.clear();
        value.clear();
        emit Write(createParcel(GetMeasuredValue, static_cast<uint8_t>(type), 0));
        if (m_semaphore.tryAcquire(ChannelCount, delayMs[type])) {
            emit measCompleted(m_measuredValue.first());
            QMapIterator<int, MeasuredValue> iterator(m_measuredValue);
            while (iterator.hasNext()) {
                iterator.next();
                value.insert(iterator.key() - 1, iterator.value());
            }
            m_result = true;
        }
    }
    return m_result;
}

double MAN2::getRmsValue()
{
    if (Emu) { //////////////////////////////////////////////////////////////////////
        return 220.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX);
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(GetRmsMeasuredValue, 9));
        if (m_semaphore.tryAcquire(1, 500)) {
            m_result = true;
            return m_rms;
        }
    }
    return 0.0;
}

bool MAN2::setCurrent(float Current, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        Current /= 1000.0f;
        emit Write(createParcel(SetCurrent, Current, channel));
        const int delay[] = { 5000, 500 };
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delay[channel == 0 ? 0 : 1]))
            m_result = true;
    }
    return m_result;
}

bool MAN2::switchCurrent(uint8_t Enable, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(SwitchCurrent, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::tripCurrentTest()
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(TripCurrentTest));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::shortCircuitTest(uint8_t Enable, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(ShortCircuitTest, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::oscilloscope(int channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(Oscilloscope, static_cast<uint8_t>(channel)));
        if (m_semaphore.tryAcquire(ChannelCount, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::setAddress(uint8_t newAddress, uint8_t oldAddress)
{
    if (newAddress == 0) {
        m_address = oldAddress;
        return false;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(SetAddress, newAddress, oldAddress));
        if (m_semaphore.tryAcquire(ChannelCount, 500)) {
            m_result = true;
            m_address = oldAddress;
        }
    }
    return m_result;
}

bool MAN2::setDefaultCalibrationCoefficients(uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        if (channel == 9) {
            m_rmsCoeff.AdcCh1Offset = 0.0;
            m_rmsCoeff.AdcCh2Offset = 1.0;
            m_result = true;
        } else {
            Reset();
            emit Write(createParcel(SetDefaultCalibCoeffi, channel));
            if (m_semaphore.tryAcquire(1, 500)) {
                m_result = true;
            }
        }
    }
    return m_result;
}

bool MAN2::getCalibrationCoefficients(GradCoeff& GradCoeff, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(GetCalibCoeff, channel));
        if (m_semaphore.tryAcquire(1, 1000)) {
            GradCoeff = m_coeff;
            m_result = true;
        }
    }
    return m_result;
}

bool MAN2::setCalibrationCoefficients(const GradCoeff& GradCoeff, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        float data[2]{ GradCoeff.AdcCh1Offset, GradCoeff.AdcCh1Scale };
        if (channel == 9)
            emit Write(createParcel(SetCalibCoeff, data, channel));
        else
            emit Write(createParcel(SetCalibCoeff, GradCoeff, channel));
        if (m_semaphore.tryAcquire(1, 500)) {
            if (channel == 9)
                m_rmsCoeff = GradCoeff;
            m_result = true;
        }
    }
    return m_result;
}

bool MAN2::saveToEepromCalibrationCoefficients(uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        emit Write(createParcel(SaveCalibCoeff, channel));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::disableAll()
{
    if (Emu) {
        return true;
    }
    while (IsConnected()) {
        Reset();
        if (!shortCircuitTest(Off))
            break;
        if (!switchCurrent(Off))
            break;
        if (!setCurrent(0.0))
            break;
        if (!oscilloscope(Off))
            break;
        m_result = true;
        break;
    }
    return m_result;
}

void MAN2::GetMeasuredValueSlot(ValueType type, uint8_t channel)
{
    if (Emu) {
        m_measuredValue.clear();
        for (int i = 0; i < 8; ++i) {
            MeasuredValue val;
            val.value1 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX * 100);
            val.value2 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX * 100);
            val.value3 = 24.0 + (qrand() - RAND_MAX / 2) / static_cast<double>(RAND_MAX * 100);
            m_measuredValue[i + 1] = val;
        }
        emit measCompleted(m_measuredValue.first());
        return;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {

        uint8_t tmp = type;
        m_measuredValue.clear();
        emit Write(createParcel(GetMeasuredValue, tmp, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delayMs[type]))
            m_result = true;
        if (!m_measuredValue.isEmpty())
            emit measCompleted(m_measuredValue.first());
    }
}

int MAN2::address() const
{
    return m_address;
}

bool MAN2::startTest(float start, float stop, float step, uint8_t channel)
{
    if (Emu) {
        return true;
    }
    QMutexLocker Locker(&m_mutex);
    if (IsConnected()) {
        Reset();
        MeasuredValue params;
        params.value1 = start;
        params.value2 = step;
        params.value3 = stop;
        emit Write(createParcel(TripCurrentTest, params, channel));
        if (m_semaphore.tryAcquire(1, 10000))
            m_result = true;
    }
    return m_result;
}

void MAN2::Init()
{
    getCalibrationCoefficients(m_rmsCoeff, 9);
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

void MAN2::RxPing(const Parcel& data)
{
    qDebug() << "PING" << (m_address = data.addres) << data.text();
    emit detectedAddress(m_address);
    m_semaphore.release();
}

void MAN2::RxGetMeasuredValue(const Parcel& data)
{
    m_value = data.value<MeasuredValue>();
    qDebug() << "GET_MEASURED_VALUE" << data.addres;
    if (m_value.type == ValueTripCurrent) {
        qDebug() << "Value1" << m_value.value1;
        qDebug() << "Value2" << m_value.value2;
        qDebug() << "Value3" << m_value.value3;
        emit measCompleted(m_value);
        return;
    }
    //    switch (m_value.Type) {
    //    case CurrentMeasuredValue: // Напряжение, ток и уставка.
    //        m_value.Value2 *= 1000.0f;
    //        m_value.Value3 *= 1000.0f;
    //        break;
    //    case ValueTripCurrent:
    //    case CalibVoltage:
    //    case CalibCurrent:
    //        break;
    //    case RawData:
    //        m_value.Value3 *= 1000.0f;
    //        break;
    //    default:
    //        return;
    //    }
    m_measuredValue[data.addres] = m_value;
    //    if (Dbg)
    m_semaphore.release();
}

void MAN2::RxSetCurrent(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "SET_CURRENT" << data.addres;
    m_value = data.value<MeasuredValue>();
    //    switch (m_value.Type) {
    //    case CurrentMeasuredValue: // Напряжение, ток и уставка.
    //        m_value.Value2 *= 1000.0f;
    //        m_value.Value3 *= 1000.0f;
    //        break;
    //    case ValueTripCurrent:
    //        //        m_value.Value1 = 123;
    //        //        m_value.Value2 = 456;
    //        //        m_value.Value3 = 789;
    //        break;
    //    case CalibVoltage:
    //        break;
    //    case CalibCurrent:
    //        //        m_value.Value1 = Ad779xSingleConversionFloat() * 0.01;
    //        break;
    //    case RawData:
    //        m_value.Value3 *= 1000.0f;
    //        break;
    //    default:
    //        return;
    //    }
    m_semaphore.release();
}

void MAN2::RxSwitchCurrent(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "SWITCH_CURRENT" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxTripCurrentTest(const Parcel& data)
{
    qDebug() << "TRIP_CURRENT_TEST" << data.addres;
    qDebug() << "Value1" << data.value<MeasuredValue>().value1;
    qDebug() << "Value2" << data.value<MeasuredValue>().value2;
    qDebug() << "Value3" << data.value<MeasuredValue>().value3;
    m_semaphore.release();
}

void MAN2::RxShortCircuitTest(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "SHORT_CIRCUIT_TEST" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxOscilloscope(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "OSCILLOSCOPE" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSetDefaultCalibCoeff(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "SET_DEFAULT_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxGetCalibCoeff(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "GET_CALIBRATION_COEFFICIENTS" << data.addres;
    m_coeff = data.value<GradCoeff>();
    m_semaphore.release();
}

void MAN2::RxSetCalibCoeff(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "SET_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSaveCalibCoeff(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "SAVE_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSetAddress(const Parcel& data)
{
    qDebug() << "RxSetAddress" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxGetRmsValue(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "GET_RMS_MEASURED_VALUE" << data.addres;
    uint32_t tmp = data.value<uint32_t>();
    m_rms = sqrt(tmp) * m_rmsCoeff.AdcCh1Scale + m_rmsCoeff.AdcCh1Offset;
    m_semaphore.release();
}

void MAN2::RxBufferOverflow(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "BUFFER_OVERFLOW" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxWrongCommand(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "WRONG_COMMAND" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxTextualParcel_t(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "TEXTUAL_PARCEL" << data.addres << data.text();
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxCrcError(const Parcel& data)
{
    //    if (Dbg)
    qDebug() << "CRC_ERROR" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxNullFunction(const Parcel& data)
{
    //    if (Dbg)
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
    m_f[Ping] = &MAN2::RxPing;
    m_f[GetMeasuredValue] = &MAN2::RxGetMeasuredValue;
    m_f[SetCurrent] = &MAN2::RxSetCurrent;
    m_f[SwitchCurrent] = &MAN2::RxSwitchCurrent;
    m_f[TripCurrentTest] = &MAN2::RxTripCurrentTest;
    m_f[ShortCircuitTest] = &MAN2::RxShortCircuitTest;
    m_f[Oscilloscope] = &MAN2::RxOscilloscope;
    m_f[SetDefaultCalibCoeffi] = &MAN2::RxSetDefaultCalibCoeff;
    m_f[GetCalibCoeff] = &MAN2::RxGetCalibCoeff;
    m_f[SetCalibCoeff] = &MAN2::RxSetCalibCoeff;
    m_f[SaveCalibCoeff] = &MAN2::RxSaveCalibCoeff;
    m_f[GetRmsMeasuredValue] = &MAN2::RxGetRmsValue;
    m_f[BufferOverflow] = &MAN2::RxBufferOverflow;
    m_f[WrongCommand] = &MAN2::RxWrongCommand;
    m_f[TextualParcel] = &MAN2::RxTextualParcel_t;
    m_f[CrcError] = &MAN2::RxCrcError;

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
    for (int i = 0; i < m_data.size() - 3; ++i) {
        const Parcel* const parcel = reinterpret_cast<const Parcel*>(m_data.constData() + i);
        if (parcel->start == RX) {
            if ((parcel->length + i) <= m_data.size()) {
                m_tmpData = m_data.mid(i, parcel->length);
                if (checkData(m_tmpData)) {
                    (m_man->*m_f[parcel->command])(*parcel);
                } else {
                    (m_man->*m_f[CrcError])(*parcel);
                    m_data.clear();
                }
                m_data.remove(0, i + parcel->length);
                i = -1;
            }
        }
    }
}
