#include "autoform.h"

#include "devicemodel.h"
#include "dialogeditbp.h"
#include "manmodel.h"
#include "sernummodel.h"
#include "tester.h"
#include "testmodel.h"
#include <QAxWidget>
#include <QMessageBox>

int id = qRegisterMetaType<ScanSettings>("ScanSettings_t");

PrepareForm::PrepareForm(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    cbxDevice->addItems(DeviceModel::cbxData());

    connect(pbClearSerNum, &QPushButton::clicked, SerNumModel::instance(), &SerNumModel::clear);
    connect(cbxDevice, qOverload<int>(&QComboBox::currentIndexChanged), [](int index) {
        DeviceModel::setIndex(index);
        if (DeviceModel::scanSettings().NumberOfChannels > 0)
            SerNumModel::instance()->setCount(8 / DeviceModel::scanSettings().NumberOfChannels);
    });
    connect(pbStartStop, &QPushButton::clicked, [this](bool checked) {
        if (checked && SerNumModel::instance()->isEmpty()) {
            QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
            pbStartStop->setChecked(false); //pbStartStopClicked(false);
        } else
            pbStartStopClicked(checked);
    });
    connect(pushButton, &QPushButton::clicked, [this] { DialogEditBp(this).exec(); });
    connect(
        pbClear, &QPushButton::clicked, [this] {
            if (QMessageBox::question(this, "", "Отчистить?") == QMessageBox::Yes)
                TestModel::instance()->reset();
        });
}

PrepareForm::~PrepareForm()
{
    if (measureTimerId) {
        killTimer(measureTimerId);
        measureTimerId = 0;
    }
}

void PrepareForm::showMessage(int num)
{
    static int stage = 0;
    QString messageText;
    switch (num) {
    case SetInputVoltageUpper:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В")
                          .arg(DeviceModel::scanSettings().Voltageerrortest5U1)
                          .arg(DeviceModel::scanSettings().Voltageerrortest5U2)
                          .replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            pbStartStopClicked(false);
        else
            m_worker->continueTests();
        return;
    case SetInputVoltageLower:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В")
                          .arg(DeviceModel::scanSettings().Voltageerrortest3_4U1)
                          .arg(DeviceModel::scanSettings().Voltageerrortest3_4U2)
                          .replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            pbStartStopClicked(false);
        else
            m_worker->continueTests();
        return;
    case SetInputVoltageNormal:
        stage = num;
        messageText = "Установите входное напряжение 220±4,4В";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            pbStartStopClicked(false);

        else
            m_worker->continueTests();
        return;
    case NoConnectionWithMan:
        messageText = "Нет связи с МАНом 2!";
        if (QMessageBox::critical(this, "", messageText, "Повторить", "Остановить измерения"))
            pbStartStopClicked(false);
        else
            m_worker->continueTests();
        return;
    case RestoreTheOperationOfChannels:
        messageText = "Восстановите работу каналов блока питания";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            pbStartStopClicked(false);
        else
            m_worker->continueTests();
        return;
    case PulsationsOnTheChannel_1:
    case PulsationsOnTheChannel_2:
    case PulsationsOnTheChannel_3:
    case PulsationsOnTheChannel_4:
    case PulsationsOnTheChannel_5:
    case PulsationsOnTheChannel_6:
    case PulsationsOnTheChannel_7:
    case PulsationsOnTheChannel_8:
        messageText = QString("Пульсации в канале %2 соответствуют норме <%1мВ ?")
                          .arg(DeviceModel::scanSettings().VisualControl)
                          .arg(num - (PulsationsOnTheChannel_1 - 1))
                          .replace('.', ',');
        switch (QMessageBox::question(this, "", messageText, "Да", "Нет", "Остановить измерения")) {
        case 0:
            TestModel::instance()->setTest2(num - PulsationsOnTheChannel_1, true);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = 1.0;
            m_worker->continueTests();
            return;
        case 1:
            TestModel::instance()->setTest2(num - PulsationsOnTheChannel_1, false);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = -1.0;
            m_worker->continueTests();
            return;
        case 2:
            pbStartStopClicked(false);
            return;
        }
        return;
    case CheckInputVoltage:
        switch (stage) {
        case SetInputVoltageUpper:
            messageText = QString("Установите входное напряжение %1±%2В")
                              .arg(DeviceModel::scanSettings().Voltageerrortest5U1)
                              .arg(DeviceModel::scanSettings().Voltageerrortest5U2)
                              .replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                pbStartStopClicked(false);
            else
                m_worker->continueTests();
            return;
        case SetInputVoltageLower:
            messageText = QString("Установите входное напряжение %1±%2В")
                              .arg(DeviceModel::scanSettings().Voltageerrortest3_4U1)
                              .arg(DeviceModel::scanSettings().Voltageerrortest3_4U2)
                              .replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                pbStartStopClicked(false);
            else
                m_worker->continueTests();
            return;
        case SetInputVoltageNormal:
            messageText = "Установите входное напряжение 220±4,4В";
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                pbStartStopClicked(false);
            else
                m_worker->continueTests();
            return;
        }
    }
    return;
}

