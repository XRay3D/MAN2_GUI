#include "automaticmeasurements.h"
#include "inputparameters.h"
#include "mydialog.h"
#include "worker.h"
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QSettings>
#include <QDate>
#include <QDir>
#include "hw/interface.h"

const ScanSettings AutomaticMeasurements::m_scanSettings;

AutomaticMeasurements::AutomaticMeasurements(QWidget* parent)
    : QWidget(parent)
    , m_paths(8)
    , m_serNum(8)
{
    setupUi(this);

    m_listCheckBox = QList<QCheckBox*>({ checkBox_1, checkBox_2, checkBox_3, checkBox_4, checkBox_5, checkBox_6, checkBox_7 });
    m_listDsbVoltage = QList<QDoubleSpinBox*>({ dsbVoltage_1, dsbVoltage_2, dsbVoltage_3, dsbVoltage_4, dsbVoltage_5, dsbVoltage_6, dsbVoltage_7, dsbVoltage_8 });
    m_listDsbCurrent = QList<QDoubleSpinBox*>({ dsbCurrent_1, dsbCurrent_2, dsbCurrent_3, dsbCurrent_4, dsbCurrent_5, dsbCurrent_6, dsbCurrent_7, dsbCurrent_8 });

    for (int i = 0; i < listWidget->count(); ++i) {
        m_doNotSkip[i] = true;
        listWidget->item(i)->setText("");
        listWidget->item(i)->setHidden(true);
    }

    //    QFont font;
    //    font.setPixelSize(20);
    //    listWidget->setFont(font);
    //    font.setPixelSize(14);
    //    widget->setFont(font);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &AutomaticMeasurements::ItemDoubleClicked);
    //    m_paths.resize(8);
    //    m_serNum.resize(8);
    connect(&m_timerRms, &QTimer::timeout, [&]() { dsbVoltageRms->setValue(MI::man()->GetMeasuredValueRMS()); });
}

AutomaticMeasurements::~AutomaticMeasurements()
{
}

void AutomaticMeasurements::ScanSettingsSlot(const ScanSettings* scanSettings)
{
    *const_cast<ScanSettings*>(&m_scanSettings) = *scanSettings;
}

void AutomaticMeasurements::SerialNumberChanged(const QString& serialNumber, int index)
{
    listWidget->item(index)->setText(serialNumber);

    if (serialNumber.isEmpty()) {
        listWidget->item(index)->setHidden(true);
    }
    else {
        m_serNum[index] = serialNumber;
        listWidget->item(index)->setHidden(false);
    }

    for (bool& val : m_doNotSkip)
        val = true;

    for (int i = 0, end = listWidget->count(); i < end; ++i) {
        if (listWidget->item(i)->isHidden()) {
            for (int j = 0; j < m_scanSettings.NumberOfChannels; ++j) {
                m_doNotSkip[i * int(m_scanSettings.NumberOfChannels) + j] = false;
            }
        }
    }
}

