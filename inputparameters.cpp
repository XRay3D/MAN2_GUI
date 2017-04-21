#include "inputparameters.h"
#include <QDebug>
#include <QMessageBox>

int id = qRegisterMetaType<ScanSettings>("ScanSettings_t");
#define SignalMapperInt static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped)

InputParameters::InputParameters(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    m_listLeSerNumDevice << leSerialNumberDevice_1;
    m_listLeSerNumDevice << leSerialNumberDevice_2;
    m_listLeSerNumDevice << leSerialNumberDevice_3;
    m_listLeSerNumDevice << leSerialNumberDevice_4;
    m_listLeSerNumDevice << leSerialNumberDevice_5;
    m_listLeSerNumDevice << leSerialNumberDevice_6;
    m_listLeSerNumDevice << leSerialNumberDevice_7;
    m_listLeSerNumDevice << leSerialNumberDevice_8;

    smSerialNumberDevice = new QSignalMapper(this);
    for (int i = 0; i < m_listLeSerNumDevice.size(); ++i) {
        QLineEdit* lineEdit = m_listLeSerNumDevice[i];
        connect(lineEdit, &QLineEdit::editingFinished, smSerialNumberDevice, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        smSerialNumberDevice->setMapping(lineEdit, i);
    }
    connect(smSerialNumberDevice, SignalMapperInt, this, &InputParameters::SerialNumberDeviceEditingFinished, Qt::DirectConnection);
}

InputParameters::~InputParameters()
{
    qDebug() << "~InputParameters()";
}

void InputParameters::on_pbClearSerialNumber_clicked()
{
    for (int i = 0; i < m_listLeSerNumDevice.size(); ++i) {
        m_listLeSerNumDevice[i]->clear();
        SerialNumberDeviceEditingFinished(i);
    }
}

void InputParameters::on_pbStatrtMeasure_clicked()
{
    bool begin = false;
    for (int i = 0; i < m_listLeSerNumDevice.size(); ++i) {
        if (!m_listLeSerNumDevice[i]->text().isEmpty()) {
            begin = true;
        }
    }
    if (!begin) {
        QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
        return;
    }
    emit CurrentTabIndex(1);
}

void InputParameters::SerialNumberDeviceEditingFinished(int num)
{
    emit SerialNumberChanged(m_listLeSerNumDevice[num]->text(), num);
}

void InputParameters::on_cbDevice_currentIndexChanged(int index)
{
    setFocusProxy(cbDevice);
    if (m_listScanSettings.size() > index && index > -1) {
        leType->setText(m_listScanSettings[index]->Type);
        leCipher->setText(m_listScanSettings[index]->Cipher);
        dsbRatedVoltage->setValue(m_listScanSettings[index]->RatedVoltage);
        sbNumberOfChannels->setValue(m_listScanSettings[index]->NumberOfChannels);
        dsbRatedCurrent->setValue(m_listScanSettings[index]->RatedCurrent);
        dsbRestrictionTest2->setValue(m_listScanSettings[index]->RestrictionTest2);
        dsbVisualControl->setValue(m_listScanSettings[index]->VisualControl);
        dsbLimitationsTest4_5->setValue(m_listScanSettings[index]->LimitationsTest4_5);
        dsbLimitTest6->setValue(m_listScanSettings[index]->LimitTest6);
        dsbRestrictionsTest7Min->setValue(m_listScanSettings[index]->RestrictionsTest7Min);
        dsbRestrictionsTest7Max->setValue(m_listScanSettings[index]->RestrictionsTest7Max);
        dsbVoltageErrorTest3_4_U1->setValue(m_listScanSettings[index]->Voltageerrortest3_4U1);
        dsbVoltageErrorTest3_4_U2->setValue(m_listScanSettings[index]->Voltageerrortest3_4U2);
        dsbVoltageErrorTest5_U1->setValue(m_listScanSettings[index]->Voltageerrortest5U1);
        dsbVoltageErrorTest5_U2->setValue(m_listScanSettings[index]->Voltageerrortest5U2);
        dsbVoltageErrorTest7->setValue(m_listScanSettings[index]->VoltageErrorTest7);
        dsbParameterDLTest7->setValue(m_listScanSettings[index]->ParameterDLTest7);

        for (int i = 0, NumberOfChannels = 8 / m_listScanSettings[index]->NumberOfChannels; i < 8; ++i) {
            if (i < NumberOfChannels) {
                m_listLeSerNumDevice[i]->setEnabled(true);
            }
            else {
                m_listLeSerNumDevice[i]->setEnabled(false);
            }
        }
        m_listScanSettings[index]->Fio = leFioOtk->text();
        emit ScanSettingsSignal(m_listScanSettings[index]);
        on_pbClearSerialNumber_clicked();
    }
}
