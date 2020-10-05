#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H

#include "common_interfaces.h"
#include "myprotokol.h"
#include <QComboBox>
#include <QDebug>
#include <QElapsedTimer>
#include <QMutex>
#include <QObject>
#include <QSemaphore>
#include <QSerialPort>
#include <QThread>
#include <QVector>
#include <QWaitCondition>
#include <stdint.h>

enum StateEnum {
    Off,
    On
};

enum { ChannelCount = 9 };

enum Command {
    /*00*/ Ping,
    /*01*/ ReadMeasuredValue,
    /*02*/ SetCurrent,
    /*03*/ SwitchCurrent,
    /*04*/ SwitchShortCircuit,
    /*05*/ TripCurrentTest,
    /*06*/ ShortCircuitTest,
    /*07*/ Oscilloscope,
    /*08*/ GetCalibrationCoefficients,
    /*09*/ SetDefaultCalibrationCoefficients,
    /*10*/ SetCalibrationCoefficients,
    /*11*/ SaveCalibrationCoefficients,
    /*12*/ SetAddress,
    BufferOverflow = 0xf0,
    WrongCommand = 0xf1,
    TextualParcel = 0xf2,
    CrcError = 0xf3
};

enum ShortCircuit {
    ScOff,
    ScGnd,
    ScShunt,
    ScGndAndShunt
};

enum Osc {
    OscOff,
    OscI,
    OscU,
    OscIU,
};

enum ValueType {
    CurrentMeasuredValue, // Напряжение, ток и уставка.
    ValueTripCurrent,
    CalibVoltage,
    CalibCurrent,
    RawData
};

#pragma pack(push, 1)

struct ChCoeff {
    float offset = 0.0;
    float scale = 0.0;
};

struct GradCoeff {
    ChCoeff adcCh1;
    ChCoeff adcCh2;
    ChCoeff adcCh3;
    ChCoeff dac;
}; //0x00

struct TripTest {
    float start;
    float stop;
    float step;
};

struct ManState {
    uint8_t load : 1;
    uint8_t shortCircuit : 2;
    uint8_t overHeat : 1;
    uint8_t oscilloscope : 2;
    uint8_t tripCurrentTest : 1;
    uint8_t : 1;
};

struct MeasuredValue {
    friend inline QDebug operator<<(QDebug debug, const MeasuredValue& mv)
    {
        const bool oldSetting = debug.autoInsertSpaces();
        debug.nospace();
        debug << QString("MV(%1, %2, %3)").arg(mv.ch1, 1, 'f', 5).arg(mv.ch2, 1, 'f', 5).arg(mv.ch3, 1, 'f', 5).toLocal8Bit().data();
        debug.setAutoInsertSpaces(oldSetting);
        return debug.maybeSpace();
    }

    float ch1 = 0.0;
    float ch2 = 0.0;
    float ch3 = 0.0;
    uint8_t type = 0;
    ManState manState;
};

#pragma pack(pop)

class MAN2;
class SerialPort;

struct CallBack {
    virtual ~CallBack() = default;

    /*00*/ virtual void RxPing(const Parcel& data) = 0;
    /*01*/ virtual void RxReadMeasuredValue(const Parcel& data) = 0;
    /*02*/ virtual void RxSetCurrent(const Parcel& data) = 0;
    /*03*/ virtual void RxSwitchCurrent(const Parcel& data) = 0;
    /*04*/ virtual void RxSwitchShortCircuit(const Parcel& data) = 0;
    /*05*/ virtual void RxTripCurrentTest(const Parcel& data) = 0;
    /*06*/ virtual void RxShortCircuitTest(const Parcel& data) = 0;
    /*07*/ virtual void RxOscilloscope(const Parcel& data) = 0;
    /*08*/ virtual void RxGetCalibrationCoefficients(const Parcel& data) = 0;
    /*09*/ virtual void RxSetDefaultCalibrationCoefficients(const Parcel& data) = 0;
    /*10*/ virtual void RxSetCalibrationCoefficients(const Parcel& data) = 0;
    /*11*/ virtual void RxSaveCalibrationCoefficients(const Parcel& data) = 0;
    /*12*/ virtual void RxSetAddress(const Parcel& data) = 0;
    virtual void RxBufferOverflow(const Parcel& data) = 0;
    virtual void RxWrongCommand(const Parcel& data) = 0;
    virtual void RxTextualParcel(const Parcel& data) = 0;
    virtual void RxCrcError(const Parcel& data) = 0;
    virtual void RxNullFunction(const Parcel& data) = 0;
};

