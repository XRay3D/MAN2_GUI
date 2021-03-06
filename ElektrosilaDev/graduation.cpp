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

    m_chbxs = QVector<QCheckBox*>({ checkBox_1, checkBox_2, checkBox_3, checkBox_4, checkBox_5, checkBox_6 });

    connect(pbSave, &QPushButton::clicked, []() { mi::man->saveToEepromCalibrationCoefficients(mi::man->address()); });

    connect(dsbxAdcCh1Offset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.adcCh1.offset = val; });
    connect(dsbxAdcCh1Scale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.adcCh1.scale = val; });
    connect(dsbxAdcCh2Offset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.adcCh2.offset = val; });
    connect(dsbxAdcCh2Scale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.adcCh2.scale = val; });
    connect(dsbxAdcCh3Offset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.adcCh3.offset = val; });
    connect(dsbxAdcCh3Scale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.adcCh3.scale = val; });
    connect(dsbxDacOffset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.dac.offset = val; });
    connect(dsbxDacScale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.dac.scale = val; });

    //    connect(cbManCh, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int index) {
    //        if (index == 8)
    //            rbVoltage->setChecked(true);
    //        rbCurrent->setEnabled(index != 8);
    //    });
}

Graduation::~Graduation()
{
    qDebug() << "~Graduation()";
}

void Graduation::gradI()
{
    enum {
        Meas1,
        Meas2
    };

    double measured[2] = { 0.0, 0.0 };
    double reference[2] = { 0.0, 0.0 };

    QMessageBox::information(this, "", "Подключите источник тока 3 ампера.");

    mi::man->getCalibrationCoefficients(gradCoeff, mi::man->address());
    setGradDsbxs();
    mi::man->setDefaultCalibrationCoefficients(mi::man->address());
    GradCoeff defGc;
    mi::man->getCalibrationCoefficients(defGc, mi::man->address());

    m_chbxs[0]->setChecked(true);
    m_chbxs[1]->setChecked(true);

    constexpr double i1 = 0.1; // A
    constexpr double i2 = 3.0; // A

    { //0.1 A
        mi::man->setCurrent(i1 * 1000, mi::man->address());
        mi::man->switchCurrent(true, mi::man->address());
        thread()->sleep(10);
        qApp->processEvents();
        mi::man->getMeasuredValue(value, mi::man->address(), CalibCurrent);
        for (int i = 0; i < count;) {
            mi::man->getMeasuredValue(value, mi::man->address(), CalibCurrent);
            measured[Meas1] += static_cast<double>(value.ch1);
            reference[Meas1] += mi::scpi->GetDcCurrent();
            dsbxMeasure_1->setValue(measured[Meas1] / ++i);
            dsbxSet_1->setValue(reference[Meas1] / i);
            qApp->processEvents();
        }

        measured[Meas1] /= count;
        reference[Meas1] /= count;
    }

    m_chbxs[2]->setChecked(true);
    m_chbxs[3]->setChecked(true);

    { //3 A
        mi::man->setCurrent(i2 * 1000, mi::man->address());
        thread()->sleep(10);
        qApp->processEvents();
        mi::man->getMeasuredValue(value, mi::man->address(), CalibCurrent);
        for (int i = 0; i < count;) {
            mi::man->getMeasuredValue(value, mi::man->address(), CalibCurrent);
            measured[Meas2] += static_cast<double>(value.ch1);
            reference[Meas2] += mi::scpi->GetDcCurrent();
            dsbxMeasure_1->setValue(measured[Meas2] / ++i);
            dsbxSet_1->setValue(reference[Meas2] / i);
            qApp->processEvents();
        }
        measured[Meas2] /= count;
        reference[Meas2] /= count;
    }

    mi::man->setCurrent(0, mi::man->address());
    mi::man->switchCurrent(false, mi::man->address());

    const double scale = (i2 - i1) / (reference[Meas2] - reference[Meas1]);
    gradCoeff.adcCh3.scale = static_cast<float>(((reference[Meas2] - reference[Meas1]) / (measured[Meas2] - measured[Meas1])) * defGc.adcCh3.scale);
    gradCoeff.dac.offset = 0; //static_cast<float>((i1 / scale) - reference[Meas1]);
    gradCoeff.dac.scale = static_cast<float>(scale * defGc.dac.scale);
    m_chbxs[4]->setChecked(true);

    //    qDebug() << "adcCh3.scale" << gradCoeff.adcCh3.scale;
    //    qDebug() << "dac.offset" << gradCoeff.dac.offset;
    //    qDebug() << "dac.scale" << gradCoeff.dac.scale;

    //    if ((0.009f < gradCoeff.adcCh3.scale) && (gradCoeff.adcCh3.scale < 0.011f)
    //        /*&& (0.0f < gradCoeff.dac.offset) && (gradCoeff.dac.offset < 0.3f)*/
    //        /*&& (defDacScale * 0.9 < gradCoeff.dac.scale) && (gradCoeff.dac.scale < defDacScale * 1.1)*/)
    mi::man->setCalibrationCoefficients(gradCoeff, mi::man->address());
    setGradDsbxs();
    //    else
    //        QMessageBox::critical(this, "", "Коэффициенты выходят за пределы!");

    m_chbxs[5]->setChecked(true);
}

