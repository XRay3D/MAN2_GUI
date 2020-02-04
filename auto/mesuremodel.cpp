#include "mesuremodel.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <mydialog.h>

#include <preparation/devicemodel.h>

const int id4 = qRegisterMetaType<QVector<int>>("QVector<int>");

MesureModel* MesureModel::instance = nullptr;

MesureModel::MesureModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_paths(8)
    , m_serNum(8)
{
    instance = this;
}

MesureModel::~MesureModel()
{
    instance = nullptr;
}

int MesureModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 7;
}

int MesureModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 8;
}

QVariant MesureModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (index.row()) {
        case 0:
            return m_data[index.column()].test1;
        case 1:
            switch (m_data[index.column()].test2) {
            case Result_t::Undefined:
                return "";
            case Result_t::True:
                return "в норме";
            case Result_t::False:
                return "отказ";
            }
        case 2:
            return m_data[index.column()].test3;
        case 3:
            return m_data[index.column()].test4;
        case 4:
            return m_data[index.column()].test5;
        case 5:
            return m_data[index.column()].test6;
        case 6:
            return m_data[index.column()].test7;
        }
    case Qt::ToolTipRole:
        switch (index.row()) {
        case 0:
            return "Измерение выходного напряжения при номинальной нагрузке и входном напряжении 220В.";
        case 1:
            return "Проверка пульсаций выходного напряжения  при номинальной нагрузке и входном напряжении 130В.";
        case 2:
            return "Измерение отклонения выходного напряжения от номинального при номинальной нагрузке и входном напряжении 130В.";
        case 3:
            return "Измерение отклонения выходного напряжения от номинального при номинальной нагрузке и входном напряжении 250В.";
        case 4:
            return "Измерение отклонения выходного напряжения холостого хода от номинального при входном напряжении 220В.";
        case 5:
            return "Ток срабатывания электронной защиты";
        case 6:
            return "Срабатывание электронной защиты при коротком замыкании";
        }
    case Qt::BackgroundColorRole:
        if (m_currentTest == index.row())
            return QColor(255, 127, 127);
        else
            return QVariant();
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant MesureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return QString("Канал №%1").arg(section + 1);
        else
            switch (section) {
            case 0:
                return "Измерение выходного\n"
                       "напряжения при номинальной\n"
                       "нагрузке и входном напряжении 220В.";
            case 1:
                return "Проверка пульсаций\n"
                       "выходного напряжения  при\n"
                       "номинальной нагрузке и входном напряжении 130В.";
            case 2:
                return "Измерение отклонения\n"
                       "выходного напряжения от номинального\n"
                       "при номинальной нагрузке и входном напряжении 130В.";
            case 3:
                return "Измерение отклонения\n"
                       "выходного напряжения от номинального\n"
                       "при номинальной нагрузке и входном напряжении 250В.";
            case 4:
                return "Измерение отклонения\n"
                       "выходного напряжения холостого хода\n"
                       "от номинального при входном напряжении 220В.";
            case 5:
                return "Ток срабатывания\n"
                       "электронной защиты";
            case 6:
                return "Срабатывание\n"
                       "электронной защиты при коротком замыкании";
            }
    }
    return QVariant();
}

Qt::ItemFlags MesureModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;
}

void MesureModel::reset()
{
    if (!instance)
        return;
    for (int i = 0; i < 8; ++i) {
        instance->m_data[i].reset();
        instance->m_paths[i].clear();
        instance->m_serNum[i].clear();
    }
    instance->dataChanged(instance->createIndex(0, 0), instance->createIndex(6, 7), { Qt::DisplayRole });
}

void MesureModel::setCurrentTest(int val)
{
    if (!instance)
        return;
    instance->m_currentTest = val;
    instance->dataChanged(instance->createIndex(0, 0), instance->createIndex(6, 7));
}

void MesureModel::setTest1(const QList<MeasuredValue_t>& list)
{
    if (!instance)
        return;
    for (int i = 0; i < list.size(); ++i)
        instance->m_data[i].test1 = static_cast<double>(list[i].Value1);
    instance->dataChanged(instance->createIndex(0, 0), instance->createIndex(0, 7), { Qt::DisplayRole });
}

void MesureModel::setTest2(int ch, bool result)
{
    if (!instance)
        return;
    instance->m_data[ch].test2 = result ? Result_t::True : Result_t::False;
    instance->dataChanged(instance->createIndex(1, ch), instance->createIndex(1, ch), { Qt::DisplayRole });
}

void MesureModel::setTest3(const QList<MeasuredValue_t>& list)
{
    if (!instance)
        return;
    for (int i = 0; i < list.size(); ++i)
        instance->m_data[i].test3 = static_cast<double>(list[i].Value1);
    instance->dataChanged(instance->createIndex(2, 0), instance->createIndex(2, 7), { Qt::DisplayRole });
}

void MesureModel::setTest4(const QList<MeasuredValue_t>& list)
{
    if (!instance)
        return;
    for (int i = 0; i < list.size(); ++i)
        instance->m_data[i].test4 = static_cast<double>(list[i].Value1);
    instance->dataChanged(instance->createIndex(3, 0), instance->createIndex(3, 7), { Qt::DisplayRole });
}

void MesureModel::setTest5(const QList<MeasuredValue_t>& list)
{
    if (!instance)
        return;
    for (int i = 0; i < list.size(); ++i)
        instance->m_data[i].test5 = static_cast<double>(list[i].Value1);
    instance->dataChanged(instance->createIndex(4, 0), instance->createIndex(4, 7), { Qt::DisplayRole });
}

