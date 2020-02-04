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
    PING,
    GET_MEASURED_VALUE,
    SET_CURRENT,
    SWITCH_CURRENT,
    TRIP_CURRENT_TEST,
    SHORT_CIRCUIT_TEST,
    OSCILLOSCOPE,
    SET_DEFAULT_CALIBRATION_COEFFICIENTS,
    GET_CALIBRATION_COEFFICIENTS,
    SET_CALIBRATION_COEFFICIENTS,
    SAVE_CALIBRATION_COEFFICIENTS,
    GetRmsMeasuredValue,
    BUFFER_OVERFLOW,
    WRONG_COMMAND,
    TEXTUAL_PARCEL,
    CRC_ERROR
};

enum CalibEnum {
    VOLTAGE_1,
    VOLTAGE_2,
    CURRENT,
    TEMPERATURE,
    OVERHEAT_TEMPERATURE, //for overheat protection
};

enum ValuetypeEnum {
    CURRENT_MEASURED_VALUE, // Напряжение, ток и уставка.
    VALUE_TRIP_CURRENT,
    CALIB_VOLTAGE,
    CALIB_CURRENT,
    RAW_DATA
};

#pragma pack(push, 1)

typedef struct GradCoeff_t {
    float AdcCh1Offset = 0.0;
    float AdcCh1Scale = 0.0;
    float AdcCh2Offset = 0.0;
    float AdcCh2Scale = 0.0;
    float AdcCh3Offset = 0.0;
    float AdcCh3Scale = 0.0;
    float DacOffset = 0.0;
    float DacScale = 0.0;
    uint8_t Crc = 0;
} GradCoeff_t; //0x00

typedef struct ManState_t {
    uint8_t Load : 1;
    uint8_t ShortCircuit : 1;
    uint8_t OverHeat : 1;
    uint8_t Oscilloscope : 1;
    uint8_t TripCurrentTest : 1;
    uint8_t Dummy : 3;
} ManState_t;

typedef struct MeasuredValue_t {
    MeasuredValue_t()
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
    ManState_t ManState;
} MeasuredValue_t;

#pragma pack(pop)

class MAN2;
class SerialPort;

class CallBack {
public:
    virtual void RxPing(const Parcel_t& data) = 0;
    virtual void RxGetMeasuredValue(const Parcel_t& data) = 0;
    virtual void RxSetCurrent(const Parcel_t& data) = 0;
    virtual void RxSwitchCurrent(const Parcel_t& data) = 0;
    virtual void RxTripCurrentTest(const Parcel_t& data) = 0;
    virtual void RxShortCircuitTest(const Parcel_t& data) = 0;
    virtual void RxOscilloscope(const Parcel_t& data) = 0;
    virtual void RxSetDefaultCalibrationCoefficients(const Parcel_t& data) = 0;
    virtual void RxGetCalibrationCoefficients(const Parcel_t& data) = 0;
    virtual void RxSetCalibrationCoefficients(const Parcel_t& data) = 0;
    virtual void RxSaveCalibrationCoefficients(const Parcel_t& data) = 0;
    virtual void RxGetRmsValue(const Parcel_t& data) = 0;
    virtual void RxBufferOverflow(const Parcel_t& data) = 0;
    virtual void RxWrongCommand(const Parcel_t& data) = 0;
    virtual void RxTextualParcel(const Parcel_t& data) = 0;
    virtual void RxCrcError(const Parcel_t& data) = 0;
    virtual void RxNullFunction(const Parcel_t& data) = 0;
};

class MAN2 : public QObject, private MyProtokol, public CommonInterfaces, public CallBack {
    Q_OBJECT
    friend class SerialPort;

public:
    MAN2(QObject* parent = 0);
    ~MAN2();

    bool Ping(const QString& PortName = QString());

    bool GetMeasuredValue(MeasuredValue_t& value, uint8_t channel, ValuetypeEnum type = CURRENT_MEASURED_VALUE);
    bool GetMeasuredValue(QList<MeasuredValue_t>& value, ValuetypeEnum type = CURRENT_MEASURED_VALUE);
    double GetRmsValue();
    bool SetCurrent(float Current, uint8_t Channel = 0);
    bool SwitchCurrent(uint8_t Enable, uint8_t Channel = 0);
    bool TripCurrentTest();
    bool ShortCircuitTest(uint8_t Enable, uint8_t Channel = 0);
    bool Oscilloscope(int Channel);
    bool SetDefaultCalibrationCoefficients(uint8_t Channel);
    bool GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t Channel);
    bool SetCalibrationCoefficients(const GradCoeff_t& GradCoeff, uint8_t Channel);
    bool SaveCalibrationCoefficients(uint8_t Channel);
    bool DisableAll();

public slots:
    void GetMeasuredValueSlot(ValuetypeEnum type = CURRENT_MEASURED_VALUE, uint8_t channel = 0);

signals:
    void Open(int mode);
    void Close();
    void Write(const QByteArray& data);
    void GetMeasuredValueSignal(const QMap<int, MeasuredValue_t>&);

private:
    SerialPort* m_port;
    GradCoeff_t m_coeff;
    GradCoeff_t m_rmsCoeff;
    MeasuredValue_t m_value;
    QMap<int, MeasuredValue_t> m_measuredValue;
    QMutex m_mutex;
    QThread m_portThread;
    float m_rms;
    int m_counter;
    mutable QSemaphore m_semaphore;
    mutable bool m_result;

    void Init();
    inline void Reset();
    void RxPing(const Parcel_t& data);
    void RxGetMeasuredValue(const Parcel_t& data);
    void RxSetCurrent(const Parcel_t& data);
    void RxSwitchCurrent(const Parcel_t& data);
    void RxTripCurrentTest(const Parcel_t& data);
    void RxShortCircuitTest(const Parcel_t& data);
    void RxOscilloscope(const Parcel_t& data);
    void RxSetDefaultCalibrationCoefficients(const Parcel_t& data);
    void RxGetCalibrationCoefficients(const Parcel_t& data);
    void RxSetCalibrationCoefficients(const Parcel_t& data);
    void RxSaveCalibrationCoefficients(const Parcel_t& data);
    void RxGetRmsValue(const Parcel_t& data);
    void RxBufferOverflow(const Parcel_t& data);
    void RxWrongCommand(const Parcel_t& data);
    void RxTextualParcel(const Parcel_t& data);
    void RxCrcError(const Parcel_t& data);
    void RxNullFunction(const Parcel_t& data);

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
    typedef void (MAN2::*func)(const Parcel_t&);
    QVector<func> m_f;

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
    int m_couter;
};

#endif // MY_PROTOCOL_H
