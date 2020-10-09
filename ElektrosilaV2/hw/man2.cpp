#include "man2.h"

#include <QApplication>
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
    m_port->moveToThread(&portThread);
    connect(&portThread, &QThread::finished, m_port, &QObject::deleteLater);
    connect(this, &MAN2::Open, m_port, &SerialPort::Open);
    connect(this, &MAN2::Close, m_port, &SerialPort::Close);
    connect(this, &MAN2::Write, m_port, &SerialPort::Write);
    portThread.start(QThread::NormalPriority);
}

MAN2::~MAN2()
{
    portThread.quit();
    portThread.wait();
}

bool MAN2::ping(const QString& PortName)
{
    //QMutexLocker locker(&mutex);
    qDebug("Ping");
    reset();
    m_connected = false;
    do {
        emit Close();
        if (!semaphore.tryAcquire(1, 1000))
            break;
        if (!PortName.isEmpty())
            m_port->setPortName(PortName);
        emit Open();
        if (!semaphore.tryAcquire(1, 1000))
            break;
        emit Write(createParcel(Ping, 0));
        if (!semaphore.tryAcquire(ChannelCount, 2000)) {
            emit Close();
            break;
        }
        m_connected = true;
    } while (0);
    return m_connected;
}

bool MAN2::isConnected() const
{
    reset();
    return m_connected;
}

bool MAN2::getMeasuredValue(MeasuredValue& value, uint8_t channel, ValueType type)
{
    QMutexLocker Locker(&mutex);

    if (!isConnected())
        return result;

    if (channel == 0)
        return result;
    m_valueMap.clear();
    emit Write(createParcel(ReadMeasuredValue, static_cast<uint8_t>(type), channel));
    if (semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delayMs[type])) {
        result = true;
        crcErrCounter = 0;
        value = lastParcel.value<MeasuredValue>();
        emit measureCompleted(m_valueMap[channel]);
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::getMeasuredValue(QMap<int, MeasuredValue>& value, ValueType type)
{
    QMutexLocker Locker(&mutex);
    do {
        if (!isConnected())
            return result;
        m_valueMap.clear();
        emit Write(createParcel(ReadMeasuredValue, static_cast<uint8_t>(type), 0));
        if (semaphore.tryAcquire(ChannelCount, delayMs[type])) {
            result = true;
            crcErrCounter = 0;
            value = m_valueMap;
            emit measuresCompleted(m_valueMap);
        }

        if (crcErrCounter)
            qDebug() << crcErrCounter << Q_FUNC_INFO;
    } while (crcErrCounter);
    return result;
}

void MAN2::startMeasure(ValueType type, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return;
    m_valueMap.clear();
    emit Write(createParcel(ReadMeasuredValue, static_cast<uint8_t>(type), channel));
    if (semaphore.tryAcquire(channel == 0 ? ChannelCount : 1, delayMs[type])) {
        if (channel && m_valueMap.contains(channel)) {
            result = true;
            crcErrCounter = 0;
            emit measureCompleted(m_valueMap[channel]);
        } else {
            result = true;
            crcErrCounter = 0;
            emit measuresCompleted(m_valueMap);
        }
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
}

double MAN2::getRmsValue()
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(ReadMeasuredValue, 10));
    if (semaphore.tryAcquire(1, 500)) {
        if (m_valueMap.contains(10)) {
            result = true;
            crcErrCounter = 0;
            return m_valueMap[10].valCh1;
        }
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return 0.0;
}

bool MAN2::setCurrent(float Current_mA, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    do {
        if (!isConnected())
            return result;
        Current_mA /= 1000.0f;
        emit Write(createParcel(SetCurrent, Current_mA, channel));
        if (semaphore.tryAcquire(channel ? 1 : ChannelCount, delayMsSm[channel ? 1 : 0])) {
            result = true;
            crcErrCounter = 0;
        }
        if (crcErrCounter)
            qDebug() << crcErrCounter << Q_FUNC_INFO;
    } while (crcErrCounter);
    return result;
}

bool MAN2::switchCurrent(uint8_t Enable, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    do {
        if (!isConnected())
            return result;
        emit Write(createParcel(SwitchCurrent, Enable, channel));
        if (semaphore.tryAcquire(channel ? 1 : ChannelCount, delayMsSm[channel ? 1 : 0])) {
            result = true;
            crcErrCounter = 0;
        }

        if (crcErrCounter)
            qDebug() << crcErrCounter << Q_FUNC_INFO;

    } while (crcErrCounter);
    return result;
}

bool MAN2::switchShortCircuit(ShortCircuit scType, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(SwitchShortCircuit, static_cast<uint8_t>(scType), channel));
    if (semaphore.tryAcquire(channel ? 1 : ChannelCount, delayMsSm[channel ? 1 : 0])) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::tripCurrentTest(TripTest tt, uint8_t channel) /*Test*/
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    tt.start /= 1000.0f;
    tt.step /= 1000.0f;
    tt.stop /= 1000.0f;
    emit Write(createParcel(TripCurrentTest, tt, channel));
    if (semaphore.tryAcquire(1, 60000)) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}
