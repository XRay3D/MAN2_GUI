#ifndef INPUTPARAMETERS_H
#define INPUTPARAMETERS_H

#include <QSignalMapper>
#include <QWidget>
#include "ui_inputparameters.h"

class ScanSettings {
public:
    ScanSettings(const QList<QString>& list = QList<QString>())
        : Type(list.size() >= 17 ? list.at(0.0) : "")
        , Cipher(list.size() >= 17 ? list.at(1) : "")
        , RatedVoltage(list.size() >= 17 ? list.at(2).toDouble(/*&ok*/) : 0.0)
        , NumberOfChannels(list.size() >= 17 ? list.at(3).toInt(/*&ok*/) : 0.0)
        , RatedCurrent(list.size() >= 17 ? list.at(4).toDouble(/*&ok*/) : 0.0)
        , RestrictionTest2(list.size() >= 17 ? list.at(5).toDouble(/*&ok*/) : 0.0)
        , VisualControl(list.size() >= 17 ? list.at(6).toDouble(/*&ok*/) : 0.0)
        , LimitationsTest4_5(list.size() >= 17 ? list.at(7).toDouble(/*&ok*/) : 0.0)
        , LimitTest6(list.size() >= 17 ? list.at(8).toDouble(/*&ok*/) : 0.0)
        , RestrictionsTest7Min(list.size() >= 17 ? list.at(9).toDouble(/*&ok*/) : 0.0)
        , RestrictionsTest7Max(list.size() >= 17 ? list.at(10).toDouble(/*&ok*/) : 0.0)
        , Voltageerrortest3_4U1(list.size() >= 17 ? list.at(11).toDouble(/*&ok*/) : 0.0)
        , Voltageerrortest3_4U2(list.size() >= 17 ? list.at(12).toDouble(/*&ok*/) : 0.0)
        , Voltageerrortest5U1(list.size() >= 17 ? list.at(13).toDouble(/*&ok*/) : 0.0)
        , Voltageerrortest5U2(list.size() >= 17 ? list.at(14).toDouble(/*&ok*/) : 0.0)
        , VoltageErrorTest7(list.size() >= 17 ? list.at(15).toDouble(/*&ok*/) : 0.0)
        , ParameterDLTest7(list.size() >= 17 ? list.at(16).toDouble(/*&ok*/) : 0.0)
    {
    }

    QString Fio;
    QString Type;
    QString Cipher;
    double RatedVoltage;
    int NumberOfChannels;
    double RatedCurrent;
    double RestrictionTest2;
    double VisualControl;
    double LimitationsTest4_5;
    double LimitTest6;
    double RestrictionsTest7Min;
    double RestrictionsTest7Max;
    double Voltageerrortest3_4U1;
    double Voltageerrortest3_4U2;
    double Voltageerrortest5U1;
    double Voltageerrortest5U2;
    double VoltageErrorTest7;
    double ParameterDLTest7;

    ScanSettings& operator=(const ScanSettings& right)
    {
        //проверка на самоприсваивание
        if (this == &right) {
            return *this;
        }
        Fio = right.Fio;
        Type = right.Type;
        Cipher = right.Cipher;
        RatedVoltage = right.RatedVoltage;
        NumberOfChannels = right.NumberOfChannels;
        RatedCurrent = right.RatedCurrent;
        RestrictionTest2 = right.RestrictionTest2;
        VisualControl = right.VisualControl;
        LimitationsTest4_5 = right.LimitationsTest4_5;
        LimitTest6 = right.LimitTest6;
        RestrictionsTest7Min = right.RestrictionsTest7Min;
        RestrictionsTest7Max = right.RestrictionsTest7Max;
        Voltageerrortest3_4U1 = right.Voltageerrortest3_4U1;
        Voltageerrortest3_4U2 = right.Voltageerrortest3_4U2;
        Voltageerrortest5U1 = right.Voltageerrortest5U1;
        Voltageerrortest5U2 = right.Voltageerrortest5U2;
        VoltageErrorTest7 = right.VoltageErrorTest7;
        ParameterDLTest7 = right.ParameterDLTest7;
        return *this;
    }
};

class MainWindow;

class InputParameters : public QWidget, private Ui::InputParameters {
    Q_OBJECT

    friend class MainWindow;

public:
    explicit InputParameters(QWidget* parent = 0);
    ~InputParameters();

signals:
    void CurrentTabIndex(int);
    void ScanSettingsSignal(const ScanSettings*);
    void SerialNumberChanged(const QString&, int);

private slots:
    void on_pbClearSerialNumber_clicked();
    void on_pbStatrtMeasure_clicked();
    void on_cbDevice_currentIndexChanged(int index);

private:
    void SerialNumberDeviceEditingFinished(int num);

    QList<QLineEdit*> m_listLeSerNumDevice;
    QSignalMapper* smSerialNumberDevice;
    QVector<ScanSettings*> m_listScanSettings;
};

#endif // INPUTPARAMETERS_H
