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

class ManInterface;
class SerialPort;

class ManInterface : public QObject, private MyProtokol {
    Q_OBJECT
public:
    ManInterface(QObject* parent = 0);
    ~ManInterface();
    void EnumeratePorts(QComboBox* comboBox);

    bool IsOpen();
    bool IsConnected() const;

    QList<QString> Ping(const QString& PortName = "");
    bool GetMeasuredValue(MeasuredValue_t& value, uint8_t channel = 0, ValueType_t type = CURRENT_MEASURED_VALUE);
    QList<MeasuredValue_t> GetMeasuredValue(ValueType_t type = CURRENT_MEASURED_VALUE, uint8_t channel = 0);
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
    void Write(const QByteArray& data);
    void SetPortName(const QString& data);
    void Open(int mode);
    void Close();
    void GetMeasuredValueSignal(const QMap<int, MeasuredValue_t>&);

private:
    SerialPort* m_port;
    //    QByteArray m_data;
    QList<int> m_channels;
    GradCoeff_t m_GradCoeff;
    MeasuredValue_t m_value;
    QMap<int, MeasuredValue_t> m_measuredValue;
    QThread m_thread;
    QMutex m_mutex;
    int m_counter;
    QSemaphore m_semaphore;
    bool m_connected;
    QElapsedTimer m_timer;
    QList<MeasuredValue_t> m_list;

    void CmdPing(const QByteArray& data);
    void CmdGetMeasuredValue(const QByteArray& data);
    void CmdSetCurrent(const QByteArray& data);
    void CmdSwitchCurrent(const QByteArray& data);
    void CmdTripCurrentTest(const QByteArray& data);
    void CmdShortCircuitTest(const QByteArray& data);
    void CmdOscilloscope(const QByteArray& data);
    void CmdSetDefaultCalibrationCoefficients(const QByteArray& data);
    void CmdGetCalibrationCoefficients(const QByteArray& data);
    void CmdSetCalibrationCoefficients(const QByteArray& data);
    void CmdSaveCalibrationCoefficients(const QByteArray& data);
    void CmdBufferOverflow(const QByteArray& data);
    void CmdWrongCommand(const QByteArray& data);
    void CmdTextualParcel(const QByteArray& data);
    void CmdCrcError(const QByteArray& data);
    void CmdNullFunction(const QByteArray& data);
};

class SerialPort : public QSerialPort, private MyProtokol {
    Q_OBJECT

public:
    SerialPort(ManInterface* manInterface);
    ~SerialPort();
    bool Open(int mode);
    bool m_isOpen;

    ManInterface* m_manInterface;
    typedef void (ManInterface::*pFunc)(const QByteArray&);
    pFunc m_cmdArray[0x100];
    void (ManInterface::*m_dataReady)(const QByteArray&);

private:
    void ReadyRead();
    QByteArray m_data;
    QByteArray m_tmpData;
    QMutex m_mutex;
    int m_couter;
};

#endif // MY_PROTOCOL_H
