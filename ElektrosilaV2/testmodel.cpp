#include "testmodel.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <devicemodel.h>
#include <mydialog.h>

const int id4 = qRegisterMetaType<QVector<int>>("QVector<int>");

TestModel* TestModel::instance()
{
    if (!m_instance)
        exit(-10);
    return m_instance;
}

TestModel::TestModel(QObject* parent, const QVector<bool>* hChecked, const QVector<bool>* vChecked)
    : QAbstractTableModel(parent)
    , m_hChecked(hChecked)
    , m_vChecked(vChecked)
{
    reset();
    QFile file("TestModel.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file); // read the data serialized from the file
        for (auto& var : m_data)
            in >> var;
    }
    m_instance = this;
}

TestModel::~TestModel()
{
    m_instance = nullptr;
    QFile file("TestModel.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file); // we will serialize the data into the file
        for (const auto& var : m_data)
            out << var;
    }
}

int TestModel::rowCount(const QModelIndex&) const { return TestCount; }

int TestModel::columnCount(const QModelIndex&) const { return Channels; }

QVariant TestModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (index.row()) {
        case 0:
            return m_data[index.column()].test1;
        case 1:
            switch (m_data[index.column()].test2) {
            case TestData::Undefined:
                return "";
            case TestData::True:
                return "в норме";
            case TestData::False:
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
        //    case Qt::ToolTipRole:
        //        switch (index.row()) {
        //        case 0:
        //            return "Измерение выходного напряжения при номинальной нагрузке и входном напряжении 220В.";
        //        case 1:
        //            return "Проверка пульсаций выходного напряжения  при номинальной нагрузке и входном напряжении 130В.";
        //        case 2:
        //            return "Измерение отклонения выходного напряжения от номинального при номинальной нагрузке и входном напряжении 130В.";
        //        case 3:
        //            return "Измерение отклонения выходного напряжения от номинального при номинальной нагрузке и входном напряжении 250В.";
        //        case 4:
        //            return "Измерение отклонения выходного напряжения холостого хода от номинального при входном напряжении 220В.";
        //        case 5:
        //            return "Ток срабатывания электронной защиты";
        //        case 6:
        //            return "Срабатывание электронной защиты при коротком замыкании";
        //        }
    case Qt::BackgroundColorRole:
        if (m_currentTest == index.row())
            return QColor(255, 127, 127);
        else if (!(flags(index) & Qt::ItemIsEnabled))
            return QColor(240, 240, 240);
        else
            return QVariant();
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return QString("№%1").arg(section + 1);
        else
            switch (section) {
            case 0:
                return QString("       Измерение выходного\n"
                               "       напряжения при номинальной\n"
                               "       нагрузке и входном напряжении %1В.")
                    .arg(220);
            case 1:
                return QString("       Проверка пульсаций выходного\n"
                               "       напряжения при номинальной нагрузке и\n"
                               "       входном напряжении %1В.")
                    .arg(130);
            case 2:
                return QString("       Измерение отклонения выходного напряжения\n"
                               "       от номинального при номинальной\n"
                               "       нагрузке и входном напряжении %1В.")
                    .arg(130);
            case 3:
                return QString("       Измерение отклонения выходного напряжения\n"
                               "       от номинального при номинальной\n"
                               "       нагрузке и входном напряжении %1В.")
                    .arg(250);
            case 4:
                return QString("       Измерение отклонения выходного\n"
                               "       напряжения холостого хода от номинального\n"
                               "       при входном напряжении %1В.")
                    .arg(220);
            case 5:
                return QString("       Ток срабатывания\n"
                               "       электронной защиты");
            case 6:
                return QString("       Срабатывание электронной защиты\n"
                               "       при коротком замыкании");
            }
    }
    return QVariant();
}