void Graduation::gradU()
{
    enum {
        Ch1,
        Ch2
    };

    double measure[2] = { 0.0, 0.0 };
    double reference = 0.0;

    mi::man->getCalibrationCoefficients(gradCoeff, mi::man->address());
    setGradDsbxs();
    //mi::man->setDefaultCalibrationCoefficients(mi::man->address());

    m_chbxs[0]->setChecked(true);
    m_chbxs[1]->setChecked(true);
    m_chbxs[2]->setChecked(true);
    m_chbxs[3]->setChecked(true);

    qApp->processEvents();
    for (int i = 0; i < count; ++i) {
        mi::man->getMeasuredValue(value, mi::man->address(), CalibVoltage);
        measure[Ch1] += value.ch1;
        measure[Ch2] += value.ch2;
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

    gradCoeff.adcCh1.scale = 0.05f * (reference / measure[Ch1]);
    gradCoeff.adcCh2.scale = 0.05f * (reference / measure[Ch2]);
    gradCoeff.adcCh1.offset = 0.0f;
    gradCoeff.adcCh2.offset = 0.0f;

    qDebug() << gradCoeff.adcCh1.scale;
    qDebug() << gradCoeff.adcCh2.scale;

    //if ((0.04f < gradCoeff.adcCh1.scale) && (gradCoeff.adcCh1.scale < 0.06f) && (0.04f < gradCoeff.adcCh2.scale) && (gradCoeff.adcCh2.scale < 0.06f))
    mi::man->setCalibrationCoefficients(gradCoeff, mi::man->address());
    //else
    //    QMessageBox::critical(this, "", "Коэффициенты выходят за пределы!");

    setGradDsbxs();

    m_chbxs[5]->setChecked(true);
}

void Graduation::gradUAdc()
{

    enum {
        Meas1,
        Meas2
    };

    double measure[2] = { 0.0, 0.0 };
    double reference[2] = { 0.0, 0.0 };
    const int count2 = 50;

    mi::man->setDefaultCalibrationCoefficients(mi::man->address());
    mi::man->getCalibrationCoefficients(gradCoeff, mi::man->address());

    m_chbxs[0]->setChecked(true);
    m_chbxs[1]->setChecked(true);

    QMessageBox::information(this, "", "Подайте на вход ~50В");
    qApp->processEvents();

    for (int i = 0, counter = 1; i < count; ++i, ++counter) {
        for (int j = 0; j < count2; ++j, ++counter) {
            mi::man->getMeasuredValue(value, mi::man->address(), CurrentMeasuredValue);
            measure[Meas1] += value.ch1;
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
            mi::man->getMeasuredValue(value, mi::man->address(), CurrentMeasuredValue);
            measure[Meas2] += value.ch1;
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

    //    gradCoeff.adcCh1.scale = ((reference[Meas2] - reference[Meas1]) / (measure[Meas2] - measure[Meas1]));
    //    gradCoeff.adcCh1.offset = reference[Meas1] - (measure[Meas1] * gradCoeff.adcCh1.scale);
    gradCoeff.adcCh1.scale = ((reference[Meas2] - reference[Meas1]) / (measure[Meas2] - measure[Meas1]));
    gradCoeff.dac.scale *= gradCoeff.adcCh1.scale;

    qDebug() << gradCoeff.adcCh1.offset;
    qDebug() << gradCoeff.adcCh1.scale;

    //            if ((1.5f < GradCoeff.adcCh1.scale) && (GradCoeff.adcCh1.scale < 2.5f))
    mi::man->setCalibrationCoefficients(gradCoeff, mi::man->address());
    //mi::man->saveToEepromCalibrationCoefficients(mi::man->address());
    //            else
    //                QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");

    m_chbxs[5]->setChecked(true);
}

void Graduation::setGradDsbxs()
{
    dsbxAdcCh1Offset->setValue(gradCoeff.adcCh1.offset);
    dsbxAdcCh1Scale->setValue(gradCoeff.adcCh1.scale);
    dsbxAdcCh2Offset->setValue(gradCoeff.adcCh2.offset);
    dsbxAdcCh2Scale->setValue(gradCoeff.adcCh2.scale);
    dsbxAdcCh3Offset->setValue(gradCoeff.adcCh3.offset);
    dsbxAdcCh3Scale->setValue(gradCoeff.adcCh3.scale);
    dsbxDacOffset->setValue(gradCoeff.dac.offset);
    dsbxDacScale->setValue(gradCoeff.dac.scale);
}

void Graduation::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    groupBoxGrad->setEnabled(mi::man->isConnected() && mi::scpi->isConnected());
}

void Graduation::on_pbStartGrad_clicked()
{
    for (QCheckBox* chbx : m_chbxs)
        chbx->setChecked(false);

    mi::man->shortCircuitTest(ScOff, mi::man->address());
    mi::man->switchCurrent(false, mi::man->address());
    mi::man->oscilloscope(0);

    if (rbVoltage->isChecked()) {
        gradU();
    } else if (rbVoltageAc->isChecked()) {
        gradUAdc();
    } else if (rbCurrent->isChecked()) {
        gradI();
    }
}

void Graduation::on_pbGet_clicked()
{
    mi::man->getCalibrationCoefficients(gradCoeff, mi::man->address());
    setGradDsbxs();
}

void Graduation::on_pbSet_clicked()
{
    mi::man->getCalibrationCoefficients(gradCoeff, mi::man->address());
    gradCoeff.adcCh1.offset = dsbxAdcCh1Offset->value();
    gradCoeff.adcCh1.scale = dsbxAdcCh1Scale->value();
    gradCoeff.adcCh2.offset = dsbxAdcCh2Offset->value();
    gradCoeff.adcCh2.scale = dsbxAdcCh2Scale->value();
    gradCoeff.adcCh3.offset = dsbxAdcCh3Offset->value();
    gradCoeff.adcCh3.scale = dsbxAdcCh3Scale->value();
    gradCoeff.dac.offset = dsbxDacOffset->value();
    gradCoeff.dac.scale = dsbxDacScale->value();
    mi::man->setCalibrationCoefficients(gradCoeff, mi::man->address());
}

void Graduation::on_pbSetDefault_clicked()
{
    mi::man->setDefaultCalibrationCoefficients(mi::man->address());
    mi::man->getCalibrationCoefficients(gradCoeff, mi::man->address());
    setGradDsbxs();
}
