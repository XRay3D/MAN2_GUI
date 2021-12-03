#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H

#include "common_interfaces.h"
#include "config.h"
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

enum {
    AdcCount = 1,
    ManCount = CHCNT,
    ChannelCount = ManCount + AdcCount
};

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
Q_DECLARE_FLAGS(Commands, Command)

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
Q_DECLARE_FLAGS(Oscs, Osc)

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
        debug << QString("MV(%1, %2, %3)").arg(mv.valCh1, 1, 'f', 5).arg(mv.valCh2, 1, 'f', 5).arg(mv.valCh3, 1, 'f', 5).toLocal8Bit().data();
        debug.setAutoInsertSpaces(oldSetting);
        return debug.maybeSpace();
    }

    float valCh1 = 0.0;
    float valCh2 = 0.0;
    float valCh3 = 0.0;
    uint8_t type = 0;
    ManState manState;
};

#pragma pack(pop)

class MAN2;
class SerialPort;

using MeasureMap = std::map<int, MeasuredValue>;

struct CallBack {
    virtual ~CallBack() = default;

    /*00*/ virtual void RxPing(Parcel data) = 0;
    /*01*/ virtual void RxReadMeasuredValue(Parcel data) = 0;
    /*02*/ virtual void RxSetCurrent(Parcel data) = 0;
    /*03*/ virtual void RxSwitchCurrent(Parcel data) = 0;
    /*04*/ virtual void RxSwitchShortCircuit(Parcel data) = 0;
    /*05*/ virtual void RxTripCurrentTest(Parcel data) = 0;
    /*06*/ virtual void RxShortCircuitTest(Parcel data) = 0;
    /*07*/ virtual void RxOscilloscope(Parcel data) = 0;
    /*08*/ virtual void RxGetCalibrationCoefficients(Parcel data) = 0;
    /*09*/ virtual void RxSetDefaultCalibrationCoefficients(Parcel data) = 0;
    /*10*/ virtual void RxSetCalibrationCoefficients(Parcel data) = 0;
    /*11*/ virtual void RxSaveCalibrationCoefficients(Parcel data) = 0;
    /*12*/ virtual void RxSetAddress(Parcel data) = 0;
    virtual void RxBufferOverflow(Parcel data) = 0;
    virtual void RxWrongCommand(Parcel data) = 0;
    virtual void RxTextualParcel(Parcel data) = 0;
    virtual void RxCrcError(Parcel data) = 0;
    virtual void RxNullFunction(Parcel data) = 0;
};

class MAN2 final : public QObject, public MyProtokol, public CommonInterfaces, private CallBack {
    Q_OBJECT
    friend class SerialPort;

public:
    MAN2(QObject* parent = nullptr);
    ~MAN2() override;

    bool ping(const QString& PortName = QString()) override;
    bool isConnected() const override;

    bool getMeasuredValue(MeasuredValue& value, uint8_t channel, ValueType type = CurrentMeasuredValue);
    bool getMeasuredValue(MeasureMap& value, ValueType type = CurrentMeasuredValue);
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

    const MeasureMap& valueMap() const { return m_valueMap; }

signals:
    void Open();
    void Close();
    void Write(const QByteArray& data);
    void measureCompleted(const MeasuredValue&);
    void measuresCompleted(const MeasureMap& data);

private:
    SerialPort* m_port;

    MeasureMap m_valueMap;

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

    void RxPing(Parcel data) override;
    void RxReadMeasuredValue(Parcel data) override;
    void RxSetCurrent(Parcel data) override;
    void RxSwitchCurrent(Parcel data) override;
    void RxSwitchShortCircuit(Parcel data) override;
    void RxTripCurrentTest(Parcel data) override;
    void RxShortCircuitTest(Parcel data) override;
    void RxOscilloscope(Parcel data) override;
    void RxGetCalibrationCoefficients(Parcel data) override;
    void RxSetDefaultCalibrationCoefficients(Parcel data) override;
    void RxSetCalibrationCoefficients(Parcel data) override;
    void RxSaveCalibrationCoefficients(Parcel data) override;
    void RxSetAddress(Parcel data) override;
    void RxBufferOverflow(Parcel data) override;
    void RxWrongCommand(Parcel data) override;
    void RxTextualParcel(Parcel data) override;
    void RxCrcError(Parcel data) override;
    void RxNullFunction(Parcel data) override;
};

#endif // MY_PROTOCOL_H
