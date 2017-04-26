#include "graduation.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include "measuringinterface/measuringinterface.h"

Graduation::Graduation(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
}

Graduation::~Graduation()
{
    qDebug() << "~Graduation()";
}

void Graduation::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    if (MI::man()->IsConnected()) {
        groupBoxTest->setEnabled(true);
        groupBoxGrad->setEnabled(true);
    }
    else {
        groupBoxTest->setEnabled(false);
        groupBoxGrad->setEnabled(false);
    }
}

void Graduation::on_cbTestShortCirc_clicked(bool checked)
{
    MI::man()->ShortCircuitTest(checked, m_channel);
}

void Graduation::on_pbOscilloscope_clicked(bool checked)
{
    if (checked) {
        MI::man()->Oscilloscope(cbChannelMan->currentText().toInt());
    }
    else {
        MI::man()->Oscilloscope(0);
    }
}

void Graduation::on_pbCurrentEnable_clicked(bool checked)
{
    MI::man()->SwitchCurrent(checked, m_channel);
}

void Graduation::on_pbStartGrad_clicked()
{
    //    lblState_1->setText("");
    //    chbState_0->setCheckState(Qt::Checked);
    //    chbState_1->setChecked(false);
    //    chbState_2->setChecked(false);
    //    chbState_3->setChecked(false);
    //    chbState_4->setChecked(false);
    MeasuredValue_t value;
    GradCoeff_t GradCoeff;

    double dValue1_1 = 0.0;
    double dValue1_2 = 0.0;
    double dValue2_1 = 0.0;
    double dValue2_2 = 0.0;

    double dValue1 = 0.0;
    double dValue2 = 0.0;

    const int count = 5;

    if (radioButtonVoltage->isChecked()) {
        //        QMessageBox::information(this, "", "Подайте на вход напряжение 5 вольт.");
        MI::man()->ShortCircuitTest(false, m_channel);
        MI::man()->SwitchCurrent(false, m_channel);
        MI::man()->Oscilloscope(0);
        MI::man()->GetCalibrationCoefficients(GradCoeff, m_channel);

        //        for (int i = 0; i < count; ++i) {
        //            man->GetMeasuredValue(value, channel, CALIB_VOLTAGE);
        //            dValue1_1 += value.Value1;
        //            dValue1_2 += value.Value2;
        //            if (scpi.IsFound()) {
        //                dValue1 += scpi.GetVoltage();
        //            }
        //            //            if (dValue1_1 / (i + 1) < 3.0 || dValue1_1 / (i + 1) > 6.0 && dValue1_2 / (i + 1) < 3.0 || dValue1_2 / (i + 1) > 6.0) {
        //            //                QMessageBox::information(this, "", "Проверьте подключение!");
        //            //                man->SetCalibrationCoefficients(GradCoeff, channel);
        //            //                return;
        //            //            }
        //        }
        //        dValue1_1 /= count;
        //        dValue1_2 /= count;
        //        dValue1 /= count;
        //        if (!scpi.IsFound()) {
        //            dValue1 = QInputDialog::getDouble(this, "", "Введите точное значение поданного наапряжения.");
        //        }
        if (QMessageBox::information(this, "", "Подайте на вход напряжение 36 вольт.", QMessageBox::Ok) != QMessageBox::Ok) {
            return;
        }
        MI::man()->GetMeasuredValue(value, m_channel, CALIB_VOLTAGE);
        for (int i = 0; i < count; ++i) {
            MI::man()->GetMeasuredValue(value, m_channel, CALIB_VOLTAGE);
            dValue2_1 += value.Value1;
            dValue2_2 += value.Value2;
            if (MI::scpi()->IsConnected()) {
                dValue2 += MI::scpi()->GetVoltage();
            }
            //            if (dValue2_1 / (i + 1) < 25.0 || dValue2_1 / (i + 1) > 40.0 && dValue2_2 / (i + 1) < 25.0 || dValue2_2 / (i + 1) > 40.0) {
            //                QMessageBox::information(this, "", "Проверьте подключение!");
            //                man->SetCalibrationCoefficients(GradCoeff, channel);
            //                return;
            //            }
        }
        dValue2_1 /= count;
        dValue2_2 /= count;
        dValue2 /= count;
        if (!MI::scpi()->IsConnected()) {
            dValue2 = QInputDialog::getDouble(this, "", "Введите точное значение поданного наапряжения.");
        }

        GradCoeff.AdcCh1Scale = 0.05f * (dValue2 / dValue2_1);
        GradCoeff.AdcCh2Scale = 0.05f * (dValue2 / dValue2_2);
        GradCoeff.AdcCh1Offset = 0.0f;
        GradCoeff.AdcCh2Offset = 0.0f;

        qDebug() << GradCoeff.AdcCh1Scale;
        qDebug() << GradCoeff.AdcCh2Scale;

        if ((0.04f < GradCoeff.AdcCh1Scale) && (GradCoeff.AdcCh1Scale < 0.06f)
            && (0.04f < GradCoeff.AdcCh2Scale) && (GradCoeff.AdcCh2Scale < 0.06f)) {
            MI::man()->SetCalibrationCoefficients(GradCoeff, m_channel);
            MI::man()->SaveCalibrationCoefficients(m_channel);
        }
        else {
            QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");
        }
    }
    else if (radioButtonCurrent->isChecked()) {
        QMessageBox::information(this, "", "Подключите источник тока 3 ампера.");
        MI::man()->ShortCircuitTest(false, m_channel);
        MI::man()->Oscilloscope(0);
        MI::man()->GetCalibrationCoefficients(GradCoeff, m_channel);
        MI::man()->SetDefaultCalibrationCoefficients(m_channel);

        const double i1 = 0.08;
        const double i2 = 3.0;

        //0.2 A
        MI::man()->SetCurrent(i1, m_channel);
        MI::man()->SwitchCurrent(true, m_channel);
        thread()->sleep(10);
        MI::man()->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
        for (int i = 0; i < count; ++i) {
            MI::man()->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
            dValue1_1 += value.Value1;
            if (MI::scpi()->IsConnected()) {
                dValue1 += MI::scpi()->GetCurrent();
            }
        }
        dValue1_1 /= count;
        dValue1 /= count;
        if (!MI::scpi()->IsConnected()) {
            dValue1 = QInputDialog::getDouble(this, "", "Введите точное значение измеренного тока.");
        }

        //3 A
        MI::man()->SetCurrent(3, m_channel);
        thread()->sleep(10);
        MI::man()->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
        for (int i = 0; i < count; ++i) {
            MI::man()->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
            dValue1_2 += value.Value1;
            if (MI::scpi()->IsConnected()) {
                dValue2 += MI::scpi()->GetCurrent();
            }
            if (dValue2 / (i + 1) < i1 * 0.8) {
                QMessageBox::information(this, "", "Источник тока не выдаёт 3 А.");
                MI::man()->SetCalibrationCoefficients(GradCoeff, m_channel);
                MI::man()->SwitchCurrent(false, m_channel);
                return;
            }
        }
        dValue1_2 /= count;
        dValue2 /= count;
        if (!MI::scpi()->IsConnected()) {
            dValue2 = QInputDialog::getDouble(this, "", "Введите точное значение измеренного тока.");
        }
        MI::man()->SwitchCurrent(false, m_channel);

        GradCoeff.AdcCh3Scale = ((dValue2 - dValue1) / (dValue1_2 - dValue1_1)) * 0.01;
        GradCoeff.DacOffset = (i1 - dValue1) /*- 0.00075*/ /*- (3 - dValue2) / 2*/;
        GradCoeff.DacScale = ((i2 - i1) / (dValue2 - dValue1)) * 36000.0;

        MI::man()->SetCalibrationCoefficients(GradCoeff, m_channel);
        //        man->SaveCalibrationCoefficients(channel);
        if ((0.009f < GradCoeff.AdcCh3Scale) && (GradCoeff.AdcCh3Scale < 0.011f)
            && (0.0f < GradCoeff.DacOffset) && (GradCoeff.DacOffset < 0.1f)
            && (35000.0f < GradCoeff.DacScale) && (GradCoeff.DacScale < 37000.0f)) {
            MI::man()->SetCalibrationCoefficients(GradCoeff, m_channel);
            MI::man()->SaveCalibrationCoefficients(m_channel);
        }
        else {
            QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");
        }
    }
}