class MAN2 final : public QObject, private MyProtokol, public CommonInterfaces, private CallBack {
    Q_OBJECT
    friend class SerialPort;

public:
    MAN2(QObject* parent = nullptr);
    ~MAN2() override;

    bool ping(const QString& PortName = QString()) override;
    bool isConnected() const override;

    bool getMeasuredValue(MeasuredValue& value, uint8_t channel, ValueType type = CurrentMeasuredValue);
    bool getMeasuredValue(QMap<int, MeasuredValue>& value, ValueType type = CurrentMeasuredValue);
    void startMeasure(ValueType type = CurrentMeasuredValue, uint8_t channel = 0);
    double getRmsValue();

    bool setCurrent(float Current_mA, uint8_t channel = 0);

    bool switchCurrent(uint8_t Enable, uint8_t channel = 0);
    bool switchShortCircuit(ShortCircuit scType, uint8_t channel = 0);

    bool tripCurrentTest(TripTest tt, uint8_t channel);
    bool shortCircuitTest(ShortCircuit scType, uint8_t channel);

    bool oscilloscope(uint8_t channel, Osc osc = OscIU);

    bool setAddress(uint8_t oldAddress, uint8_t newAddress);

    bool setDefaultCalibrationCoefficients(uint8_t channel);
    bool getCalibrationCoefficients(GradCoeff& gc, uint8_t channel);
    bool setCalibrationCoefficients(const GradCoeff& GradCoeff, uint8_t channel);
    bool saveToEepromCalibrationCoefficients(uint8_t channel);

    bool disableAll();

    const QMap<int, MeasuredValue>& valueMap() const { return m_valueMap; }

    int& address() { return m_address; }

signals:
    void Open();
    void Close();
    void Write(const QByteArray& data);

    void measureCompleted(const MeasuredValue&);
    void measuresCompleted(const QMap<int, MeasuredValue>& data);

    void detectedAddress(int);

private:
    SerialPort* m_port;

    int m_address = 0;

    QMap<int, MeasuredValue> m_valueMap;

    QMutex mutex;
    QThread portThread;

    int m_counter;
    mutable int crcErrCounter = 0;
    Parcel lastParcel;

    mutable QSemaphore semaphore;
    mutable bool result;
    static constexpr int delayMs[] { 1000, 1000, 10000, 10000, 1000 };
    static constexpr int delayMsSm[] { 1000, 500 };

    //    void Init();
    inline void reset() const;

    // CallBack interface

    void RxPing(const Parcel& data) override;
    void RxReadMeasuredValue(const Parcel& data) override;
    void RxSetCurrent(const Parcel& data) override;
    void RxSwitchCurrent(const Parcel& data) override;
    void RxSwitchShortCircuit(const Parcel& data) override;
    void RxTripCurrentTest(const Parcel& data) override;
    void RxShortCircuitTest(const Parcel& data) override;
    void RxOscilloscope(const Parcel& data) override;
    void RxGetCalibrationCoefficients(const Parcel& data) override;
    void RxSetDefaultCalibrationCoefficients(const Parcel& data) override;
    void RxSetCalibrationCoefficients(const Parcel& data) override;
    void RxSaveCalibrationCoefficients(const Parcel& data) override;
    void RxSetAddress(const Parcel& data) override;
    void RxBufferOverflow(const Parcel& data) override;
    void RxWrongCommand(const Parcel& data) override;
    void RxTextualParcel(const Parcel& data) override;
    void RxCrcError(const Parcel& data) override;
    void RxNullFunction(const Parcel& data) override;
};

#endif // MY_PROTOCOL_H
