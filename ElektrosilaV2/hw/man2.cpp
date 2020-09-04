#include "man2.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTimer>

#define Dbg 0

const int id1 = qRegisterMetaType<MeasuredValue>("MeasuredValue");
const int id2 = qRegisterMetaType<ValueType>("ValueType");
const int id3 = qRegisterMetaType<uint8_t>("uint8_t");
const int id5 = qRegisterMetaType<QMap<int, MeasuredValue>>("QMap<int, MeasuredValue>");
const int id6 = qRegisterMetaType<MeasuredValue>("MeasuredValue");

class SerialPort : public QSerialPort, private MyProtokol {
public:
    SerialPort(MAN2* manInterface);
    ~SerialPort();
    void Open();
    void Close();
    void Write(const QByteArray& data);

    bool m_isOpen;

    MAN2* m_man;
    using func = void (MAN2::*)(const Parcel&);
    QVector<func> m_f;

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief MAN2::MAN2
/// \param parent
///
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
    QMutexLocker locker(&m_mutex);
    qDebug("Ping");
    reset();
    m_connected = false;
    do {
        emit Close();
        if (!m_semaphore.tryAcquire(1, 1000))
            break;
        if (!PortName.isEmpty())
            m_port->setPortName(PortName);
        emit Open();
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

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        m_valueMap.clear();
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
    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        m_valueMap.clear();
        value.clear();
        emit Write(createParcel(GetMeasuredValue, static_cast<uint8_t>(type), 0));
        if (m_semaphore.tryAcquire(ChannelCount, delayMs[type])) {
            emit measureCompleted(m_valueMap.first());
            QMapIterator<int, MeasuredValue> iterator(m_valueMap);
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
    //    QMutexLocker Locker(&m_mutex);
    //    if (IsConnected()) {
    //        Reset();
    //        emit Write(createParcel(GetRmsMeasuredValue, 9));
    //        if (m_semaphore.tryAcquire(1, 500)) {
    //            m_result = true;
    //            return m_rms;
    //        }
    //    }
    return 0.0;
}

bool MAN2::setCurrent(float Current, uint8_t channel)
{

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
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

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        emit Write(createParcel(SwitchCurrent, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::tripCurrentTest()
{

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        emit Write(createParcel(TripCurrentTest));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::shortCircuitTest(uint8_t Enable, uint8_t channel)
{

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        emit Write(createParcel(ShortCircuitTest, Enable, channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::oscilloscope(int channel)
{

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
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
    if (isConnected()) {
        reset();
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
    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        if (channel == 9) {
            m_rmsCoeff.adcCh1.offset = 0.0;
            m_rmsCoeff.adcCh2.offset = 1.0;
            m_result = true;
        } else {
            reset();
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
    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
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
    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        float data[2] { GradCoeff.adcCh1.offset, GradCoeff.adcCh1.scale };
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

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        emit Write(createParcel(SaveCalibCoeff, channel));
        if (m_semaphore.tryAcquire(1, 500))
            m_result = true;
    }
    return m_result;
}

bool MAN2::disableAll()
{

    while (isConnected()) {
        reset();
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

void MAN2::startMeasure(ValueType type, uint8_t channel)
{
    QMutexLocker Locker(&m_mutex);
    QElapsedTimer t;
    t.start();
    if (isConnected()) {
        m_valueMap.clear();
        emit Write(createParcel(GetMeasuredValue, static_cast<uint8_t>(type), channel));
        if (m_semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delayMs[type]))
            m_result = true;
        if (!m_valueMap.isEmpty())
            emit measureCompleted(m_valueMap.first());
        emit measuresCompleted(m_valueMap);
    }
    qDebug() << "startMeasure" << t.elapsed() << m_result;
}

int MAN2::address() const { return m_address; }

bool MAN2::startTest(float start, float stop, float step, uint8_t channel)
{

    QMutexLocker Locker(&m_mutex);
    if (isConnected()) {
        reset();
        MeasuredValue params;
        params.valCh1 = start;
        params.valCh2 = step;
        params.valCh3 = stop;
        emit Write(createParcel(TripCurrentTest, params, channel));
        if (m_semaphore.tryAcquire(1, 10000))
            m_result = true;
    }
    return m_result;
}

void MAN2::Init()
{
    getCalibrationCoefficients(m_rmsCoeff, 9);
    qDebug() << m_rmsCoeff.adcCh1.offset; //-46.0562
    qDebug() << m_rmsCoeff.adcCh1.scale; //0.402455
    //-46.3256
    //0.402617
}

void MAN2::reset()
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

//void MAN2::RxGetMeasuredValue(const Parcel& data)
//{
//    m_value = data.value<MeasuredValue>();
//    switch (m_value.type) {
//    case CurrentMeasuredValue: // Напряжение, ток и уставка.
//        m_value.valCh2 *= 1000.0f;
//        m_value.valCh3 *= 1000.0f;
//        break;
//    case ValueTripCurrent:
//    case CalibVoltage:
//    case CalibCurrent:
//        break;
//    case RawData:
//        m_value.valCh3 *= 1000.0f;
//        break;
//    default:
//        return;
//    }
//    m_measuredValue[data.addres] = m_value;
//    if constexpr (Dbg)
//        qDebug() << "GET_MEASURED_VALUE" /* << data.toHex().toUpper()*/ << m_value.valCh1 << m_value.valCh2 << m_value.valCh3;
//    m_semaphore.release();
//}

//void MAN2::RxSetCurrent(const Parcel& data)
//{
//    if constexpr (Dbg)
//        qDebug() << "SET_CURRENT" << data.addres;
//    m_value = data.value<MeasuredValue>();
//    switch (m_value.type) {
//    case CurrentMeasuredValue: // Напряжение, ток и уставка.
//        m_value.valCh2 *= 1000.0f;
//        m_value.valCh3 *= 1000.0f;
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
//        m_value.valCh3 *= 1000.0f;
//        break;
//    default:
//        return;
//    }
//    m_semaphore.release();
//}

void MAN2::RxGetMeasuredValue(const Parcel& data)
{
    m_value = data.value<MeasuredValue>();
    if constexpr (Dbg)
        qDebug() << "GET_MEASURED_VALUE" << data.addres;
    switch (m_value.type) {
    case CurrentMeasuredValue: // Напряжение, ток и уставка.
        m_value.valCh2 *= 1000.0f;
        m_value.valCh3 *= 1000.0f;
        break;
    case ValueTripCurrent:
    case CalibVoltage:
    case CalibCurrent:
        break;
    case RawData:
        m_value.valCh3 *= 1000.0f;
        break;
    default:
        return;
    }
    m_valueMap[data.addres] = m_value;
    emit measureCompleted(m_value);
    m_semaphore.release();
}

void MAN2::RxSetCurrent(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "SET_CURRENT" << data.addres;
    m_value = data.value<MeasuredValue>();
    switch (m_value.type) {
    case CurrentMeasuredValue: // Напряжение, ток и уставка.
        m_value.valCh2 *= 1000.0f;
        m_value.valCh3 *= 1000.0f;
        break;
    case ValueTripCurrent:
        //        m_value.Value1 = 123;
        //        m_value.Value2 = 456;
        //        m_value.Value3 = 789;
        break;
    case CalibVoltage:
        break;
    case CalibCurrent:
        //        m_value.Value1 = Ad779xSingleConversionFloat() * 0.01;
        break;
    case RawData:
        m_value.valCh3 *= 1000.0f;
        break;
    default:
        return;
    }
    m_semaphore.release();
}

void MAN2::RxSwitchCurrent(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "SWITCH_CURRENT" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxTripCurrentTest(const Parcel& data)
{
    qDebug() << "TRIP_CURRENT_TEST" << data.addres;
    qDebug() << "Value1" << data.value<MeasuredValue>().valCh1;
    qDebug() << "Value2" << data.value<MeasuredValue>().valCh2;
    qDebug() << "Value3" << data.value<MeasuredValue>().valCh3;
    m_semaphore.release();
}

void MAN2::RxShortCircuitTest(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "SHORT_CIRCUIT_TEST" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxOscilloscope(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "OSCILLOSCOPE" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSetDefaultCalibCoeff(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "SET_DEFAULT_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxGetCalibCoeff(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "GET_CALIBRATION_COEFFICIENTS" << data.addres;
    m_coeff = data.value<GradCoeff>();
    m_semaphore.release();
}

void MAN2::RxSetCalibCoeff(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "SET_CALIBRATION_COEFFICIENTS" << data.addres;
    Q_UNUSED(data)
    m_semaphore.release();
}

void MAN2::RxSaveCalibCoeff(const Parcel& data)
{
    if constexpr (Dbg)
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
    if constexpr (Dbg)
        qDebug() << "GET_RMS_MEASURED_VALUE" << data.addres;
    uint32_t tmp = data.value<uint32_t>();
    m_rms = sqrt(tmp) * m_rmsCoeff.adcCh1.scale + m_rmsCoeff.adcCh1.offset;
    m_semaphore.release();
}

void MAN2::RxBufferOverflow(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "BUFFER_OVERFLOW" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxWrongCommand(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "WRONG_COMMAND" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxTextualParcel_t(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "TEXTUAL_PARCEL" << data.addres << data.text();
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxCrcError(const Parcel& data)
{
    if constexpr (Dbg)
        qDebug() << "CRC_ERROR" << data.addres;
    Q_UNUSED(data)
    //m_semaphore.release();
}

void MAN2::RxNullFunction(const Parcel& data)
{
    if constexpr (Dbg)
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

    //    connect(this, &QSerialPort::breakEnabledChanged, [](bool set) { qDebug() << "breakEnabled" << set; });
    //    connect(this, &QSerialPort::dataTerminalReadyChanged, [](bool set) { qDebug() << "dataTerminalReady" << set; });
    //    connect(this, &QSerialPort::requestToSendChanged, [](bool set) { qDebug() << "requestToSend" << set; });

    //    connect(this, &QSerialPort::baudRateChanged, [](qint32 baudRate, QSerialPort::Directions directions) { qDebug() << baudRate << directions; });
    //    connect(this, &QSerialPort::dataBitsChanged, [](QSerialPort::DataBits dataBits) { qDebug() << dataBits; });
    connect(this, &QSerialPort::errorOccurred, [this](QSerialPort::SerialPortError error) {
        qDebug() << error;
        switch (error) {
        case NoError:
        case DeviceNotFoundError:
        case PermissionError:
        case OpenError:
        case ParityError:
        case FramingError:
        case BreakConditionError:
            break;
        case WriteError:
        case ReadError:
            if (isOpen()) {
                close();
                thread()->msleep(100);
                reset();
                qDebug() << errorString();
                open(ReadWrite);
                qDebug() << errorString();
            }
            break;
        case ResourceError:
            if (isOpen()) {
                close();
                thread()->msleep(100);
                reset();
                qDebug() << errorString();
                open(ReadWrite);
                qDebug() << errorString();
            }
            break;
        case UnsupportedOperationError:
        case UnknownError:
        case TimeoutError:
        case NotOpenError:
            break;
        }
    });
    connect(this, &QSerialPort::errorOccurred, [this](QSerialPort::SerialPortError error) {
        qDebug() << "\n0" << error;
        switch (error) {
        case NoError:
        case DeviceNotFoundError:
        case PermissionError:
        case OpenError:
        case ParityError:
        case FramingError:
        case BreakConditionError:
        case WriteError:
        case ReadError:
            break;
        case ResourceError:
            close();
            reset();
            thread()->msleep(1000);
            reset();
            qDebug() << '1' << errorString();
            open(ReadWrite);
            qDebug() << '2' << errorString();
            open(ReadWrite);
            qDebug() << '3' << errorString();
            open(ReadWrite);
            qDebug() << '4' << errorString();
            open(ReadWrite);
            qDebug() << '5' << errorString();
            break;
        case UnsupportedOperationError:
        case UnknownError:
        case TimeoutError:
        case NotOpenError:
            break;
        }
    });
    //    connect(this, &QSerialPort::flowControlChanged, [](QSerialPort::FlowControl flow) { qDebug() << flow; });
    //    connect(this, &QSerialPort::parityChanged, [](QSerialPort::Parity parity) { qDebug() << parity; });
    //    connect(this, &QSerialPort::stopBitsChanged, [](QSerialPort::StopBits stopBits) { qDebug() << stopBits; });

    setBaudRate(QSerialPort::Baud57600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &SerialPort::ReadyRead);
}

SerialPort::~SerialPort() { }

void SerialPort::Open()
{
    if (open(ReadWrite))
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