void Graduation::on_dsbSetCurrent_valueChanged(double arg1)
{
    MI::man()->SetCurrent(arg1, m_channel);
}

void Graduation::on_pbMeasure_clicked()
{
    MeasuredValue_t value;
    MI::man()->GetMeasuredValue(value, m_channel, RAW_DATA);
    qDebug() << "Value1" << value.Value1;
    qDebug() << "Value2" << value.Value2;
    qDebug() << "Value3" << value.Value3;
}

void Graduation::on_pbGradRead_clicked()
{
    GradCoeff_t GradCoeff;
    MI::man()->GetCalibrationCoefficients(GradCoeff, m_channel);
    qDebug() << "AdcCh1Offset" << GradCoeff.AdcCh1Offset;
    qDebug() << "AdcCh1Scale" << GradCoeff.AdcCh1Scale;
    qDebug() << "AdcCh2Offset" << GradCoeff.AdcCh2Offset;
    qDebug() << "AdcCh2Scale" << GradCoeff.AdcCh2Scale;
    qDebug() << "AdcCh3Offset" << GradCoeff.AdcCh3Offset;
    qDebug() << "AdcCh3Scale" << GradCoeff.AdcCh3Scale;
    qDebug() << "DacOffset" << GradCoeff.DacOffset;
    qDebug() << "DacScale" << GradCoeff.DacScale;
    qDebug() << "Crc" << GradCoeff.Crc;
}

