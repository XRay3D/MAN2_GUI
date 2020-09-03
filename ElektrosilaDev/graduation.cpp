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

    connect(pbSave, &QPushButton::clicked, [&]() { mi::man->saveToEepromCalibrationCoefficients(m_channel); });

    connect(dsbxAdcCh1Offset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.AdcCh1Offset = val; });
    connect(dsbxAdcCh1Scale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.AdcCh1Scale = val; });
    connect(dsbxAdcCh2Offset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.AdcCh2Offset = val; });
    connect(dsbxAdcCh2Scale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.AdcCh2Scale = val; });
    connect(dsbxAdcCh3Offset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.AdcCh3Offset = val; });
    connect(dsbxAdcCh3Scale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.AdcCh3Scale = val; });
    connect(dsbxDacOffset, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.DacOffset = val; });
    connect(dsbxDacScale, qOverload<double>(&QDoubleSpinBox::valueChanged),
        [this](double val) { gradCoeff.DacScale = val; });

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

void Graduation::gradI()
{
    enum {
        Meas1,
        Meas2
    };

    double measured[2] = { 0.0, 0.0 };
    double reference[2] = { 0.0, 0.0 };

    QMessageBox::information(this, "", "Подключите источник тока 3 ампера.");

    mi::man->getCalibrationCoefficients(gradCoeff, m_channel);
    setGradDsbxs();
    mi::man->setDefaultCalibrationCoefficients(m_channel);

    m_chbxs[0]->setChecked(true);
    m_chbxs[1]->setChecked(true);

    constexpr double i1 = 0.01; // A
    constexpr double i2 = 3.0; // A

    GradCoeff GradCoeff2;
    mi::man->getCalibrationCoefficients(GradCoeff2, m_channel);
    const double defDacScale = GradCoeff2.DacScale; // A

    { //0.1 A
        mi::man->setCurrent(i1 * 1000, m_channel);
        mi::man->switchCurrent(true, m_channel);
        thread()->sleep(10);
        qApp->processEvents();
        mi::man->getMeasuredValue(value, m_channel, CalibCurrent);
        for (int i = 0; i < count;) {
            mi::man->getMeasuredValue(value, m_channel, CalibCurrent);
            measured[Meas1] += static_cast<double>(value.value1);
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
        mi::man->setCurrent(i2 * 1000, m_channel);
        thread()->sleep(10);
        qApp->processEvents();
        mi::man->getMeasuredValue(value, m_channel, CalibCurrent);
        for (int i = 0; i < count;) {
            mi::man->getMeasuredValue(value, m_channel, CalibCurrent);
            measured[Meas2] += static_cast<double>(value.value1);
            reference[Meas2] += mi::scpi->GetDcCurrent();
            dsbxMeasure_1->setValue(measured[Meas2] / ++i);
            dsbxSet_1->setValue(reference[Meas2] / i);
            qApp->processEvents();
        }
        measured[Meas2] /= count;
        reference[Meas2] /= count;
    }

    mi::man->setCurrent(0, m_channel);
    mi::man->switchCurrent(false, m_channel);

    const double scale = (i2 - i1) / (reference[Meas2] - reference[Meas1]);
    gradCoeff.AdcCh3Scale = static_cast<float>(((reference[Meas2] - reference[Meas1]) / (measured[Meas2] - measured[Meas1])) * 0.01);
    gradCoeff.DacOffset = static_cast<float>((i1 / scale) - reference[Meas1]);
    gradCoeff.DacScale = static_cast<float>(scale * defDacScale);
    m_chbxs[4]->setChecked(true);

    //    qDebug() << "AdcCh3Scale" << gradCoeff.AdcCh3Scale;
    //    qDebug() << "DacOffset" << gradCoeff.DacOffset;
    //    qDebug() << "DacScale" << gradCoeff.DacScale;

    //    if ((0.009f < gradCoeff.AdcCh3Scale) && (gradCoeff.AdcCh3Scale < 0.011f)
    //        /*&& (0.0f < gradCoeff.DacOffset) && (gradCoeff.DacOffset < 0.3f)*/
    //        /*&& (defDacScale * 0.9 < gradCoeff.DacScale) && (gradCoeff.DacScale < defDacScale * 1.1)*/)
    mi::man->setCalibrationCoefficients(gradCoeff, m_channel);
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

    mi::man->getCalibrationCoefficients(gradCoeff, m_channel);
    setGradDsbxs();

    m_chbxs[0]->setChecked(true);
    m_chbxs[1]->setChecked(true);
    m_chbxs[2]->setChecked(true);
    m_chbxs[3]->setChecked(true);

    qApp->processEvents();
    for (int i = 0; i < count; ++i) {
        mi::man->getMeasuredValue(value, m_channel, CalibVoltage);
        measure[Ch1] += value.value1;
        measure[Ch2] += value.value2;
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

    gradCoeff.AdcCh1Scale = 0.05f * (reference / measure[Ch1]);
    gradCoeff.AdcCh2Scale = 0.05f * (reference / measure[Ch2]);
    gradCoeff.AdcCh1Offset = 0.0f;
    gradCoeff.AdcCh2Offset = 0.0f;

    qDebug() << gradCoeff.AdcCh1Scale;
    qDebug() << gradCoeff.AdcCh2Scale;

    if ((0.04f < gradCoeff.AdcCh1Scale) && (gradCoeff.AdcCh1Scale < 0.06f) && (0.04f < gradCoeff.AdcCh2Scale) && (gradCoeff.AdcCh2Scale < 0.06f))
        mi::man->setCalibrationCoefficients(gradCoeff, m_channel);
    else
        QMessageBox::critical(this, "", "Коэффициенты выходят за пределы!");

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

    mi::man->setDefaultCalibrationCoefficients(m_channel);

    m_chbxs[0]->setChecked(true);
    m_chbxs[1]->setChecked(true);

    QMessageBox::information(this, "", "Подайте на вход ~50В");
    qApp->processEvents();

    for (int i = 0, counter = 1; i < count; ++i, ++counter) {
        for (int j = 0; j < count2; ++j, ++counter) {
            measure[Meas1] += mi::man->getRmsValue();
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
            measure[Meas2] += mi::man->getRmsValue();
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

    gradCoeff.AdcCh1Scale = ((reference[Meas2] - reference[Meas1]) / (measure[Meas2] - measure[Meas1]));
    gradCoeff.AdcCh1Offset = reference[Meas1] - (measure[Meas1] * gradCoeff.AdcCh1Scale);

    qDebug() << gradCoeff.AdcCh1Offset;
    qDebug() << gradCoeff.AdcCh1Scale;

    //            if ((1.5f < GradCoeff.AdcCh1Scale) && (GradCoeff.AdcCh1Scale < 2.5f))
    mi::man->setCalibrationCoefficients(gradCoeff, m_channel);
    mi::man->saveToEepromCalibrationCoefficients(m_channel);
    //            else
    //                QMessageBox::critical(this, "", "Что-то пошло не так, коэффициенты выходят за пределы!");

    m_chbxs[5]->setChecked(true);
}

void Graduation::setGradDsbxs()
{
    dsbxAdcCh1Offset->setValue(gradCoeff.AdcCh1Offset);
    dsbxAdcCh1Scale->setValue(gradCoeff.AdcCh1Scale);
    dsbxAdcCh2Offset->setValue(gradCoeff.AdcCh2Offset);
    dsbxAdcCh2Scale->setValue(gradCoeff.AdcCh2Scale);
    dsbxAdcCh3Offset->setValue(gradCoeff.AdcCh3Offset);
    dsbxAdcCh3Scale->setValue(gradCoeff.AdcCh3Scale);
    dsbxDacOffset->setValue(gradCoeff.DacOffset);
    dsbxDacScale->setValue(gradCoeff.DacScale);
}

void Graduation::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    groupBoxGrad->setEnabled(mi::man->IsConnected() && mi::scpi->IsConnected());
    m_channel = mi::man->address(); // cbManCh->currentIndex() + 1;
}

void Graduation::on_pbStartGrad_clicked()
{
    for (QCheckBox* chbx : m_chbxs)
        chbx->setChecked(false);

    mi::man->shortCircuitTest(false, m_channel);
    mi::man->switchCurrent(false, m_channel);
    mi::man->oscilloscope(0);

    if (rbVoltage->isChecked()) {
        if (m_channel == 9) {
            gradUAdc();
        } else {
            gradU();
        }
    } else if (rbCurrent->isChecked()) {
        gradI();
    }
}

void Graduation::on_pbGet_clicked()
{
    mi::man->getCalibrationCoefficients(gradCoeff, m_channel);
    setGradDsbxs();
}

void Graduation::on_pbSet_clicked()
{
    mi::man->getCalibrationCoefficients(gradCoeff, m_channel);
    gradCoeff.AdcCh1Offset = dsbxAdcCh1Offset->value();
    gradCoeff.AdcCh1Scale = dsbxAdcCh1Scale->value();
    gradCoeff.AdcCh2Offset = dsbxAdcCh2Offset->value();
    gradCoeff.AdcCh2Scale = dsbxAdcCh2Scale->value();
    gradCoeff.AdcCh3Offset = dsbxAdcCh3Offset->value();
    gradCoeff.AdcCh3Scale = dsbxAdcCh3Scale->value();
    gradCoeff.DacOffset = dsbxDacOffset->value();
    gradCoeff.DacScale = dsbxDacScale->value();
    mi::man->setCalibrationCoefficients(gradCoeff, m_channel);
}

void Graduation::on_pbSetDefault_clicked()
{
    mi::man->setDefaultCalibrationCoefficients(m_channel);
    mi::man->getCalibrationCoefficients(gradCoeff, m_channel);
    setGradDsbxs();
}