bool MAN2::shortCircuitTest(ShortCircuit scType, uint8_t channel) /*Test*/
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(ShortCircuitTest, static_cast<uint8_t>(scType), channel));
    if (semaphore.tryAcquire(1, 2000)) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::oscilloscope(uint8_t channel, Osc osc)
{
    qDebug() << "oscilloscope" << channel << Oscs(osc);
    QMutexLocker Locker(&mutex);
    do {
        if (!isConnected())
            return result;
        emit Write(createParcel(Oscilloscope, static_cast<uint8_t>(osc), channel));
        if (semaphore.tryAcquire(ChannelCount, 500)) {
            result = true;
            crcErrCounter = 0;
        }

        if (crcErrCounter)
            qDebug() << crcErrCounter << Q_FUNC_INFO;

    } while (crcErrCounter);
    return result;
}

bool MAN2::setAddress(uint8_t oldAddress, uint8_t newAddress)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(SetAddress, oldAddress, newAddress));
    if (semaphore.tryAcquire(1, 500)) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::setDefaultCalibrationCoefficients(uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(SetDefaultCalibrationCoefficients, channel));
    if (semaphore.tryAcquire(1, 500)) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::getCalibrationCoefficients(GradCoeff& gc, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(GetCalibrationCoefficients, channel));
    if (semaphore.tryAcquire(1, 500)) {
        gc = lastParcel.value<GradCoeff>();
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::setCalibrationCoefficients(const GradCoeff& GradCoeff, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(SetCalibrationCoefficients, GradCoeff, channel));
    if (semaphore.tryAcquire(1, 500)) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::saveToEepromCalibrationCoefficients(uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(SaveCalibrationCoefficients, channel));
    if (semaphore.tryAcquire(1, 500)) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << Q_FUNC_INFO;
    return result;
}

bool MAN2::disableAll()
{
    if (!isConnected())
        return result;
    do {
        if (switchShortCircuit(ScOff))
            if (switchCurrent(Off))
                if (setCurrent(0.0))
                    if (oscilloscope(Off))
                        break;
        if (crcErrCounter)
            qDebug() << crcErrCounter << Q_FUNC_INFO;
    } while (crcErrCounter);
    return result;
}

void MAN2::reset() const
{
    result = false;
    //    if (crcErrCounter)
    //        --crcErrCounter;
    semaphore.acquire(semaphore.available());
}