void AutomaticMeasurements::ShowMessage(int num)
{
    static int stage = 0;
    QString messageText;
    switch (num) {
    case SET_INPUT_VOLTAGE_0:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В").arg(m_scanSettings.Voltageerrortest5U1).arg(m_scanSettings.Voltageerrortest5U2).replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            m_worker->FinishMeasurements();
        else
            m_worker->Continue();
        break;
    case SET_INPUT_VOLTAGE_1:
        stage = num;
        messageText = QString("Установите входное напряжение %1±%2В").arg(m_scanSettings.Voltageerrortest3_4U1).arg(m_scanSettings.Voltageerrortest3_4U2).replace('.', ',');
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            m_worker->FinishMeasurements();
        else
            m_worker->Continue();
        break;
    case SET_INPUT_VOLTAGE_2:
        stage = num;
        messageText = "Установите входное напряжение 220±4,4В";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            m_worker->FinishMeasurements();

        else
            m_worker->Continue();
        break;
    case NO_CONNECTION_WITH_MAN:
        messageText = "Нет связи с МАНом 2!";
        if (QMessageBox::critical(this, "", messageText, "Повторить", "Остановить измерения"))
            m_worker->FinishMeasurements();
        else
            m_worker->Continue();
        break;
    case RESTORE_THE_OPERATION_OF_CHANNELS:
        messageText = "Восстановите работу каналов блока питания";
        if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
            m_worker->FinishMeasurements();
        else
            m_worker->Continue();
        break;
    case PULSATIONS_ON_THE_CHANNEL_1:
    case PULSATIONS_ON_THE_CHANNEL_2:
    case PULSATIONS_ON_THE_CHANNEL_3:
    case PULSATIONS_ON_THE_CHANNEL_4:
    case PULSATIONS_ON_THE_CHANNEL_5:
    case PULSATIONS_ON_THE_CHANNEL_6:
    case PULSATIONS_ON_THE_CHANNEL_7:
    case PULSATIONS_ON_THE_CHANNEL_8:
        messageText = QString("Пульсации в канале %2 соответствуют норме <%1мВ ?").arg(m_scanSettings.VisualControl).arg(num - (PULSATIONS_ON_THE_CHANNEL_1 - 1)).replace('.', ',');
        switch (QMessageBox::question(this, "", messageText, "Да", "Нет", "Остановить измерения")) {
        case 0:
            m_worker->Continue();
            m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = 1.0;
            break;
        case 1:
            m_result[num - PULSATIONS_ON_THE_CHANNEL_1].test2 = -1.0;
            m_worker->Continue();
            break;
        case 2:
            m_worker->FinishMeasurements();
            break;
        default:
            break;
        }
        break;
    case VERIFICATION_IS_COMPLETE:
        messageText = "Проверка закончена.";
        QMessageBox::information(this, "", messageText);
        for (int i = 0; i < 8; ++i) {
            if (!listWidget->item(i)->isHidden()) {
                SaveProtokol(listWidget->item(i)->text(), i);
            }
            if (!m_paths[i].isEmpty()) {
                ShowProtocol(i);
            }
        }
        break;
    case TEST_1:
    case TEST_2:
    case TEST_3:
    case TEST_4:
    case TEST_5:
    case TEST_6:
    case TEST_7:
        m_listCheckBox[num - TEST_1]->setChecked(true);
        break;
    case CHECK_INPUT_VOLTAGE:
        switch (stage) {
        case 0:
            messageText = QString("Установите входное напряжение %1±%2В").arg(m_scanSettings.Voltageerrortest5U1).arg(m_scanSettings.Voltageerrortest5U2).replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                m_worker->FinishMeasurements();
            else
                m_worker->Continue();
            break;
        case 1:
            messageText = QString("Установите входное напряжение %1±%2В").arg(m_scanSettings.Voltageerrortest3_4U1).arg(m_scanSettings.Voltageerrortest3_4U2).replace('.', ',');
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                m_worker->FinishMeasurements();
            else
                m_worker->Continue();
            break;
        case 2:
            stage = num;
            messageText = "Установите входное напряжение 220±4,4В";
            if (QMessageBox::information(this, "", messageText, "Ок", "Остановить измерения"))
                m_worker->FinishMeasurements();
            else
                m_worker->Continue();
            break;
        default:
            break;
        }

        break;
    default:
        break;
    }
}

void AutomaticMeasurements::ShowProtocol(int num)
{
    MyDialog* Dialog = new MyDialog(this, m_serNum[num]);
    Dialog->LoadFile(m_paths[num]);
    // Dialog->exec();
    // Dialog->deleteLater();
}

void AutomaticMeasurements::ItemDoubleClicked(QListWidgetItem* item)
{
    ShowProtocol(listWidget->row(item));
}

void AutomaticMeasurements::UpdateProgresBar()
{
    if (progressBar->value() == progressBar->maximum()) {
        progressBar->setValue(0);
    }
    progressBar->setValue(progressBar->value() + 1);
}

