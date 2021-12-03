#include "man2.h"

#include <QApplication>
#include <QDebug>
#include <QSerialPortInfo>
#include <QTimer>

#define Dbg 0

const int id1 = qRegisterMetaType<MeasuredValue>("MeasuredValue");
const int id2 = qRegisterMetaType<ValueType>("ValueType");
const int id3 = qRegisterMetaType<uint8_t>("uint8_t");
const int id5 = qRegisterMetaType<std::map<int, MeasuredValue>>("std::map<int, MeasuredValue>");
const int id6 = qRegisterMetaType<MeasureMap>("MeasureMap");
const int id7 = qRegisterMetaType<MeasuredValue>("MeasuredValue");

static const QStringList cmdList {
    /*00*/ "Ping",
    /*01*/ "ReadMeasuredValue",
    /*02*/ "SetCurrent",
    /*03*/ "SwitchCurrent",
    /*04*/ "SwitchShortCircuit",
    /*05*/ "TripCurrentTest",
    /*06*/ "ShortCircuitTest",
    /*07*/ "Oscilloscope",
    /*08*/ "GetCalibrationCoefficients",
    /*09*/ "SetDefaultCalibrationCoefficients",
    /*10*/ "SetCalibrationCoefficients",
    /*11*/ "SaveCalibrationCoefficients",
    /*12*/ "SetAddress",
};

class SerialPort : public QSerialPort, private MyProtokol {
public:
    SerialPort(MAN2* man);
    ~SerialPort();
    void Open();
    void Close();
    void Write(const QByteArray& data);
    MAN2* man;
    bool m_isOpen;
    using callback = void (MAN2::*)(Parcel);
    QVector<callback> callbacks;

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
};

///////////
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
    if (EMU)
        return m_connected = true;

    QMutexLocker locker(&mutex);
    reset();
    m_connected = false;
    crcErrCounter = 0;
    do {
        emit Close();
        if (!semaphore.tryAcquire(1, 2000))
            break;
        if (!PortName.isEmpty())
            m_port->setPortName(PortName);
        emit Open();
        if (!semaphore.tryAcquire(1, 2000))
            break;
        emit Write(createParcel(Ping, 0));
        if (!semaphore.tryAcquire(ChannelCount, 1000)) {
            emit Close();
            break;
        }
        m_connected = true;
    } while (0);
    qDebug(__FUNCTION__);
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
        qDebug() << crcErrCounter << __FUNCTION__;
    return result;
}

bool MAN2::getMeasuredValue(MeasureMap& value, ValueType type)
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
            qDebug() << crcErrCounter << __FUNCTION__;
    } while (crcErrCounter && crcErrCounter < 10);
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        if (semaphore.tryAcquire(channel ? 1 : ManCount, delayMsSm[channel ? 1 : 0])) {
            result = true;
            crcErrCounter = 0;
        }
        if (crcErrCounter)
            qDebug() << crcErrCounter << __FUNCTION__;
    } while (crcErrCounter && crcErrCounter < 10);
    return result;
}

bool MAN2::switchCurrent(uint8_t Enable, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    do {
        if (!isConnected())
            return result;
        emit Write(createParcel(SwitchCurrent, Enable, channel));
        if (semaphore.tryAcquire(channel ? 1 : ManCount, delayMsSm[channel ? 1 : 0])) {
            result = true;
            crcErrCounter = 0;
        }

        if (crcErrCounter)
            qDebug() << crcErrCounter << __FUNCTION__;

    } while (crcErrCounter && crcErrCounter < 10);
    return result;
}

bool MAN2::switchShortCircuit(ShortCircuit scType, uint8_t channel)
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    emit Write(createParcel(SwitchShortCircuit, static_cast<uint8_t>(scType), channel));
    if (semaphore.tryAcquire(channel ? 1 : ManCount, delayMsSm[channel ? 1 : 0])) {
        result = true;
        crcErrCounter = 0;
    }

    if (crcErrCounter)
        qDebug() << crcErrCounter << __FUNCTION__;
    return result;
}

bool MAN2::tripCurrentTest(TripTest tt, uint8_t channel) /*Test*/
{
    QMutexLocker Locker(&mutex);
    if (!isConnected())
        return result;
    if (channel == 0)
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        qDebug() << crcErrCounter << __FUNCTION__;
    return result;
}

