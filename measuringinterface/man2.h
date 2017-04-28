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

enum COMMAND {
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
    BUFFER_OVERFLOW = 0xDF,
    WRONG_COMMAND = 0xEF,
    TEXTUAL_PARCEL = 0xFF,
    CRC_ERROR = 0xCF
};

enum CALIB_e {
    VOLTAGE_1,
    VOLTAGE_2,
    CURRENT,
    TEMPERATURE,
    OVERHEAT_TEMPERATURE, //for overheat protection
};

typedef enum {
    CURRENT_MEASURED_VALUE, // Напряжение, ток и уставка.
    VALUE_TRIP_CURRENT,
    CALIB_VOLTAGE,
    CALIB_CURRENT,
    RAW_DATA
} ValueType_t;

#pragma pack(push, 1)

typedef struct GradCoeff_t {
    float AdcCh1Offset;
    float AdcCh1Scale;
    float AdcCh2Offset;
    float AdcCh2Scale;
    float AdcCh3Offset;
    float AdcCh3Scale;
    float DacOffset;
    float DacScale;
    uint8_t Crc;
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

class MAN2 : public QObject, private MyProtokol, public COMMON_INTERFACES {
    Q_OBJECT
public:
    MAN2(QObject* parent = 0);
    ~MAN2();

    bool IsOpen();

    bool Ping(const QString& PortName = QString());
    bool IsConnected() const;

    bool GetMeasuredValue(MeasuredValue_t& value, uint8_t channel = 0, ValueType_t type = CURRENT_MEASURED_VALUE);
    bool GetMeasuredValue(QList<MeasuredValue_t>& value, uint8_t channel = 0, ValueType_t type = CURRENT_MEASURED_VALUE);
    bool SetCurrent(float Current, uint8_t Channel = 0);
    bool SwitchCurrent(uint8_t Enable, uint8_t Channel = 0);
    bool TripCurrentTest();
    bool ShortCircuitTest(uint8_t Enable, uint8_t Channel = 0);
    bool Oscilloscope(uint8_t Channel);
    bool SetDefaultCalibrationCoefficients(uint8_t Channel);
    bool GetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t Channel);
    bool SetCalibrationCoefficients(GradCoeff_t& GradCoeff, uint8_t Channel);
    bool SaveCalibrationCoefficients(uint8_t Channel);

public slots:
    void GetMeasuredValueSlot(ValueType_t type = CURRENT_MEASURED_VALUE, uint8_t channel = 0);

signals:
    void Write(const QByteArray& m_data);
    void SetPortName(const QString& m_data);
    void Open(int mode);
    void Close();
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
    QSemaphore m_semaphore;
    bool m_connected;
    bool m_result;
    QElapsedTimer m_timer;

    void CallbackCmdPing(const QByteArray& m_data);
    void CallbackCmdGetMeasuredValue(const QByteArray& m_data);
    void CallbackCmdSetCurrent(const QByteArray& m_data);
    void CallbackCmdSwitchCurrent(const QByteArray& m_data);
    void CallbackCmdTripCurrentTest(const QByteArray& m_data);
    void CallbackCmdShortCircuitTest(const QByteArray& m_data);
    void CallbackCmdOscilloscope(const QByteArray& m_data);
    void CallbackCmdSetDefaultCalibrationCoefficients(const QByteArray& m_data);
    void CallbackCmdGetCalibrationCoefficients(const QByteArray& m_data);
    void CallbackCmdSetCalibrationCoefficients(const QByteArray& m_data);
    void CallbackCmdSaveCalibrationCoefficients(const QByteArray& m_data);
    void CallbackCmdBufferOverflow(const QByteArray& m_data);
    void CallbackCmdWrongCommand(const QByteArray& m_data);
    void CallbackCmdTextualParcel(const QByteArray& m_data);
    void CallbackCmdCrcError(const QByteArray& m_data);
    void CallbackCmdNullFunction(const QByteArray& m_data);

    // COMMON_INTERFACES interface
};

class SerialPort : public QSerialPort, private MyProtokol {
    Q_OBJECT

public:
    SerialPort(MAN2* manInterface);
    ~SerialPort();
    bool Open(int mode);
    bool m_isOpen;

    MAN2* m_manInterface;
    typedef void (MAN2::*pFunc)(const QByteArray&);
    pFunc m_cmdArray[0x100];
    void (MAN2::*m_dataReady)(const QByteArray&);

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
    int m_couter;
};

#endif // MY_PROTOCOL_H