void AutomaticMeasurements::SaveProtokol(const QString& serialNumber, int number)
{
    QFile file1("Шапка.htm");
    QFile file2("Строка.htm");
    QFile file3("Подвал.htm");

    qDebug() << file1.open(QFile::ReadOnly);
    qDebug() << file2.open(QFile::ReadOnly);
    qDebug() << file3.open(QFile::ReadOnly);

    QString protocol;
    QString cap = QString().fromLocal8Bit(file1.readAll());
    QString rowStr = QString().fromLocal8Bit(file2.readAll());
    QString basement = QString().fromLocal8Bit(file3.readAll());

    protocol.append(cap.arg(m_scanSettings.Cipher)
                        .arg(serialNumber)
                        .arg(QString::number(m_scanSettings.RatedVoltage - m_scanSettings.RestrictionTest2).replace('.', ','))
                        .arg(QString::number(m_scanSettings.RatedVoltage + m_scanSettings.RestrictionTest2).replace('.', ','))
                        .arg(QString::number(m_scanSettings.VisualControl).replace('.', ','))
                        .arg(QString::number(m_scanSettings.LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(m_scanSettings.LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(m_scanSettings.RestrictionsTest7Min).replace('.', ','))
                        .arg(QString::number(m_scanSettings.RestrictionsTest7Max).replace('.', ','))
                        .arg(QString::number(m_scanSettings.Voltageerrortest3_4U1).replace('.', ','))
                        .arg(QString::number(m_scanSettings.Voltageerrortest5U1).replace('.', ',')));

    const int rowCount = m_scanSettings.NumberOfChannels;

    for (int row = 0; row < rowCount; ++row) {
        bool flags[7] = { false, false, false, false, false, false, false };
        QString str;
        QStringList m_list;
        m_list.clear();
        double val;
        for (int col = 0; col < 8; ++col) {
            switch (col) {
            case 0:
                str = QString::number(row + 1);
                break;
            case 1:
                val = m_result[row + number * rowCount].test1;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(m_scanSettings.RatedVoltage - val) > m_scanSettings.RestrictionTest2) {
                    flags[col - 1] = true;
                }
                break;
            case 2:
                val = m_result[row + number * rowCount].test2;
                if (val > 0) {
                    str = "в норме";
                }
                else {
                    flags[col - 1] = true;
                    str = "отказ";
                }
                break;
            case 3:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test3;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 4:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test4;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 5:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test5;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 6:
                val = m_result[row + number * rowCount].test6;
                str = QString::number(val, 'f', 1).replace('.', ',');
                if ((m_scanSettings.RestrictionsTest7Min) > val || val > (m_scanSettings.RestrictionsTest7Max)) {
                    flags[col - 1] = true;
                }
                if (0.0 > val) {
                    flags[col - 1] = true;
                    //str = "отказ";
                }
                break;
            case 7:
                val = m_result[row + number * rowCount].test7;
                str = QString::number(val, 'f', 3).replace('.', ',');
                if (val > m_scanSettings.VoltageErrorTest7) {
                    flags[col - 1] = true;
                    //str = "отказ";
                }
                break;
            default:
                break;
            }
            m_list.append(str);
        }
        protocol.append(rowStr
                            .arg(m_list[0])
                            .arg(m_list[1])
                            .arg(m_list[2])
                            .arg(m_list[3])
                            .arg(m_list[4])
                            .arg(m_list[5])
                            .arg(m_list[6])
                            .arg(m_list[7])
                            .arg(flags[0] ? "; color:red" : "")
                            .arg(flags[1] ? "; color:red" : "")
                            .arg(flags[2] ? "; color:red" : "")
                            .arg(flags[3] ? "; color:red" : "")
                            .arg(flags[4] ? "; color:red" : "")
                            .arg(flags[5] ? "; color:red" : "")
                            .arg(flags[6] ? "; color:red" : ""));
    }
    protocol.append(basement.arg(QDate::currentDate().toString("dd.MM.yyyy"))
                        .arg(m_scanSettings.Fio));

    QString path = qApp->applicationDirPath()
                       .append("/")
                       .append(m_scanSettings.Type)
                       .append(QDate::currentDate().toString("/yyyy"));

    if (!QDir().mkpath(path)) {
        QMessageBox::critical(this, "", "Не удaётся записать на диск файл протокола!");
        return;
    }
    path = path.append("/")
               .append(serialNumber)
               .append(QDate::currentDate().toString("_dd.MM.yyyy"))
               .append("г.htm");

    m_paths[number] = path;

    QFile file4(path);
    qDebug() << file4.open(QFile::WriteOnly);
    qDebug() << file4.write(protocol.toLocal8Bit());
    file1.close();
    file2.close();
    file3.close();
    file4.close();
    // MyDialog* Dialog = new MyDialog(this, "123");
    // Dialog->LoadFile(qApp->applicationDirPath().append("/").append("123.htm"));
}

void AutomaticMeasurements::GetMeasuredValueSlot(const QMap<int, MeasuredValue_t>& m_list)
{
    QMapIterator<int, MeasuredValue_t> iterator(m_list);
    while (iterator.hasNext()) {
        iterator.next();
        m_listDsbVoltage[iterator.key() - 1]->setValue(iterator.value().Value1);
        m_listDsbCurrent[iterator.key() - 1]->setValue(iterator.value().Value2);
    }
}

void AutomaticMeasurements::on_pbStartStop_clicked(bool checked)
{
    pbStartStop->setChecked(checked);
    if (checked) {
        for (int i = 0, flag = 0; i < listWidget->count(); ++i) {
            if (listWidget->item(i)->isHidden()) {
                ++flag;
            }
            if (flag == 8) {
                QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
                pbStartStop->setChecked(false);
                return;
            }
        }
        foreach (QCheckBox* cb, m_listCheckBox) {
            cb->setChecked(false);
        }
        pbStartStop->setText("Остановить измерения");
        memset(m_result, 0, sizeof(m_result));
        m_worker = new Worker(m_doNotSkip, m_result, &m_scanSettings);
        connect(m_worker, &QThread::finished, [this]() {
            pbStartStop->setText("Начать измерения");
            pbStartStop->setChecked(false);
        });
        connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_worker, &Worker::ShowMessage, this, &AutomaticMeasurements::ShowMessage);
        connect(m_worker, &Worker::UpdateProgresBar, this, &AutomaticMeasurements::UpdateProgresBar);
        progressBar->setValue(0);
        m_worker->start();
    }
    else {
        pbStartStop->setText("Начать измерения");
        m_worker->FinishMeasurements();
    }
}

void AutomaticMeasurements::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    if (MI::man()->IsConnected()) {
        connect(MI::man(), &MAN2::GetMeasuredValueSignal, this, &AutomaticMeasurements::GetMeasuredValueSlot);
        m_timerRms.start(100);
        return;
    }
    setEnabled(false);
}
void AutomaticMeasurements::hideEvent(QHideEvent* event)
{
    disconnect(MI::man(), &MAN2::GetMeasuredValueSignal, this, &AutomaticMeasurements::GetMeasuredValueSlot);
    m_timerRms.stop();
}