Qt::ItemFlags TestModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags{}; // = Qt::ItemIsEditable;
    bool enabled = false;
    if (m_hChecked && m_vChecked)
        enabled = m_hChecked->value(index.column()) & m_vChecked->value(index.row());
    else if (m_hChecked)
        enabled = m_hChecked->value(index.column());
    else if (m_vChecked)
        enabled = m_vChecked->value(index.row());
    else
        enabled = true;
    return flags | (enabled ? Qt::ItemIsEnabled /*| Qt::ItemIsSelectable */ : Qt::NoItemFlags);

    return Qt::ItemIsEnabled;
}

void TestModel::reset()
{
    for (int i = 0; i < 8; ++i) {
        m_data[i].reset(m_vChecked);
        m_paths[i].clear();
        m_serNum[i].clear();
    }
    dataChanged(createIndex(Test1, 0), createIndex(Test7, 7), { Qt::DisplayRole });
}

void TestModel::setCurrentTest(int val)
{
    m_currentTest = val;
    dataChanged(createIndex(Test1, 0), createIndex(Test7, 7));
}

void TestModel::setTest1(const QMap<int, MeasuredValue>& data)
{
    QMapIterator i(data);
    while (i.hasNext()) {
        i.next();
        if (i.key() > 8)
            continue;
        m_data[i.key() - 1].test1 = static_cast<double>(i.value().valCh1);
    }
    dataChanged(createIndex(Test1, 0), createIndex(Test1, 7), { Qt::DisplayRole });
}

void TestModel::setTest2(int ch, bool result)
{
    if (!m_instance || ch > 7)
        return;
    m_data[ch].test2 = result ? TestData::True : TestData::False;
    dataChanged(createIndex(Test2, ch), createIndex(Test2, ch), { Qt::DisplayRole });
}

void TestModel::setTest3(const QMap<int, MeasuredValue>& data)
{
    QMapIterator i(data);
    while (i.hasNext()) {
        i.next();
        if (i.key() > 8)
            continue;
        m_data[i.key() - 1].test3 = static_cast<double>(i.value().valCh1);
    }
    dataChanged(createIndex(Test3, 0), createIndex(Test3, 7), { Qt::DisplayRole });
}

void TestModel::setTest4(const QMap<int, MeasuredValue>& data)
{
    QMapIterator i(data);
    while (i.hasNext()) {
        i.next();
        if (i.key() > 8)
            continue;
        m_data[i.key() - 1].test4 = static_cast<double>(i.value().valCh1);
    }
    dataChanged(createIndex(Test4, 0), createIndex(Test4, 7), { Qt::DisplayRole });
}

void TestModel::setTest5(const QMap<int, MeasuredValue>& data)
{
    QMapIterator i(data);
    while (i.hasNext()) {
        i.next();
        if (i.key() > 8)
            continue;
        m_data[i.key() - 1].test5 = static_cast<double>(i.value().valCh1);
    }
    dataChanged(createIndex(Test5, 0), createIndex(Test5, 7), { Qt::DisplayRole });
}

void TestModel::setTest6(int ch, double value)
{
    if (!m_instance || ch > 7)
        return;
    m_data[ch].test6 = value;
    dataChanged(createIndex(Test6, ch), createIndex(Test6, ch), { Qt::DisplayRole });
}

void TestModel::setTest7(const QMap<int, MeasuredValue>& data)
{
    QMapIterator i(data);
    while (i.hasNext()) {
        i.next();
        if (i.key() > 8)
            continue;
        m_data[i.key() - 1].test7 = static_cast<double>(i.value().valCh1);
    }
    dataChanged(createIndex(Test7, 0), createIndex(Test7, 7), { Qt::DisplayRole });
}