bool MAN2::oscilloscope(uint8_t channel, Osc osc)
{
    QMutexLocker Locker(&mutex);
    do {
        if (!isConnected())
            return result;
        emit Write(createParcel(Oscilloscope, static_cast<uint8_t>(osc), channel));
        if (semaphore.tryAcquire(ManCount, 1000)) {
            result = true;
            crcErrCounter = 0;
        }

        if (crcErrCounter)
            qDebug() << "crcErrCounter" << crcErrCounter << __FUNCTION__;
    } while (crcErrCounter && crcErrCounter < 10);
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
        qDebug() << crcErrCounter << __FUNCTION__;
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
            qDebug() << crcErrCounter << __FUNCTION__;
    } while (crcErrCounter && crcErrCounter < 10);
    return result;
}

void MAN2::reset() const
{
    result = false;
    semaphore.acquire(semaphore.available());
}

void MAN2::RxPing(Parcel data)
{
    qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxReadMeasuredValue(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
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
void MAN2::RxSetCurrent(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
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
void MAN2::RxSwitchCurrent(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSwitchShortCircuit(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxTripCurrentTest(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
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
void MAN2::RxShortCircuitTest(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
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
void MAN2::RxOscilloscope(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxGetCalibrationCoefficients(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSetDefaultCalibrationCoefficients(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSetCalibrationCoefficients(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSaveCalibrationCoefficients(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxSetAddress(Parcel data)
{
    qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxBufferOverflow(Parcel data)
{
    //qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxWrongCommand(Parcel data)
{
    qDebug() << __FUNCTION__ << data.addres << static_cast<Command>(data.command);
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxTextualParcel(Parcel data)
{
    qDebug() << __FUNCTION__ << data.addres;
    lastParcel = data;
    semaphore.release();
}
void MAN2::RxCrcError(Parcel data)
{
    qDebug() << __FUNCTION__ << data.addres << cmdList.value(data.command);
    lastParcel = data;
    ++crcErrCounter;
}
void MAN2::RxNullFunction(Parcel data)
{
    qDebug() << __FUNCTION__ << data.addres << data.command;
    lastParcel = data;
    semaphore.release(9);
}

////////////////////////////////////
/// \brief SerialPort::SerialPort
/// \param manInterface
///
SerialPort::SerialPort(MAN2* man)
    : man(man)
    , m_isOpen(false)
    , callbacks(0x100, &MAN2::RxNullFunction)
{
    callbacks[Ping] = &MAN2::RxPing;
    callbacks[ReadMeasuredValue] = &MAN2::RxReadMeasuredValue;
    callbacks[SetCurrent] = &MAN2::RxSetCurrent;
    callbacks[SwitchCurrent] = &MAN2::RxSwitchCurrent;
    callbacks[SwitchShortCircuit] = &MAN2::RxSwitchShortCircuit;
    callbacks[TripCurrentTest] = &MAN2::RxTripCurrentTest;
    callbacks[ShortCircuitTest] = &MAN2::RxShortCircuitTest;
    callbacks[Oscilloscope] = &MAN2::RxOscilloscope;
    callbacks[GetCalibrationCoefficients] = &MAN2::RxGetCalibrationCoefficients;
    callbacks[SetDefaultCalibrationCoefficients] = &MAN2::RxSetDefaultCalibrationCoefficients;
    callbacks[SetCalibrationCoefficients] = &MAN2::RxSetCalibrationCoefficients;
    callbacks[SaveCalibrationCoefficients] = &MAN2::RxSaveCalibrationCoefficients;
    callbacks[SetAddress] = &MAN2::RxSetAddress;
    callbacks[BufferOverflow] = &MAN2::RxBufferOverflow;
    callbacks[WrongCommand] = &MAN2::RxWrongCommand;
    callbacks[TextualParcel] = &MAN2::RxTextualParcel;
    callbacks[CrcError] = &MAN2::RxCrcError;

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
        //qDebug() << __FUNCTION__ << error;
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

    setBaudRate(QSerialPort::Baud57600);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    connect(this, &QSerialPort::readyRead, this, &SerialPort::ReadyRead);
}

SerialPort::~SerialPort() { }

void SerialPort::Open()
{
    if (open(ReadWrite))
        man->semaphore.release();
}

void SerialPort::Close()
{
    close();
    man->semaphore.release();
}

void SerialPort::Write(const QByteArray& data)
{
    //qDebug() << __FUNCTION__ << data.toHex().toUpper();
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
                    (man->*callbacks[parcel->command])(*parcel);
                } else {
                    (man->*callbacks[CrcError])(*parcel);
                    m_data.clear();
                }
                m_data.remove(0, i + parcel->length);
                i = -1;
            }
        }
    }
}
