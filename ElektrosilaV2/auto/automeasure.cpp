#include "automeasure.h"
#include "devicemodel.h"
#include "hw/interface.h"
#include "manmodel.h"
#include "mydialog.h"
#include "scansettings.h"
#include "sernummodel.h"
#include "testmodel.h"
#include "worker.h"
#include <QCheckBox>
#include <QMessageBox>
#include <QSettings>

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

    tvSerNum->setModel(SerNumModel::instance());
    tvSerNum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvSerNum->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tvSerNum->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(tvSerNum, &QTableView::doubleClicked, [](const QModelIndex& index) {
        if (index.row() < SerNumModel::serNumCount())
            TestModel::instance()->instance()->showProtocol(index.row());
    });
    tableView->initCheckBox();
    //    tableView->setModel(new TestModel);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //    QSettings settings;
    //    settings.beginGroup("AutoMeasure");
    //    splitter_2->restoreGeometry(settings.value("Geometry2").toByteArray());
    //    splitter->restoreGeometry(settings.value("Geometry1").toByteArray());

    connect(&m_timerRms, &QTimer::timeout, [&]() { m_model->setRms(mi::man->getRmsValue()); });
}

AutoMeasure::~AutoMeasure()
{
    //    QSettings settings;
    //    settings.beginGroup("AutoMeasure");
    //    settings.setValue("Geometry2", splitter_2->saveGeometry());
    //    settings.setValue("Geometry1", splitter->saveGeometry());
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
            TestModel::instance()->setTest2(num - PulsationsOnTheChannel_1, true);
            //m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = 1.0;
            m_worker->Continue();
            return;
        case 1:
            TestModel::instance()->setTest2(num - PulsationsOnTheChannel_1, false);
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
    tableView->verticalHeader()->setEnabled(!checked);
    tableView->checkBox()->setEnabled(!checked);

    if (checked) {
        if (SerNumModel::isEmpty()) {
            QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
            pbStartStop->setChecked(false);
            return;
        }

        int i = SerNumModel::serNumCount() * DeviceModel::scanSettings().NumberOfChannels;
        for (bool& val : m_doNotSkip)
            val = i-- > 0;

        pbStartStop->setText("Остановить измерения");
        m_worker = new Worker(m_doNotSkip);
        connect(m_worker, &QThread::finished, this, &AutoMeasure::endSlot);
        connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_worker, &Worker::showMessage, this, &AutoMeasure::showMessage);
        connect(m_worker, &Worker::updateProgresBar, this, &AutoMeasure::updateProgresBar);
        progressBar->setValue(0);
        progressBar->setMaximum(7 + SerNumModel::serNumCount() * DeviceModel::scanSettings().NumberOfChannels * 2);
        m_worker->start();
    } else {
        pbStartStop->setText("Начать измерения");
        m_worker->FinishMeasurements();
        progressBar->setValue(0);
        for (int i = 0; i < SerNumModel::serNumCount(); ++i) {
            TestModel::instance()->saveProtokol(SerNumModel::serNum(i), i);
            TestModel::instance()->showProtocol(i);
        }
        QMessageBox::information(nullptr, "", "Проверка закончена.");
    }
}

void AutoMeasure::showEvent(QShowEvent* event)
{
    if (mi::man->isConnected()) {
        connect(mi::man, &MAN2::measuresCompleted, m_model, &ManModel::setMeasuredValues);
        m_timerRms.start(100);
        setEnabled(true);
    } else
        setEnabled(false);
    QWidget::showEvent(event);
}

void AutoMeasure::hideEvent(QHideEvent* event)
{
    disconnect(mi::man, &MAN2::measuresCompleted, m_model, &ManModel::setMeasuredValues);
    m_timerRms.stop();
    QWidget::hideEvent(event);
}
