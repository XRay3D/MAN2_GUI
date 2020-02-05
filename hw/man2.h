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
enum CommandEnum {
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
    GetRmsMeasuredValue,
    BufferOverflow,
    WrongCommand,
    TextualParcel,
    CrcError
};

//enum CalibEnum {
//    VOLTAGE_1,
//    VOLTAGE_2,
//    CURRENT,
//    TEMPERATURE,
//    OVERHEAT_TEMPERATURE, //for overheat protection
//};

enum ValuetypeEnum {
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
    uint8_t Crc = 0;
}; //0x00

struct ManState {
    uint8_t Load : 1;
    uint8_t ShortCircuit : 1;
    uint8_t OverHeat : 1;
    uint8_t Oscilloscope : 1;
    uint8_t TripCurrentTest : 1;
    uint8_t Dummy : 3;
};

struct MeasuredValue {
    MeasuredValue()
        : Value1(-9999.0)
        , Value2(-9999.0)
        , Value3(-9999.0)
        , Type(0)
    {
    }
    float Value1;
    float Value2;
    float Value3;
    uint8_t Type;
    ManState ManState;
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
    virtual void RxGetRmsValue(const Parcel& data) = 0;
    virtual void RxBufferOverflow(const Parcel& data) = 0;
    virtual void RxWrongCommand(const Parcel& data) = 0;
    virtual void RxTextualParcel_t(const Parcel& data) = 0;
    virtual void RxCrcError(const Parcel& data) = 0;
    virtual void RxNullFunction(const Parcel& data) = 0;
};

class MAN2 : public QObject, private MyProtokol, public CommonInterfaces, public CallBack {
    Q_OBJECT
    friend class SerialPort;

public:
    MAN2(QObject* parent = nullptr);
    ~MAN2() override;

    bool ping(const QString& PortName = QString()) override;

    bool getMeasuredValue(MeasuredValue& value, uint8_t channel, ValuetypeEnum type = CurrentMeasuredValue);
    bool getMeasuredValue(QList<MeasuredValue>& value, ValuetypeEnum type = CurrentMeasuredValue);
    double getRmsValue();
    bool setCurrent(float Current, uint8_t Channel = 0);
    bool switchCurrent(uint8_t Enable, uint8_t Channel = 0);
    bool tripCurrentTest();
    bool thortCircuitTest(uint8_t Enable, uint8_t Channel = 0);
    bool oscilloscope(int Channel);
    bool setDefaultCalibrationCoefficients(uint8_t Channel);
    bool getCalibrationCoefficients(GradCoeff& GradCoeff, uint8_t Channel);
    bool setCalibrationCoefficients(const GradCoeff& GradCoeff, uint8_t Channel);
    bool saveToEepromCalibrationCoefficients(uint8_t Channel);
    bool disableAll();

public slots:
    void GetMeasuredValueSlot(ValuetypeEnum type = CurrentMeasuredValue, uint8_t channel = 0);

signals:
    void Open(int mode);
    void Close();
    void Write(const QByteArray& data);
    void GetMeasuredValueSignal(const QMap<int, MeasuredValue>&);

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

    void Init();
    inline void Reset();
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
    void RxGetRmsValue(const Parcel& data) override;
    void RxBufferOverflow(const Parcel& data) override;
    void RxWrongCommand(const Parcel& data) override;
    void RxTextualParcel_t(const Parcel& data) override;
    void RxCrcError(const Parcel& data) override;
    void RxNullFunction(const Parcel& data) override;

    // COMMON_INTERFACES interface
};

class SerialPort : public QSerialPort, private MyProtokol {
    Q_OBJECT

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
    int m_couter;
};

#endif // MY_PROTOCOL_H