void MesureModel::setTest6(int ch, double value)
{
    if (!instance)
        return;
    instance->m_data[ch].test6 = value;
    instance->dataChanged(instance->createIndex(5, ch), instance->createIndex(5, ch), { Qt::DisplayRole });
}

void MesureModel::setTest7(const QList<MeasuredValue_t>& list)
{
    if (!instance)
        return;
    for (int i = 0; i < list.size(); ++i)
        instance->m_data[i].test7 = static_cast<double>(std::min(list[i].Value1, list[i].Value2));
    instance->dataChanged(instance->createIndex(6, 0), instance->createIndex(6, 7), { Qt::DisplayRole });
}

void MesureModel::saveProtokol(const QString& serialNumber, int number)
{
    instance->m_serNum[number] = serialNumber;
    QFile fileTop("Шапка.htm");
    QFile fileRow("Строка.htm");
    QFile fileBot("Подвал.htm");

    qDebug() << fileTop.open(QFile::ReadOnly);
    qDebug() << fileRow.open(QFile::ReadOnly);
    qDebug() << fileBot.open(QFile::ReadOnly);

    QString protocol;
    QString strTop(QString().fromLocal8Bit(fileTop.readAll()));
    QString strRow(QString().fromLocal8Bit(fileRow.readAll()));
    QString strBot(QString().fromLocal8Bit(fileBot.readAll()));

    fileTop.close();
    fileRow.close();
    fileBot.close();

    protocol.append(strTop.arg(DeviceModel::scanSettings().Cipher)
                        .arg(serialNumber)
                        .arg(QString::number(DeviceModel::scanSettings().RatedVoltage - DeviceModel::scanSettings().RestrictionTest2).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RatedVoltage + DeviceModel::scanSettings().RestrictionTest2).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().VisualControl).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RestrictionsTest7Min).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RestrictionsTest7Max).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().Voltageerrortest3_4U1).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().Voltageerrortest5U1).replace('.', ',')));

    const int rowCount = DeviceModel::scanSettings().NumberOfChannels;

    bool ok = true;
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
                val = instance->m_data[row + number * rowCount].test1;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(DeviceModel::scanSettings().RatedVoltage - val) > DeviceModel::scanSettings().RestrictionTest2) {
                    flags[col - 1] = true;
                }
                break;
            case 2: {
                int val = instance->m_data[row + number * rowCount].test2;
                if (val == Result_t::True) {
                    str = "в норме";
                } else {
                    flags[col - 1] = true;
                    str = "отказ";
                }
            } break;
            case 3:
                val = instance->m_data[row + number * rowCount].test1 - instance->m_data[row + number * rowCount].test3;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 4:
                val = instance->m_data[row + number * rowCount].test1 - instance->m_data[row + number * rowCount].test4;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 5:
                val = instance->m_data[row + number * rowCount].test1 - instance->m_data[row + number * rowCount].test5;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 6:
                val = instance->m_data[row + number * rowCount].test6;
                str = QString::number(val, 'f', 1).replace('.', ',');
                if (DeviceModel::scanSettings().RestrictionsTest7Min > val || val > DeviceModel::scanSettings().RestrictionsTest7Max) {
                    flags[col - 1] = true;
                }
                if (0.0 > val) {
                    flags[col - 1] = true;
                    //str = "отказ";
                }
                break;
            case 7:
                val = instance->m_data[row + number * rowCount].test7;
                if (val > DeviceModel::scanSettings().VoltageErrorTest7) {
                    flags[col - 1] = true;
                    str = "отказ";
                } else {
                    str = "в норме";
                }
                break;
            default:
                break;
            }
            m_list.append(str);
        }
        protocol.append(strRow
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
        ok = ok && !flags[0] && !flags[1] && !flags[2] && !flags[3] && !flags[4] && !flags[5] && !flags[6];
    }

    protocol.append(strBot.arg(QDate::currentDate().toString("dd.MM.yyyy"))
                        .arg(DeviceModel::scanSettings().Fio)
                        .arg(!ok ? "; color:red" : "")
                        .arg(ok ? "Прибор соответствует требованиям ТУ" : "Прибор не соответствует требованиям ТУ"));

    QString path = qApp->applicationDirPath()
                       .append("/")
                       .append(DeviceModel::scanSettings().Type)
                       .append(QDate::currentDate().toString("/yyyy"));

    if (!QDir().mkpath(path)) {
        QMessageBox::critical(reinterpret_cast<QWidget*>(instance->parent()), "", "Не удaётся записать на диск файл протокола!");
        return;
    }
    path = path.append("/")
               .append(serialNumber)
               .append(QDateTime::currentDateTime().toString(" dd.MM.yyyy" /* H.mm.ss"*/))
               .append(".htm");

    instance->m_paths[number] = path;

    QFile file4(path);
    qDebug() << file4.open(QFile::WriteOnly);
    qDebug() << file4.write(protocol.toLocal8Bit());
    file4.close();
}

void MesureModel::showProtocol(int num)
{
    if (!instance)
        return;
    if (instance->m_serNum[num].isEmpty()) {
        QMessageBox::warning(reinterpret_cast<QWidget*>(instance->parent()), "", "Протокол не создан!");
        return;
    }
    MyDialog* Dialog = new MyDialog(reinterpret_cast<QWidget*>(instance->parent()), instance->m_serNum[num]);
    Dialog->LoadFile(instance->m_paths[num]);
}
