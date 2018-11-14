#include "mesuremodel.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <mydialog.h>

#include <preparation/devicemodel.h>

int id4 = qRegisterMetaType<QVector<int>>("QVector<int>");

MesureModel* MesureModel::self = nullptr;

MesureModel::MesureModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_paths(8)
    , m_serNum(8)
{
    self = this;
}

MesureModel::~MesureModel()
{
    self = nullptr;
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
            return m_data[index.column()].test2;
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
            return QString("Тест №%1").arg(section + 1);
    }
    return QVariant();
}

Qt::ItemFlags MesureModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;
}

void MesureModel::reset()
{
    for (int i = 0; i < 8; ++i) {
        m_data[i].reset();
        m_paths[i].clear();
        m_serNum[i].clear();
    }
    dataChanged(createIndex(0, 0), createIndex(6, 7), { Qt::DisplayRole });
}

void MesureModel::setTest1(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i)
        m_data[i].test1 = list[i].Value1;
    dataChanged(createIndex(0, 0), createIndex(0, 7), { Qt::DisplayRole });
}

void MesureModel::setTest2(int ch, bool result)
{
    m_data[ch].test2 = result;
    dataChanged(createIndex(1, ch), createIndex(1, ch), { Qt::DisplayRole });
}

void MesureModel::setTest3(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i)
        m_data[i].test3 = list[i].Value1;
    dataChanged(createIndex(2, 0), createIndex(2, 7), { Qt::DisplayRole });
}

void MesureModel::setTest4(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i)
        m_data[i].test4 = list[i].Value1;
    dataChanged(createIndex(3, 0), createIndex(3, 7), { Qt::DisplayRole });
}

void MesureModel::setTest5(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i)
        m_data[i].test5 = list[i].Value1;
    dataChanged(createIndex(4, 0), createIndex(4, 7), { Qt::DisplayRole });
}

void MesureModel::setTest6(int ch, double value)
{
    m_data[ch].test6 = value;
    dataChanged(createIndex(5, ch), createIndex(5, ch), { Qt::DisplayRole });
}

void MesureModel::setTest7(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i)
        m_data[i].test7 = list[i].Value1;
    dataChanged(createIndex(6, 0), createIndex(6, 7), { Qt::DisplayRole });
}

void MesureModel::saveProtokol(const QString& serialNumber, int number)
{
    m_serNum[number] = serialNumber;
    QFile fileTop("Шапка.htm");
    QFile fileRow("Строка.htm");
    QFile fileBot("Подвал.htm");

    qDebug() << fileTop.open(QFile::ReadOnly);
    qDebug() << fileRow.open(QFile::ReadOnly);
    qDebug() << fileBot.open(QFile::ReadOnly);

    QString protocol;
    QString strTop = QString().fromLocal8Bit(fileTop.readAll());
    QString strRow = QString().fromLocal8Bit(fileRow.readAll());
    QString strBot = QString().fromLocal8Bit(fileBot.readAll());

    fileTop.close();
    fileRow.close();
    fileBot.close();

    protocol.append(strTop.arg(DeviceModel::self->scanSettings().Cipher)
                        .arg(serialNumber)
                        .arg(QString::number(DeviceModel::self->scanSettings().RatedVoltage - DeviceModel::self->scanSettings().RestrictionTest2).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().RatedVoltage + DeviceModel::self->scanSettings().RestrictionTest2).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().VisualControl).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().RestrictionsTest7Min).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().RestrictionsTest7Max).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().Voltageerrortest3_4U1).replace('.', ','))
                        .arg(QString::number(DeviceModel::self->scanSettings().Voltageerrortest5U1).replace('.', ',')));

    const int rowCount = DeviceModel::self->scanSettings().NumberOfChannels;

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
                val = m_data[row + number * rowCount].test1;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(DeviceModel::self->scanSettings().RatedVoltage - val) > DeviceModel::self->scanSettings().RestrictionTest2) {
                    flags[col - 1] = true;
                }
                break;
            case 2:
                val = m_data[row + number * rowCount].test2;
                if (val > 0) {
                    str = "в норме";
                } else {
                    flags[col - 1] = true;
                    str = "отказ";
                }
                break;
            case 3:
                val = m_data[row + number * rowCount].test1 - m_data[row + number * rowCount].test3;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::self->scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 4:
                val = m_data[row + number * rowCount].test1 - m_data[row + number * rowCount].test4;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::self->scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 5:
                val = m_data[row + number * rowCount].test1 - m_data[row + number * rowCount].test5;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::self->scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 6:
                val = m_data[row + number * rowCount].test6;
                str = QString::number(val, 'f', 1).replace('.', ',');
                if ((DeviceModel::self->scanSettings().RestrictionsTest7Min) > val || val > (DeviceModel::self->scanSettings().RestrictionsTest7Max)) {
                    flags[col - 1] = true;
                }
                if (0.0 > val) {
                    flags[col - 1] = true;
                    //str = "отказ";
                }
                break;
            case 7:
                val = m_data[row + number * rowCount].test7;
                str = QString::number(val, 'f', 3).replace('.', ',');
                if (val > DeviceModel::self->scanSettings().VoltageErrorTest7) {
                    flags[col - 1] = true;
                    //str = "отказ";
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
    }
    protocol.append(strBot.arg(QDate::currentDate().toString("dd.MM.yyyy"))
                        .arg(DeviceModel::self->scanSettings().Fio));

    QString path = qApp->applicationDirPath()
                       .append("/")
                       .append(DeviceModel::self->scanSettings().Type)
                       .append(QDate::currentDate().toString("/yyyy"));

    if (!QDir().mkpath(path)) {
        QMessageBox::critical(reinterpret_cast<QWidget*>(parent()), "", "Не удaётся записать на диск файл протокола!");
        return;
    }
    path = path.append("/")
               .append(serialNumber)
               .append(QDateTime::currentDateTime().toString(" dd.MM.yyyy H.mm.ss"))
               .append(".htm");

    m_paths[number] = path;

    QFile file4(path);
    qDebug() << file4.open(QFile::WriteOnly);
    qDebug() << file4.write(protocol.toLocal8Bit());
    file4.close();
}

void MesureModel::showProtocol(int num)
{
    if (m_serNum[num].isEmpty()) {
        QMessageBox::warning(reinterpret_cast<QWidget*>(parent()), "", "Протокол не создан!");
        return;
    }
    MyDialog* Dialog = new MyDialog(reinterpret_cast<QWidget*>(parent()), m_serNum[num]);
    Dialog->LoadFile(m_paths[num]);
    // Dialog->exec();
    // Dialog->deleteLater();
}
