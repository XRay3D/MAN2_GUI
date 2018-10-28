#include "graduation.h"

#include "hw/interface.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>

Graduation::Graduation(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    for (QSerialPortInfo& info : QSerialPortInfo::availablePorts())
        cbxScpi->addItem(info.portName());
    m_chbxs = QVector<QCheckBox*>({ checkBox_1, checkBox_2, checkBox_3, checkBox_4, checkBox_5, checkBox_6 });

    connect(pbSave, &QPushButton::clicked, [&]() { mi::man->SaveCalibrationCoefficients(m_channel); });

    connect(pbPingScpi, &QPushButton::clicked, [&]() {
        if (mi::scpi->Ping(cbxScpi->currentText()))
            QMessageBox::information(this, "", "Ок!");
        else
            QMessageBox::warning(this, "", "Нет связи!");
        groupBoxGrad->setEnabled(mi::man->IsConnected() && mi::scpi->IsConnected());
    });

    connect(cbManCh, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int index) {
        if (index == 8)
            rbVoltage->setChecked(true);
        rbCurrent->setEnabled(index != 8);
    });
}

Graduation::~Graduation()
{
    qDebug() << "~Graduation()";
}

void Graduation::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    groupBoxGrad->setEnabled(mi::man->IsConnected() && mi::scpi->IsConnected());
}

