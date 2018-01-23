#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QThread>
#include <QSerialPort>
#include <stdint.h>
#include <QComboBox>
#include <QSemaphore>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include "common_interfaces.h"
#include "myprotokol.h"

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
    GET_RMS_MEASURED_VALUE,
    BUFFER_OVERFLOW = 0xF0,
    WRONG_COMMAND = 0xF1,
    TEXTUAL_PARCEL = 0xF2,
    CRC_ERROR = 0xF3
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
    virtual void RxPing(const QByteArray& data) = 0;
    virtual void RxGetMeasuredValue(const QByteArray& data) = 0;
    virtual void RxSetCurrent(const QByteArray& data) = 0;
    virtual void RxSwitchCurrent(const QByteArray& data) = 0;
    virtual void RxTripCurrentTest(const QByteArray& data) = 0;
    virtual void RxShortCircuitTest(const QByteArray& data) = 0;
    virtual void RxOscilloscope(const QByteArray& data) = 0;
    virtual void RxSetDefaultCalibrationCoefficients(const QByteArray& data) = 0;
    virtual void RxGetCalibrationCoefficients(const QByteArray& data) = 0;
    virtual void RxSetCalibrationCoefficients(const QByteArray& data) = 0;
    virtual void RxSaveCalibrationCoefficients(const QByteArray& data) = 0;
    virtual void RxGetMeasuredRmsValue(const QByteArray& data) = 0;
    virtual void RxBufferOverflow(const QByteArray& data) = 0;
    virtual void RxWrongCommand(const QByteArray& data) = 0;
    virtual void RxTextualParcel(const QByteArray& data) = 0;
    virtual void RxCrcError(const QByteArray& data) = 0;
    virtual void RxNullFunction(const QByteArray& data) = 0;
};

class MAN2 : public QObject, private MyProtokol, public COMMON_INTERFACES, public CallBack {
    Q_OBJECT
    friend class SerialPort;

public:
    MAN2(QObject* parent = 0);
    ~MAN2();

    bool Ping(const QString& PortName = QString());
    bool IsConnected() const;

    bool GetMeasuredValue(MeasuredValue_t& value, uint8_t channel, ValuetypeEnum type = CURRENT_MEASURED_VALUE);
    bool GetMeasuredValue(QList<MeasuredValue_t>& value, ValuetypeEnum type = CURRENT_MEASURED_VALUE);
    double GetMeasuredValueRMS();
    bool SetCurrent(float Current, uint8_t Channel = 0);
    bool SwitchCurrent(uint8_t Enable, uint8_t Channel = 0);
    bool TripCurrentTest();
    bool ShortCircuitTest(uint8_t Enable, uint8_t Channel = 0);
    bool Oscilloscope(uint8_t Channel);
    bool SetDefaultCalibrationCoefficients(uint8_t Channel);
    bool GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t Channel);
    bool SetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t Channel);
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
    //    QByteArray m_data;
    GradCoeff_t m_GradCoeff;
    MeasuredValue_t m_value;
    QMap<int, MeasuredValue_t> m_measuredValue;
    QThread m_portThread;
    QMutex m_mutex;
    int m_counter;
    mutable QSemaphore m_semaphore;
    mutable bool m_result;
    float m_rms;
    GradCoeff_t m_rmsScale;
    void Init();
    void Reset();
    void RxPing(const QByteArray& data);
    void RxGetMeasuredValue(const QByteArray& data);
    void RxSetCurrent(const QByteArray& data);
    void RxSwitchCurrent(const QByteArray& data);
    void RxTripCurrentTest(const QByteArray& data);
    void RxShortCircuitTest(const QByteArray& data);
    void RxOscilloscope(const QByteArray& data);
    void RxSetDefaultCalibrationCoefficients(const QByteArray& data);
    void RxGetCalibrationCoefficients(const QByteArray& data);
    void RxSetCalibrationCoefficients(const QByteArray& data);
    void RxSaveCalibrationCoefficients(const QByteArray& data);
    void RxGetMeasuredRmsValue(const QByteArray& data);
    void RxBufferOverflow(const QByteArray& data);
    void RxWrongCommand(const QByteArray& data);
    void RxTextualParcel(const QByteArray& data);
    void RxCrcError(const QByteArray& data);
    void RxNullFunction(const QByteArray& data);

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
    typedef void (MAN2::*func)(const QByteArray&);
    QVector<func> m_f;

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
    int m_couter;
};

#endif // MY_PROTOCOL_H
