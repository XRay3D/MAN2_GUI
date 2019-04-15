#include "automaticmeasurements.h"
#include "hw/interface.h"
#include "mandatamodel.h"
#include "mesuremodel.h"
#include "mydialog.h"
#include "preparation/preparation.h"
#include "preparation/sernummodel.h"
#include "worker.h"
#include <QDate>
#include <QDir>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QSettings>
#include <preparation/devicemodel.h>

AutomaticMeasurements::AutomaticMeasurements(QWidget* parent)
    : QWidget(parent)
    , m_model(new ManDataModel(this))
{
    setupUi(this);

    tvMeasure->setModel(m_model);
    tvMeasure->setSpan(2, 0, 1, 8);

    tvMeasure->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvMeasure->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tvSerNum->setModel(SerNumModel ::self);
    tvSerNum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvSerNum->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tvSerNum->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(tvSerNum, &QTableView::doubleClicked, [](const QModelIndex& index) {
        if (index.row() < SerNumModel::self->serNumCount())
            MesureModel::self->showProtocol(index.row());
    });

    tableView->setModel(new MesureModel);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(&m_timerRms, &QTimer::timeout, [&]() { m_model->setRms(mi::man->GetRmsValue()); });
}

AutomaticMeasurements::~AutomaticMeasurements()
{
}

void AutomaticMeasurements::showMessage(int num)
{
    static int stage = 0;
    QString messageText;
    switch (num) {
    case SET_INPUT_VOLTAGE_0:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В")
                          .arg(DeviceModel::self->scanSettings().Voltageerrortest5U1)
                          .arg(DeviceModel::self->scanSettings().Voltageerrortest5U2)
                          .replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case SET_INPUT_VOLTAGE_1:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В")
                          .arg(DeviceModel::self->scanSettings().Voltageerrortest3_4U1)
                          .arg(DeviceModel::self->scanSettings().Voltageerrortest3_4U2)
                          .replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case SET_INPUT_VOLTAGE_2:
        stage = num;
        messageText = "Установите входное напряжение 220±4,4В";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);

        else
            m_worker->Continue();
        return;
    case NO_CONNECTION_WITH_MAN:
        messageText = "Нет связи с МАНом 2!";
        if (QMessageBox::critical(this, "", messageText, "Повторить", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case RESTORE_THE_OPERATION_OF_CHANNELS:
        messageText = "Восстановите работу каналов блока питания";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            on_pbStartStop_clicked(false);
        else
            m_worker->Continue();
        return;
    case PULSATIONS_ON_THE_CHANNEL_1:
    case PULSATIONS_ON_THE_CHANNEL_2:
    case PULSATIONS_ON_THE_CHANNEL_3:
    case PULSATIONS_ON_THE_CHANNEL_4:
    case PULSATIONS_ON_THE_CHANNEL_5:
    case PULSATIONS_ON_THE_CHANNEL_6:
    case PULSATIONS_ON_THE_CHANNEL_7:
    case PULSATIONS_ON_THE_CHANNEL_8:
        messageText = QString("Пульсации в канале %2 соответствуют норме <%1мВ ?")
                          .arg(DeviceModel::self->scanSettings().VisualControl)
                          .arg(num - (PULSATIONS_ON_THE_CHANNEL_1 - 1))
                          .replace('.', ',');
        switch (QMessageBox::question(this, "", messageText, "Да", "Нет", "Остановить измерения")) {
        case 0:
            MesureModel::self->setTest2(num - PULSATIONS_ON_THE_CHANNEL_1, true);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = 1.0;
            m_worker->Continue();
            return;
        case 1:
            MesureModel::self->setTest2(num - PULSATIONS_ON_THE_CHANNEL_1, false);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = -1.0;
            m_worker->Continue();
            return;
        case 2:
            on_pbStartStop_clicked(false);
            return;
        }
        return;
    case TEST_1:
    case TEST_2:
    case TEST_3:
    case TEST_4:
    case TEST_5:
    case TEST_6:
    case TEST_7:
        //m_listCheckBox[num - TEST_1]->setChecked(true);
        return;
    case CHECK_INPUT_VOLTAGE:
        switch (stage) {
        case 0:
            messageText = QString("Установите входное напряжение %1±%2В")
                              .arg(DeviceModel::self->scanSettings().Voltageerrortest5U1)
                              .arg(DeviceModel::self->scanSettings().Voltageerrortest5U2)
                              .replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                on_pbStartStop_clicked(false);
            else
                m_worker->Continue();
            return;
        case 1:
            messageText = QString("Установите входное напряжение %1±%2В")
                              .arg(DeviceModel::self->scanSettings().Voltageerrortest3_4U1)
                              .arg(DeviceModel::self->scanSettings().Voltageerrortest3_4U2)
                              .replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                on_pbStartStop_clicked(false);
            else
                m_worker->Continue();
            return;
        case 2:
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

void AutomaticMeasurements::updateProgresBar()
{
    if (progressBar->value() == progressBar->maximum())
        progressBar->setValue(0);
    progressBar->setValue(progressBar->value() + 1);
}

void AutomaticMeasurements::endSlot()
{
    on_pbStartStop_clicked(false);
    QMessageBox::information(this, "", "Проверка закончена.");
    for (int i = 0; i < SerNumModel::self->serNumCount(); ++i) {
        MesureModel::self->saveProtokol(SerNumModel::self->serNum(i), i);
        MesureModel::self->showProtocol(i);
    }
}

void AutomaticMeasurements::on_pbStartStop_clicked(bool checked)
{
    pbStartStop->setChecked(checked);
    if (checked) {
        if (SerNumModel::self->isEmpty()) {
            QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
            pbStartStop->setChecked(false);
            return;
        }

        int i = SerNumModel::self->serNumCount() * DeviceModel::self->scanSettings().NumberOfChannels;
        for (bool& val : m_doNotSkip)
            val = i-- > 0;

        MesureModel::self->reset();
        pbStartStop->setText("Остановить измерения");
        m_worker = new Worker(m_doNotSkip);
        connect(m_worker, &QThread::finished, this, &AutomaticMeasurements::endSlot);
        connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_worker, &Worker::showMessage, this, &AutomaticMeasurements::showMessage);
        connect(m_worker, &Worker::updateProgresBar, this, &AutomaticMeasurements::updateProgresBar);
        progressBar->setValue(0);
        progressBar->setMaximum(7 + SerNumModel::self->serNumCount() * DeviceModel::self->scanSettings().NumberOfChannels * 22);
        m_worker->start();
    } else {
        pbStartStop->setText("Начать измерения");
        m_worker->FinishMeasurements();
        for (int i = 0; i < SerNumModel::self->serNumCount(); ++i) {
            MesureModel::self->saveProtokol(SerNumModel::self->serNum(i), i);
            MesureModel::self->showProtocol(i);
        }
    }
}

void AutomaticMeasurements::showEvent(QShowEvent* /*event*/)
{
    if (mi::man->IsConnected()) {
        connect(mi::man, &MAN2::GetMeasuredValueSignal, m_model, &ManDataModel::setMeasuredValueSignal);
        m_timerRms.start(100);
        return;
    }
    setEnabled(false);
}

void AutomaticMeasurements::hideEvent(QHideEvent* /*event*/)
{
    disconnect(mi::man, &MAN2::GetMeasuredValueSignal, m_model, &ManDataModel::setMeasuredValueSignal);
    m_timerRms.stop();
}