void MAN2::RxPing(const Parcel& data)
{
    //qDebug() << "RxPing" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxReadMeasuredValue(const Parcel& data)
{
    ////qDebug() << "RxReadMeasuredValue" << data.addres;
    lastParcel = data;

    auto value = data.value<MeasuredValue>();
    switch (value.type) {
    case CurrentMeasuredValue: // Напряжение, ток и уставка.
        value.valCh2 *= 1000.0f; // A -> mA
        value.valCh3 *= 1000.0f; // A -> mA
        break;
    case ValueTripCurrent:
    case CalibVoltage:
    case CalibCurrent:
        break;
    case RawData:
        value.valCh3 *= 1000.0f; // A -> mA
        break;
    default: // error
        semaphore.release();
        return;
    }
    m_valueMap[data.addres] = value;

    semaphore.release();
}
void MAN2::RxSetCurrent(const Parcel& data)
{
    //qDebug() << "RxSetCurrent" << data.addres;
    lastParcel = data;

    auto value = data.value<MeasuredValue>();
    switch (value.type) {
    case CurrentMeasuredValue: // Напряжение, ток и уставка.
        value.valCh2 *= 1000.0f; // A -> mA
        value.valCh3 *= 1000.0f; // A -> mA
        break;
    case RawData:
        value.valCh3 *= 1000.0f; // A -> mA
        break;
    default: // error
        semaphore.release();
        return;
    }
    m_valueMap[data.addres] = value;

    semaphore.release();
}
void MAN2::RxSwitchCurrent(const Parcel& data)
{
    //qDebug() << "RxSwitchCurrent" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSwitchShortCircuit(const Parcel& data)
{
    //qDebug() << "RxSwitchShortCircuit" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxTripCurrentTest(const Parcel& data)
{
    //qDebug() << "RxTripCurrentTest" << data.addres;
    lastParcel = data;

    auto value = data.value<MeasuredValue>();
    switch (value.type) {
    case ValueTripCurrent:
        value.valCh2 *= 1000.0f; // A -> mA
        value.valCh3 *= 1000.0f; // A -> mA
        break;
    default: // error
        semaphore.release();
        return;
    }
    m_valueMap[data.addres] = value;

    emit measuresCompleted(m_valueMap);

    QApplication::processEvents();

    if (!value.manState.tripCurrentTest) // if end
        semaphore.release();
}
void MAN2::RxShortCircuitTest(const Parcel& data)
{
    //qDebug() << "RxShortCircuitTest" << data.addres;
    lastParcel = data;

    auto value = data.value<MeasuredValue>();
    switch (value.type) {
    case RawData: // Напряжение, ток и уставка.
        value.valCh2 *= 1000.0f; // A -> mA
        //value.valCh3; // Is time - sec
        break;
    default: // error
        semaphore.release();
        return;
    }
    m_valueMap[data.addres] = value;

    qDebug() << value;

    semaphore.release();
}
void MAN2::RxOscilloscope(const Parcel& data)
{
    //qDebug() << "RxOscilloscope" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxGetCalibrationCoefficients(const Parcel& data)
{
    //qDebug() << "RxGetCalibrationCoefficients" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSetDefaultCalibrationCoefficients(const Parcel& data)
{
    //qDebug() << "RxSetDefaultCalibrationCoefficients" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSetCalibrationCoefficients(const Parcel& data)
{
    //qDebug() << "RxSetCalibrationCoefficients" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSaveCalibrationCoefficients(const Parcel& data)
{
    //qDebug() << "RxSaveCalibrationCoefficients" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSetAddress(const Parcel& data)
{
    //qDebug() << "RxSetAddress" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxBufferOverflow(const Parcel& data)
{
    //qDebug() << "RxBufferOverflow" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxWrongCommand(const Parcel& data)
{
    qDebug() << "RxWrongCommand" << data.addres << static_cast<Command>(data.command);
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxTextualParcel(const Parcel& data)
{
    qDebug() << "RxTextualParcel" << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxCrcError(const Parcel& data)
{
    qDebug() << "RxCrcError" << data.addres << static_cast<Command>(data.command);
    lastParcel = data;
    ++crcErrCounter;
    //thread()->sleep(1);
    //semaphore.release(9);
}
void MAN2::RxNullFunction(const Parcel& data)
{
    qDebug() << "RxNullFunction" << data.addres << data.command;
    lastParcel = data;
    semaphore.release(9);
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
    m_f[ReadMeasuredValue] = &MAN2::RxReadMeasuredValue;
    m_f[SetCurrent] = &MAN2::RxSetCurrent;
    m_f[SwitchCurrent] = &MAN2::RxSwitchCurrent;
    m_f[SwitchShortCircuit] = &MAN2::RxSwitchShortCircuit;
    m_f[TripCurrentTest] = &MAN2::RxTripCurrentTest;
    m_f[ShortCircuitTest] = &MAN2::RxShortCircuitTest;
    m_f[Oscilloscope] = &MAN2::RxOscilloscope;
    m_f[GetCalibrationCoefficients] = &MAN2::RxGetCalibrationCoefficients;
    m_f[SetDefaultCalibrationCoefficients] = &MAN2::RxSetDefaultCalibrationCoefficients;
    m_f[SetCalibrationCoefficients] = &MAN2::RxSetCalibrationCoefficients;
    m_f[SaveCalibrationCoefficients] = &MAN2::RxSaveCalibrationCoefficients;
    m_f[SetAddress] = &MAN2::RxSetAddress;
    m_f[BufferOverflow] = &MAN2::RxBufferOverflow;
    m_f[WrongCommand] = &MAN2::RxWrongCommand;
    m_f[TextualParcel] = &MAN2::RxTextualParcel;
    m_f[CrcError] = &MAN2::RxCrcError;

    // connect(this, &QSerialPort::breakEnabledChanged, [](bool set) { //qDebug() << "breakEnabled" << set; });
    // connect(this, &QSerialPort::dataTerminalReadyChanged, [](bool set) { //qDebug() << "dataTerminalReady" << set; });
    // connect(this, &QSerialPort::requestToSendChanged, [](bool set) { //qDebug() << "requestToSend" << set; });
    // connect(this, &QSerialPort::baudRateChanged, [](qint32 baudRate, QSerialPort::Directions directions) { qDebug() << baudRate << directions; });
    // connect(this, &QSerialPort::dataBitsChanged, [](QSerialPort::DataBits dataBits) { qDebug() << dataBits; });

    connect(this, &QSerialPort::errorOccurred, [this](QSerialPort::SerialPortError error) {
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
        //qDebug() << "\n0" << error;
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

    // connect(this, &QSerialPort::flowControlChanged, [](QSerialPort::FlowControl flow) { qDebug() << flow; });
    // connect(this, &QSerialPort::parityChanged, [](QSerialPort::Parity parity) { qDebug() << parity; });
    // connect(this, &QSerialPort::stopBitsChanged, [](QSerialPort::StopBits stopBits) { qDebug() << stopBits; });

    setBaudRate(QSerialPort::Baud57600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &SerialPort::ReadyRead);
}

SerialPort::~SerialPort() { }

void SerialPort::Open()
{
    if (open(ReadWrite))
        m_man->semaphore.release();
}

void SerialPort::Close()
{
    close();
    m_man->semaphore.release();
}

void SerialPort::Write(const QByteArray& data)
{
    //    //qDebug() << "Write" << data.toHex().toUpper();
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