void Graduation::on_pbStartGrad_clicked()
{
    for (QCheckBox* chbx : m_chbxs)
        chbx->setChecked(false);

    MeasuredValue_t value;
    GradCoeff_t GradCoeff;

    const int count = 10;

    m_channel = cbManCh->currentIndex() + 1;

    mi::man->ShortCircuitTest(false, m_channel);
    mi::man->SwitchCurrent(false, m_channel);
    mi::man->Oscilloscope(0);

    if (rbVoltage->isChecked()) {
        if (m_channel == 9) {

            enum {
                Meas1,
                Meas2
            };

            double measure[2] = { 0.0, 0.0 };
            double reference[2] = { 0.0, 0.0 };
            const int count2 = 50;

            mi::man->SetDefaultCalibrationCoefficients(m_channel);

            m_chbxs[0]->setChecked(true);
            m_chbxs[1]->setChecked(true);

            QMessageBox::information(this, "", "Подайте на вход ~50В");
            qApp->processEvents();

            for (int i = 0, counter = 1; i < count; ++i, ++counter) {
                for (int j = 0; j < count2; ++j, ++counter) {
                    measure[Meas1] += mi::man->GetRmsValue();
                    thread()->msleep(10);
                    dsbxMeasure_1->setValue(measure[Meas1] / counter);
                    qApp->processEvents();
                }
                reference[Meas1] += mi::scpi->GetAcVoltage();
                dsbxSet_1->setValue(reference[Meas1] / (i + 1));
                qApp->processEvents();
            }
            measure[Meas1] /= count * count2;
            reference[Meas1] /= count;

            m_chbxs[2]->setChecked(true);
            m_chbxs[3]->setChecked(true);

            QMessageBox::information(this, "", "Подайте на вход ~250В");
            qApp->processEvents();
            for (int i = 0, counter = 1; i < count; ++i, ++counter) {
                for (int j = 0; j < count2; ++j, ++counter) {
                    measure[Meas2] += mi::man->GetRmsValue();
                    thread()->msleep(10);
                    dsbxMeasure_1->setValue(measure[Meas2] / counter);
                    qApp->processEvents();
                }
                reference[Meas2] += mi::scpi->GetAcVoltage();
                dsbxSet_1->setValue(reference[Meas2] / (i + 1));
                qApp->processEvents();
            }
            measure[Meas2] /= count * count2;
            reference[Meas2] /= count;

            m_chbxs[4]->setChecked(true);

            GradCoeff.AdcCh1Scale = ((reference[Meas2] - reference[Meas1]) / (measure[Meas2] - measure[Meas1]));
            GradCoeff.AdcCh1Offset = reference[Meas1] - (measure[Meas1] * GradCoeff.AdcCh1Scale);

            qDebug() << GradCoeff.AdcCh1Offset;
            qDebug() << GradCoeff.AdcCh1Scale;

            //            if ((1.5f < GradCoeff.AdcCh1Scale) && (GradCoeff.AdcCh1Scale < 2.5f))
            mi::man->SetCalibrationCoefficients(GradCoeff, m_channel);
            mi::man->SaveCalibrationCoefficients(m_channel);
            //            else
            //                QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");

            m_chbxs[5]->setChecked(true);
        } else {
            enum {
                Ch1,
                Ch2
            };

            double measure[2] = { 0.0, 0.0 };
            double reference = 0.0;

            mi::man->GetCalibrationCoefficients(GradCoeff, m_channel);

            m_chbxs[0]->setChecked(true);
            m_chbxs[1]->setChecked(true);
            m_chbxs[2]->setChecked(true);
            m_chbxs[3]->setChecked(true);

            qApp->processEvents();
            for (int i = 0; i < count; ++i) {
                mi::man->GetMeasuredValue(value, m_channel, CALIB_VOLTAGE);
                measure[Ch1] += value.Value1;
                measure[Ch2] += value.Value2;
                qApp->processEvents();
                reference += mi::scpi->GetDcVoltage();
                dsbxMeasure_1->setValue(measure[Ch1] / (i + 1));
                dsbxMeasure_2->setValue(measure[Ch2] / (i + 1));
                dsbxSet_1->setValue(reference / (i + 1));
                qApp->processEvents();
            }
            measure[Ch1] /= count;
            measure[Ch2] /= count;
            reference /= count;
            m_chbxs[4]->setChecked(true);

            GradCoeff.AdcCh1Scale = 0.05f * (reference / measure[Ch1]);
            GradCoeff.AdcCh2Scale = 0.05f * (reference / measure[Ch2]);
            GradCoeff.AdcCh1Offset = 0.0f;
            GradCoeff.AdcCh2Offset = 0.0f;

            qDebug() << GradCoeff.AdcCh1Scale;
            qDebug() << GradCoeff.AdcCh2Scale;

            if ((0.04f < GradCoeff.AdcCh1Scale) && (GradCoeff.AdcCh1Scale < 0.06f) && (0.04f < GradCoeff.AdcCh2Scale) && (GradCoeff.AdcCh2Scale < 0.06f))
                mi::man->SetCalibrationCoefficients(GradCoeff, m_channel);
            else
                QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");

            m_chbxs[5]->setChecked(true);
        }
    } else if (rbCurrent->isChecked()) {
        enum {
            Meas1,
            Meas2
        };
        double measure[2] = { 0.0, 0.0 };
        double reference[2] = { 0.0, 0.0 };

        QMessageBox::information(this, "", "Подключите источник тока 3 ампера.");

        mi::man->GetCalibrationCoefficients(GradCoeff, m_channel);
        mi::man->SetDefaultCalibrationCoefficients(m_channel);

        m_chbxs[0]->setChecked(true);
        m_chbxs[1]->setChecked(true);

        const double i1 = 0.08;
        const double i2 = 3.0;

        //0.2 A
        mi::man->SetCurrent(i1 * 1000, m_channel);
        mi::man->SwitchCurrent(true, m_channel);
        thread()->sleep(10);
        mi::man->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
        qApp->processEvents();
        for (int i = 0; i < count; ++i) {
            mi::man->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
            measure[Meas1] += value.Value1;
            reference[Meas1] += mi::scpi->GetDcCurrent();
            dsbxMeasure_1->setValue(measure[Meas1] / (i + 1));
            dsbxSet_1->setValue(reference[Meas1] / (i + 1));
            qApp->processEvents();
        }
        measure[Meas1] /= count;
        reference[Meas1] /= count;

        m_chbxs[2]->setChecked(true);
        m_chbxs[3]->setChecked(true);
        //3 A
        mi::man->SetCurrent(i2 * 1000, m_channel);
        thread()->sleep(10);
        mi::man->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
        qApp->processEvents();
        for (int i = 0; i < count; ++i) {
            mi::man->GetMeasuredValue(value, m_channel, CALIB_CURRENT);
            measure[Meas2] += value.Value1;
            reference[Meas2] += mi::scpi->GetDcCurrent();
            dsbxMeasure_1->setValue(measure[Meas2] / (i + 1));
            dsbxSet_1->setValue(reference[Meas2] / (i + 1));
            qApp->processEvents();
        }
        measure[Meas2] /= count;
        reference[Meas2] /= count;

        mi::man->SetCurrent(0, m_channel);
        mi::man->SwitchCurrent(false, m_channel);

        GradCoeff.AdcCh3Scale = ((reference[Meas2] - reference[Meas1]) / (measure[Meas2] - measure[Meas1])) * 0.01;
        GradCoeff.DacOffset = (i1 - reference[Meas1]) /*- 0.00075*/ /*- (3 - dValue2) / 2*/;
        GradCoeff.DacScale = ((i2 - i1) / (reference[Meas2] - reference[Meas1])) * 36000.0;
        m_chbxs[4]->setChecked(true);

        if ((0.009f < GradCoeff.AdcCh3Scale) && (GradCoeff.AdcCh3Scale < 0.011f)
            && (0.0f < GradCoeff.DacOffset) && (GradCoeff.DacOffset < 0.1f)
            && (35000.0f < GradCoeff.DacScale) && (GradCoeff.DacScale < 37000.0f))
            mi::man->SetCalibrationCoefficients(GradCoeff, m_channel);
        else
            QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");

        m_chbxs[5]->setChecked(true);
    }
}
