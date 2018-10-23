#include "prepareform.h"
#include "devicemodel.h"
#include "sernummodel.h"

#include <QMessageBox>
int id = qRegisterMetaType<ScanSettings>("ScanSettings_t");

PrepareForm::PrepareForm(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    tvParams->setModel(new DeviceModel(this));
    tvParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tvSerNum->setModel(SerNumModel::self);
    tvSerNum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvSerNum->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(pbClearSerNum, &QPushButton::clicked, this, &PrepareForm::clearSerialNumber_clicked);
    connect(pbStatrtMeasure, &QPushButton::clicked, this, &PrepareForm::statrtMeasure_clicked);
    connect(cbxDevice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PrepareForm::cbxDevice_currentIndexChanged);
}

PrepareForm::~PrepareForm()
{
}

void PrepareForm::clearSerialNumber_clicked()
{
    for (QLineEdit* lineEdit : m_listLeSerNum) {
        lineEdit->clear();
        //DeviceSerNumChange(lineEdit);
        emit SerialNumberChanged("", m_listLeSerNum.indexOf(lineEdit));
    }
}

void PrepareForm::statrtMeasure_clicked()
{
    bool begin = false;
    for (int i = 0; i < m_listLeSerNum.size(); ++i) {
        if (!m_listLeSerNum[i]->text().isEmpty()) {
            begin = true;
        }
    }
    if (!begin) {
        QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
        return;
    }
    emit CurrentTabIndex(1);
}

void PrepareForm::cbxDevice_currentIndexChanged(int index)
{
    //    setFocusProxy(cbDevice);
    //    if (m_listScanSettings.size() > index && index > -1) {

    //        leType->setText(m_listScanSettings[index]->Type);
    //        leCipher->setText(m_listScanSettings[index]->Cipher);
    //        dsbRatedVoltage->setValue(m_listScanSettings[index]->RatedVoltage);
    //        sbNumberOfChannels->setValue(m_listScanSettings[index]->NumberOfChannels);
    //        dsbRatedCurrent->setValue(m_listScanSettings[index]->RatedCurrent);
    //        dsbRestrictionTest2->setValue(m_listScanSettings[index]->RestrictionTest2);
    //        dsbVisualControl->setValue(m_listScanSettings[index]->VisualControl);
    //        dsbLimitationsTest4_5->setValue(m_listScanSettings[index]->LimitationsTest4_5);
    //        dsbLimitTest6->setValue(m_listScanSettings[index]->LimitTest6);
    //        dsbRestrictionsTest7Min->setValue(m_listScanSettings[index]->RestrictionsTest7Min);
    //        dsbRestrictionsTest7Max->setValue(m_listScanSettings[index]->RestrictionsTest7Max);
    //        dsbVoltageErrorTest3_4_U1->setValue(m_listScanSettings[index]->Voltageerrortest3_4U1);
    //        dsbVoltageErrorTest3_4_U2->setValue(m_listScanSettings[index]->Voltageerrortest3_4U2);
    //        dsbVoltageErrorTest5_U1->setValue(m_listScanSettings[index]->Voltageerrortest5U1);
    //        dsbVoltageErrorTest5_U2->setValue(m_listScanSettings[index]->Voltageerrortest5U2);
    //        dsbVoltageErrorTest7->setValue(m_listScanSettings[index]->VoltageErrorTest7);
    //        dsbParameterDLTest7->setValue(m_listScanSettings[index]->ParameterDLTest7);

    //        for (int i = 0, NumberOfChannels = 8 / m_listScanSettings[index]->NumberOfChannels; i < 8; ++i) {
    //            if (i < NumberOfChannels) {
    //                m_listLeSerNumDevice[i]->setVisible(true);
    //            } else {
    //                m_listLeSerNumDevice[i]->setVisible(false);
    //            }
    //        }
    //        m_listScanSettings[index]->Fio = leFioOtk->text();
    //        emit ScanSettingsSignal(m_listScanSettings[index]);
    //        on_pbClearSerialNumber_clicked();
    //    }
}
