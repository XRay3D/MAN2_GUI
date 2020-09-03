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

enum Command {
    Ping,
    GetMeasuredValue,
    SetCurrent,
    SwitchCurrent,
    TripCurrentTest,
    ShortCircuitTest,
    Oscilloscope,
    SetDefaultCalibCoeffi,
    GetCalibCoeff,
    SetCalibCoeff,
    SaveCalibCoeff,
    SetAddress,
    GetRmsMeasuredValue,
    BufferOverflow = 0xF0,
    WrongCommand = 0xF1,
    TextualParcel = 0xF2,
    CrcError = 0xF3
};

enum ShortCircuit {
    ScOff,
    ScGnd,
    ScShunt,
    ScGndAndShunt
};
//enum CalibEnum {
//    VOLTAGE_1,
//    VOLTAGE_2,
//    CURRENT,
//    TEMPERATURE,
//    OVERHEAT_TEMPERATURE, //for overheat protection
//};

enum ValueType {
    CurrentMeasuredValue, // Напряжение, ток и уставка.
    ValueTripCurrent,
    CalibVoltage,
    CalibCurrent,
    RawData
};

#pragma pack(push, 1)

struct GradCoeff {
    float AdcCh1Offset = 0.0;
    float AdcCh1Scale = 0.0;
    float AdcCh2Offset = 0.0;
    float AdcCh2Scale = 0.0;
    float AdcCh3Offset = 0.0;
    float AdcCh3Scale = 0.0;
    float DacOffset = 0.0;
    float DacScale = 0.0;
    uint8_t Address = 0;
    uint8_t Crc = 0;
}; //0x00

struct ManState {
    uint8_t Load : 1;
    uint8_t ShortCircuit : 2;
    uint8_t OverHeat : 1;
    uint8_t Oscilloscope : 1;
    uint8_t TripCurrentTest : 1;
    uint8_t Dummy : 2;
};

struct MeasuredValue {
    float value1 = 0.0;
    float value2 = 0.0;
    float value3 = 0.0;
    uint8_t type = 0;
    ManState manState;
};

#pragma pack(pop)

class MAN2;
class SerialPort;

class CallBack {
public:
    virtual ~CallBack() = default;
    virtual void RxPing(const Parcel& data) = 0;
    virtual void RxGetMeasuredValue(const Parcel& data) = 0;
    virtual void RxSetCurrent(const Parcel& data) = 0;
    virtual void RxSwitchCurrent(const Parcel& data) = 0;
    virtual void RxTripCurrentTest(const Parcel& data) = 0;
    virtual void RxShortCircuitTest(const Parcel& data) = 0;
    virtual void RxOscilloscope(const Parcel& data) = 0;
    virtual void RxSetDefaultCalibCoeff(const Parcel& data) = 0;
    virtual void RxGetCalibCoeff(const Parcel& data) = 0;
    virtual void RxSetCalibCoeff(const Parcel& data) = 0;
    virtual void RxSaveCalibCoeff(const Parcel& data) = 0;
    virtual void RxSetAddress(const Parcel& data) = 0;
    virtual void RxGetRmsValue(const Parcel& data) = 0;
    virtual void RxBufferOverflow(const Parcel& data) = 0;
    virtual void RxWrongCommand(const Parcel& data) = 0;
    virtual void RxTextualParcel_t(const Parcel& data) = 0;
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

    bool getMeasuredValue(MeasuredValue& value, uint8_t channel, ValueType type = CurrentMeasuredValue);
    bool getMeasuredValue(QVector<MeasuredValue>& value, ValueType type = CurrentMeasuredValue);
    double getRmsValue();
    bool setCurrent(float Current, uint8_t Channel = 0);
    bool switchCurrent(uint8_t Enable, uint8_t Channel = 0);
    bool tripCurrentTest();
    bool shortCircuitTest(uint8_t Enable, uint8_t Channel = 0);
    bool oscilloscope(int Channel);
    bool setAddress(uint8_t newAddress, uint8_t oldAddress);
    bool setDefaultCalibrationCoefficients(uint8_t Channel);
    bool getCalibrationCoefficients(GradCoeff& GradCoeff, uint8_t Channel);
    bool setCalibrationCoefficients(const GradCoeff& GradCoeff, uint8_t Channel);
    bool saveToEepromCalibrationCoefficients(uint8_t Channel);
    bool disableAll();
    int address() const;
    bool startTest(float start, float stop, float step, uint8_t channel);

public slots:
    void GetMeasuredValueSlot(ValueType type = CurrentMeasuredValue, uint8_t channel = 0);

signals:
    void detectedAddress(int mode);
    void Open(int mode);
    void Close();
    void Write(const QByteArray& data);
    void measCompleted(const MeasuredValue&);

private:
    SerialPort* m_port;
    GradCoeff m_coeff;
    GradCoeff m_rmsCoeff;
    MeasuredValue m_value;
    QMap<int, MeasuredValue> m_measuredValue;
    QMutex m_mutex;
    QThread m_portThread;
    float m_rms;
    int m_counter;
    mutable QSemaphore m_semaphore;
    mutable bool m_result;
    static constexpr int delayMs[]{ 1000, 1000, 10000, 10000, 1000 };
    int m_address = -1;

    void Init();
    inline void Reset();

    // CallBack interface
    void RxPing(const Parcel& data) override;
    void RxGetMeasuredValue(const Parcel& data) override;
    void RxSetCurrent(const Parcel& data) override;
    void RxSwitchCurrent(const Parcel& data) override;
    void RxTripCurrentTest(const Parcel& data) override;
    void RxShortCircuitTest(const Parcel& data) override;
    void RxOscilloscope(const Parcel& data) override;
    void RxSetDefaultCalibCoeff(const Parcel& data) override;
    void RxGetCalibCoeff(const Parcel& data) override;
    void RxSetCalibCoeff(const Parcel& data) override;
    void RxSaveCalibCoeff(const Parcel& data) override;
    void RxSetAddress(const Parcel& data) override;
    void RxGetRmsValue(const Parcel& data) override;
    void RxBufferOverflow(const Parcel& data) override;
    void RxWrongCommand(const Parcel& data) override;
    void RxTextualParcel_t(const Parcel& data) override;
    void RxCrcError(const Parcel& data) override;
    void RxNullFunction(const Parcel& data) override;
};

#endif // MY_PROTOCOL_H