void PrepareForm::updateProgresBar()
{
    if (progressBar->value() == progressBar->maximum())
        progressBar->setValue(0);
    progressBar->setValue(progressBar->value() + 1);
}

void PrepareForm::endSlot()
{
    pbStartStopClicked(false);
    for (int i = 0; i < SerNumModel::instance()->serNumCount(); ++i) {
        TestModel::instance()->saveProtokol(SerNumModel::instance()->serNum(i), i);
        TestModel::instance()->showProtocol(i);
    }
    QMessageBox::information(nullptr, "", "Проверка закончена.");
}

void PrepareForm::pbStartStopClicked(bool checked)
{
    pbStartStop->setChecked(checked);
    tvTest->setCheckable(!checked);

    if (checked) {
        pbStartStop->setText("Остановить измерения");

        if (measureTimerId) {
            killTimer(measureTimerId);
            measureTimerId = 0;
        }
        if (SerNumModel::instance()->isEmpty()) {
            QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
            pbStartStop->setChecked(false);
            return;
        }

        int i = SerNumModel::instance()->serNumCount() * DeviceModel::scanSettings().NumberOfChannels;
        for (bool& val : m_doNotSkip)
            val = i-- > 0;

        m_worker = new Tester(m_doNotSkip);
        connect(m_worker, &QThread::finished, this, &PrepareForm::endSlot);
        connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_worker, &Tester::showMessage, this, &PrepareForm::showMessage);
        connect(m_worker, &Tester::updateProgresBar, this, &PrepareForm::updateProgresBar);

        progressBar->setValue(0);
        progressBar->setMaximum(7 + SerNumModel::instance()->serNumCount() * DeviceModel::scanSettings().NumberOfChannels * 2);

        m_worker->start();
    } else {
        pbStartStop->setText("Начать измерения");

        if (m_worker)
            m_worker->finishTests();

        progressBar->setValue(0);

        if (measureTimerId == 0)
            measureTimerId = startTimer(500);
    }
}

void PrepareForm::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    connect(mi::man, &MAN2::measuresCompleted, static_cast<ManModel*>(tvMeasure->model()), &ManModel::setMeasuredValues);
    connect(this, &PrepareForm::startMeasure, mi::man, &MAN2::startMeasure);
    if (mi::man->isConnected()) {
        measureTimerId = startTimer(500);
        setEnabled(true);
    } else {
        setEnabled(false);
        emit currentTabIndex(2);
    }
}

void PrepareForm::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    disconnect(mi::man, &MAN2::measuresCompleted, static_cast<ManModel*>(tvMeasure->model()), &ManModel::setMeasuredValues);
    disconnect(this, &PrepareForm::startMeasure, mi::man, &MAN2::startMeasure);
    if (measureTimerId) {
        killTimer(measureTimerId);
        measureTimerId = 0;
    }
}

void PrepareForm::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == measureTimerId) {
        emit startMeasure();
    }
}

void PrepareForm::on_pbProto_clicked()
{
    QDialog* mainWidget = new QDialog;
    QAxWidget* excel = new QAxWidget(mainWidget);
    QHBoxLayout* hbox = new QHBoxLayout(mainWidget);
    hbox->addWidget(excel);
    mainWidget->setLayout(hbox);
    excel->setProperty("Visible", true);
    mainWidget->show();
    excel->setControl("D:/PRO/QT/MAN2/protocol/protocol.xlsx");
    mainWidget->exec();
    connect(mainWidget, &QObject::destroyed, [excel] { excel->dynamicCall("Quit()"); });
    mainWidget->deleteLater();
    //    for (int i = 0; i < SerNumModel::serNumCount(); ++i) {
    //        TestModel::instance()->saveProtokol(SerNumModel::serNum(i), i);
    //        TestModel::instance()->showProtocol(i);
    //    }
}
