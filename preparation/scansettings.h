#ifndef INPUTPARAMETERS_H
#define INPUTPARAMETERS_H

#include <QWidget>

class ScanSettings {
public:
    explicit ScanSettings(const QList<QString>& list = QList<QString>())
    {
        bool ok;
        if (list.size() > 16) {
            Type = list.at(0.0);
            Cipher = list.at(1);
            RatedVoltage = list.at(2).toDouble(&ok);
            NumberOfChannels = list.at(3).toInt(&ok);
            RatedCurrent = list.at(4).toDouble(&ok);
            RestrictionTest2 = list.at(5).toDouble(&ok);
            VisualControl = list.at(6).toDouble(&ok);
            LimitationsTest4_5 = list.at(7).toDouble(&ok);
            LimitTest6 = list.at(8).toDouble(&ok);
            RestrictionsTest7Min = list.at(9).toDouble(&ok);
            RestrictionsTest7Max = list.at(10).toDouble(&ok);
            Voltageerrortest3_4U1 = list.at(11).toDouble(&ok);
            Voltageerrortest3_4U2 = list.at(12).toDouble(&ok);
            Voltageerrortest5U1 = list.at(13).toDouble(&ok);
            Voltageerrortest5U2 = list.at(14).toDouble(&ok);
            VoltageErrorTest7 = list.at(15).toDouble(&ok);
            ParameterDLTest7 = list.at(16).toDouble(&ok);
        }
    }

    QString Fio;
    QString Type;
    QString Cipher;
    double RatedVoltage = 0.0;
    int NumberOfChannels = 0;
    double RatedCurrent = 0.0;
    double RestrictionTest2 = 0.0;
    double VisualControl = 0.0;
    double LimitationsTest4_5 = 0.0;
    double LimitTest6 = 0.0;
    double RestrictionsTest7Min = 0.0;
    double RestrictionsTest7Max = 0.0;
    double Voltageerrortest3_4U1 = 0.0;
    double Voltageerrortest3_4U2 = 0.0;
    double Voltageerrortest5U1 = 0.0;
    double Voltageerrortest5U2 = 0.0;
    double VoltageErrorTest7 = 0.0;
    double ParameterDLTest7 = 0.0;

    //    ScanSettings& operator=(const ScanSettings& right)
    //    {
    //        //проверка на самоприсваивание
    //        if (this == &right) {
    //            return *this;
    //        }
    //        Fio = right.Fio;
    //        Type = right.Type;
    //        Cipher = right.Cipher;
    //        RatedVoltage = right.RatedVoltage;
    //        NumberOfChannels = right.NumberOfChannels;
    //        RatedCurrent = right.RatedCurrent;
    //        RestrictionTest2 = right.RestrictionTest2;
    //        VisualControl = right.VisualControl;
    //        LimitationsTest4_5 = right.LimitationsTest4_5;
    //        LimitTest6 = right.LimitTest6;
    //        RestrictionsTest7Min = right.RestrictionsTest7Min;
    //        RestrictionsTest7Max = right.RestrictionsTest7Max;
    //        Voltageerrortest3_4U1 = right.Voltageerrortest3_4U1;
    //        Voltageerrortest3_4U2 = right.Voltageerrortest3_4U2;
    //        Voltageerrortest5U1 = right.Voltageerrortest5U1;
    //        Voltageerrortest5U2 = right.Voltageerrortest5U2;
    //        VoltageErrorTest7 = right.VoltageErrorTest7;
    //        ParameterDLTest7 = right.ParameterDLTest7;
    //        return *this;
    //    }
};

Q_DECLARE_METATYPE(ScanSettings)

#endif // INPUTPARAMETERS_H