void Graduation::on_pbCheckConnectionScpi_clicked()
{
    qDebug() << MI::scpi()->Ping(cbScpi->currentText());
    //    qDebug() << scpi.GetCurrent();
    //    qDebug() << scpi.GetVoltage();
}

void Graduation::on_pbSave_clicked()
{
    GradCoeff_t GradCoeff;
    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginGroup("Channel" + cbChannelMan->currentText());
    MI::man()->GetCalibrationCoefficients(GradCoeff, m_channel);
    settings.setValue("AdcCh1Offset", 1.0 * GradCoeff.AdcCh1Offset);
    settings.setValue("AdcCh1Scale", 1.0 * GradCoeff.AdcCh1Scale);
    settings.setValue("AdcCh2Offset", 1.0 * GradCoeff.AdcCh2Offset);
    settings.setValue("AdcCh2Scale", 1.0 * GradCoeff.AdcCh2Scale);
    settings.setValue("AdcCh3Offset", 1.0 * GradCoeff.AdcCh3Offset);
    settings.setValue("AdcCh3Scale", 1.0 * GradCoeff.AdcCh3Scale);
    settings.setValue("DacOffset", 1.0 * GradCoeff.DacOffset);
    settings.setValue("DacScale", 1.0 * GradCoeff.DacScale);
    settings.endGroup();
}

void Graduation::on_pbLoad_clicked()
{
    GradCoeff_t GradCoeff;
    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginGroup("Channel" + cbChannelMan->currentText());
    GradCoeff.AdcCh1Offset = settings.value("AdcCh1Offset", 0.0).toFloat();
    GradCoeff.AdcCh1Scale = settings.value("AdcCh1Scale", 0.05).toFloat();
    GradCoeff.AdcCh2Offset = settings.value("AdcCh2Offset", 0.0).toFloat();
    GradCoeff.AdcCh2Scale = settings.value("AdcCh2Scale", 0.05).toFloat();
    GradCoeff.AdcCh3Offset = settings.value("AdcCh3Offset", 0.0).toFloat();
    GradCoeff.AdcCh3Scale = settings.value("AdcCh3Scale", 0.01).toFloat();
    GradCoeff.DacOffset = settings.value("DacOffset", 0.0).toFloat();
    GradCoeff.DacScale = settings.value("DacScale", 36000.0).toFloat();
    settings.endGroup();
    MI::man()->SetCalibrationCoefficients(GradCoeff, m_channel);
    MI::man()->SaveCalibrationCoefficients(m_channel);
}

void Graduation::on_cbChannelMan_currentIndexChanged(int index)
{
    m_channel = index;
}
