#include "automeasure.h"
#include "hw/interface.h"
#include "manmodel.h"
#include "mesuremodel.h"
#include "mydialog.h"
#include "preparation/devicemodel.h"
#include "preparation/scansettings.h"
#include "preparation/sernummodel.h"
#include "worker.h"
#include <QMessageBox>

AutoMeasure::AutoMeasure(QWidget* parent)
    : QWidget(parent)
    , m_model(new ManModel(this))
{
    setupUi(this);

    tvMeasure->setModel(m_model);
    tvMeasure->setSpan(2, 0, 1, 8);

    tvMeasure->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvMeasure->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tvMeasure->verticalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    tvSerNum->setModel(SerNumModel::self);
    tvSerNum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvSerNum->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tvSerNum->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(tvSerNum, &QTableView::doubleClicked, [](const QModelIndex& index) {
        if (index.row() < SerNumModel::self->serNumCount())
            MesureModel::showProtocol(index.row());
    });

    tableView->setModel(new MesureModel);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(&m_timerRms, &QTimer::timeout, [&]() { m_model->setRms(mi::man->getRmsValue()); });
}

AutoMeasure::~AutoMeasure()
{
}

void AutoMeasure::showMessage(int num)
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
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case SetInputVoltageLower:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В")
                          .arg(DeviceModel::scanSettings().Voltageerrortest3_4U1)
                          .arg(DeviceModel::scanSettings().Voltageerrortest3_4U2)
                          .replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case SetInputVoltageNormal:
        stage = num;
        messageText = "Установите входное напряжение 220±4,4В";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);

        else
            m_worker->Continue();
        return;
    case NoConnectionWithMan:
        messageText = "Нет связи с МАНом 2!";
        if (QMessageBox::critical(this, "", messageText, "Повторить", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case RestoreTheOperationOfChannels:
        messageText = "Восстановите работу каналов блока питания";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
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
            MesureModel::setTest2(num - PulsationsOnTheChannel_1, true);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = 1.0;
            m_worker->Continue();
            return;
        case 1:
            MesureModel::setTest2(num - PulsationsOnTheChannel_1, false);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = -1.0;
            m_worker->Continue();
            return;
        case 2:
            on_pbStartStop_clicked(false);
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
                on_pbStartStop_clicked(false);
            else
                m_worker->Continue();
            return;
        case SetInputVoltageLower:
            messageText = QString("Установите входное напряжение %1±%2В")
                              .arg(DeviceModel::scanSettings().Voltageerrortest3_4U1)
                              .arg(DeviceModel::scanSettings().Voltageerrortest3_4U2)
                              .replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                on_pbStartStop_clicked(false);
            else
                m_worker->Continue();
            return;
        case SetInputVoltageNormal:
            messageText = "Установите входное напряжение 220±4,4В";
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                on_pbStartStop_clicked(false);
            else
                m_worker->Continue();
            return;
        }
    }
    return;
}

void AutoMeasure::updateProgresBar()
{
    if (progressBar->value() == progressBar->maximum())
        progressBar->setValue(0);
    progressBar->setValue(progressBar->value() + 1);
}

void AutoMeasure::endSlot() { on_pbStartStop_clicked(false); }

void AutoMeasure::on_pbStartStop_clicked(bool checked)
{
    pbStartStop->setChecked(checked);
    if (checked) {
        if (SerNumModel::self->isEmpty()) {
            QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
            pbStartStop->setChecked(false);
            return;
        }

        int i = SerNumModel::self->serNumCount() * DeviceModel::scanSettings().NumberOfChannels;
        for (bool& val : m_doNotSkip)
            val = i-- > 0;

        MesureModel::reset();
        pbStartStop->setText("Остановить измерения");
        m_worker = new Worker(m_doNotSkip);
        connect(m_worker, &QThread::finished, this, &AutoMeasure::endSlot);
        connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_worker, &Worker::showMessage, this, &AutoMeasure::showMessage);
        connect(m_worker, &Worker::updateProgresBar, this, &AutoMeasure::updateProgresBar);
        progressBar->setValue(0);
        progressBar->setMaximum(7 + SerNumModel::self->serNumCount() * DeviceModel::scanSettings().NumberOfChannels * 2);
        m_worker->start();
    } else {
        pbStartStop->setText("Начать измерения");
        m_worker->FinishMeasurements();
        progressBar->setValue(0);
        for (int i = 0; i < SerNumModel::self->serNumCount(); ++i) {
            MesureModel::saveProtokol(SerNumModel::self->serNum(i), i);
            MesureModel::showProtocol(i);
        }
        QMessageBox::information(nullptr, "", "Проверка закончена.");
    }
}

void AutoMeasure::showEvent(QShowEvent* /*event*/)
{
    if (mi::man->IsConnected()) {
        connect(mi::man, &MAN2::GetMeasuredValueSignal, m_model, &ManModel::setMeasuredValueSignal);
        m_timerRms.start(100);
        setEnabled(true);
        return;
    }
    setEnabled(false);
}

void AutoMeasure::hideEvent(QHideEvent* /*event*/)
{
    disconnect(mi::man, &MAN2::GetMeasuredValueSignal, m_model, &ManModel::setMeasuredValueSignal);
    m_timerRms.stop();
}