void TestModel::saveProtokol(const QString& serialNumber, int number)
{
    if (serialNumber.isEmpty())
        return;

    m_serNum[number] = serialNumber;

    QFile fileTop("Шапка.htm");
    QFile fileRow("Строка.htm");
    QFile fileBot("Подвал.htm");

    fileTop.open(QFile::ReadOnly);
    fileRow.open(QFile::ReadOnly);
    fileBot.open(QFile::ReadOnly);

    QString protocol;
    QString strTop(QString().fromLocal8Bit(fileTop.readAll()));
    QString strRow(QString().fromLocal8Bit(fileRow.readAll()));
    QString strBot(QString().fromLocal8Bit(fileBot.readAll()));

    fileTop.close();
    fileRow.close();
    fileBot.close();

    protocol.append(strTop.arg(DeviceModel::scanSettings().Cipher)
                        .arg(serialNumber)
                        .arg(QString::number(DeviceModel::scanSettings().RatedVoltage
                            - DeviceModel::scanSettings().RestrictionTest2)
                                 .replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RatedVoltage
                            + DeviceModel::scanSettings().RestrictionTest2)
                                 .replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().VisualControl).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RestrictionsTest7Min).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RestrictionsTest7Max).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().Voltageerrortest3_4U1).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().Voltageerrortest5U1).replace('.', ',')));

    const int rowCount = DeviceModel::scanSettings().NumberOfChannels;

    bool allOk = true;
    for (int row = 0; row < rowCount; ++row) {
        bool flags[]{ false, false, false, false, false, false, false };
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
                if (qAbs(DeviceModel::scanSettings().RatedVoltage - val) > DeviceModel::scanSettings().RestrictionTest2) {
                    flags[col - 1] = true;
                }
                break;
            case 2: {
                int val = m_data[row + number * rowCount].test2;
                if (val == TestData::True) {
                    str = "в норме";
                } else {
                    flags[col - 1] = true;
                    str = "отказ";
                }
            } break;
            case 3:
                val = m_data[row + number * rowCount].test1 - m_data[row + number * rowCount].test3;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 4:
                val = m_data[row + number * rowCount].test1 - m_data[row + number * rowCount].test4;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 5:
                val = m_data[row + number * rowCount].test1 - m_data[row + number * rowCount].test5;
                str = QString::number(val, 'f', 4).replace('.', ',');
                if (qAbs(val) > DeviceModel::scanSettings().LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 6:
                val = m_data[row + number * rowCount].test6;
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
                val = m_data[row + number * rowCount].test7;
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
        allOk = allOk && !flags[0] && !flags[1] && !flags[2] && !flags[3] && !flags[4] && !flags[5] && !flags[6];
    }

    protocol.append(strBot.arg(QDate::currentDate().toString("dd.MM.yyyy"))
                        .arg(DeviceModel::scanSettings().Fio)
                        .arg(/*!ok ? "; color:red" : */ "")
                        .arg(/*ok ? "Прибор соответствует требованиям ТУ" : "Прибор не соответствует требованиям ТУ"*/ ""));

    QString path = qApp->applicationDirPath()
                       .append("/")
                       .append(DeviceModel::scanSettings().Type)
                       .append(QDate::currentDate().toString("/yyyy"));

    if (!QDir().mkpath(path)) {
        QMessageBox::critical(reinterpret_cast<QWidget*>(parent()), "", "Не удaётся записать на диск файл протокола!");
        return;
    }
    path = path.append("/")
               .append(serialNumber)
               .append(QDateTime::currentDateTime().toString(" dd.MM.yyyy" /* H.mm.ss"*/))
               .append(".htm");

    m_paths[number] = path;

    QFile file4(path);
    qDebug() << file4.open(QFile::WriteOnly);
    qDebug() << file4.write(protocol.toLocal8Bit());
    file4.close();
}

void TestModel::showProtocol(int num)
{
    if (m_serNum[num].isEmpty()) {
        QMessageBox::warning(reinterpret_cast<QWidget*>(parent()), "", "Протокол не создан!");
        return;
    }
    MyDialog* Dialog = new MyDialog(reinterpret_cast<QWidget*>(parent()), m_serNum[num]);
    Dialog->LoadFile(m_paths[num]);
}

bool TestModel::dontSkip(int num) { return m_vChecked->size() ? m_vChecked->at(num) : false; }

void TestModel::onChecked(int index, int orientation)
{
    if (orientation == Qt::Horizontal)
        emit dataChanged(createIndex(0, index), createIndex(rowCount() - 1, index), { Qt::DisplayRole });
    else
        emit dataChanged(createIndex(index, 0), createIndex(index, columnCount() - 1), { Qt::DisplayRole });
}
