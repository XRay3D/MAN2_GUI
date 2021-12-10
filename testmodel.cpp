#include "testmodel.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <devicemodel.h>
#include <protodialog.h>

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
        for (auto& data : m_data)
            in >> data;
        for (auto& paths : m_paths)
            in >> paths;
        for (auto& serNum : m_serNum)
            in >> serNum;
    }
    m_instance = this;
}

TestModel::~TestModel()
{
    m_instance = nullptr;
    QFile file("TestModel.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file); // we will serialize the data into the file
        for (auto& data : m_data)
            out << data;
        for (auto& paths : m_paths)
            out << paths;
        for (auto& serNum : m_serNum)
            out << serNum;
    }
}

int TestModel::rowCount(const QModelIndex&) const { return TestCount; }

int TestModel::columnCount(const QModelIndex&) const { return ManCount; }

QVariant TestModel::data(const QModelIndex& index, int role) const
{
    QVariant rt;
    if (role == Qt::DisplayRole) {
        //        visit_at(m_data[index.column()], index.row(), [&rt]<class T>(T& arg) {
        //            if constexpr (std::is_same_v<std::decay_t<T>, double>)
        //                rt = arg;
        //            else
        //                switch (arg) {
        //                case TestData::Undefined:
        //                    rt = "";
        //                case TestData::True:
        //                    rt = "в норме";
        //                case TestData::False:
        //                    rt = "отказ";
        //                }
        //        });
        //        return rt;
        switch (index.row()) {
        case Test1:
            return m_data[index.column()].test1;
        case Test2:
            switch (m_data[index.column()].test2) {
            case TestData::Undefined:
                return "";
            case TestData::True:
                return "в норме";
            case TestData::False:
                return "отказ";
            }
        case Test3:
            return m_data[index.column()].test3;
        case Test4:
            return m_data[index.column()].test4;
        case Test5:
            return m_data[index.column()].test5;
        case Test6:
            return m_data[index.column()].test6;
        case Test7:
            return m_data[index.column()].test7;
        }
    }
    if (role == Qt::BackgroundColorRole) {
        if (m_currentTest == index.row())
            return QColor(255, 127, 127);
        else if (!(flags(index) & Qt::ItemIsEnabled))
            return QColor(240, 240, 240);
        else
            return QVariant();
    }
    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return QString("№%1").arg(section + 1);
        else
            switch (section) {
            case Test1:
                return QString("       Измерение выходного\n"
                               "       напряжения при номинальной\n"
                               "       нагрузке и входном напряжении %1В.")
                    .arg(220);
            case Test2:
                return QString("       Проверка пульсаций выходного\n"
                               "       напряжения при номинальной нагрузке и\n"
                               "       входном напряжении %1В.")
                    .arg(130);
            case Test3:
                return QString("       Измерение отклонения выходного напряжения\n"
                               "       от номинального при номинальной\n"
                               "       нагрузке и входном напряжении %1В.")
                    .arg(130);
            case Test4:
                return QString("       Измерение отклонения выходного напряжения\n"
                               "       от номинального при номинальной\n"
                               "       нагрузке и входном напряжении %1В.")
                    .arg(250);
            case Test5:
                return QString("       Измерение отклонения выходного\n"
                               "       напряжения холостого хода от номинального\n"
                               "       при входном напряжении %1В.")
                    .arg(220);
            case Test6:
                return QString("       Ток срабатывания\n"
                               "       электронной защиты");
            case Test7:
                return QString("       Срабатывание электронной защиты\n"
                               "       при коротком замыкании");
            }
    }
    return QVariant();
}

Qt::ItemFlags TestModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags {}; // = Qt::ItemIsEditable;
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
    for (int i = 0; i < ManCount; ++i) {
        m_data[i].reset(m_vChecked);
        m_paths[i].clear();
        m_serNum[i].clear();
    }
    dataChanged(createIndex(Test1, 0), createIndex(Test7, ManCount), { Qt::DisplayRole });
}

void TestModel::setCurrentTest(int val)
{
    m_currentTest = val;
    dataChanged(createIndex(Test1, 0), createIndex(Test7, ManCount));
}

void TestModel::setTestData(const MeasureMap& data)
{
    for (auto [key, value] : data) {
        if (key > ManCount)
            continue;
        visit_at(m_data[key - 1], m_currentTest, [value = value]<class T>(T& arg) {
            if constexpr (std::is_same_v<T, double>)
                arg = value.valCh1;
            else
                arg = T(value.valCh1);
        });
    }
    dataChanged(createIndex(m_currentTest, 0), createIndex(m_currentTest, 7), { Qt::DisplayRole });
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
                        .arg(QString::number(DeviceModel::scanSettings().RatedVoltage - DeviceModel::scanSettings().RestrictionTest2).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RatedVoltage + DeviceModel::scanSettings().RestrictionTest2).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().VisualControl).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().LimitationsTest4_5).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().LimitTest6).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RestrictionsTest7Min).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().RestrictionsTest7Max).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().Voltageerrortest3_4U1).replace('.', ','))
                        .arg(QString::number(DeviceModel::scanSettings().Voltageerrortest5U1).replace('.', ',')));

    const int rowCount = DeviceModel::scanSettings().NumberOfChannels;

    bool allOk = true;
    for (int row = 0; row < rowCount; ++row) {
        bool flags[] { false, false, false, false, false, false, false };
        QString str;
        QStringList m_list;
        m_list.clear();
        double val;
        for (int col = 0; col <= TestCount; ++col) {
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
                if (qAbs(val) > DeviceModel::scanSettings().LimitTest6) {
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
        //.arg(/*!ok ? "; color:red" : */ "")
        //.arg(/*ok ? "Прибор соответствует требованиям ТУ" : "Прибор не соответствует требованиям ТУ"*/ "")
    );

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
    //    if (m_serNum[num].isEmpty()) {
    //        QMessageBox::warning(reinterpret_cast<QWidget*>(parent()), "", "Протокол не создан!");
    //        return;
    //    }
    if (!m_protocolVisible[num] && !m_serNum[num].isEmpty()) {
        m_protocolVisible[num] = true;
        ProtoDialog* Dialog = new ProtoDialog(reinterpret_cast<QWidget*>(parent()), m_serNum[num]);
        Dialog->LoadFile(m_paths[num]);
        connect(Dialog, &QDialog::destroyed, [this, num] { m_protocolVisible[num] = false; });
    }
}

bool TestModel::dontSkip(int num) { return m_vChecked->size() ? m_vChecked->at(num) : false; }

void TestModel::onChecked(int index, int orientation)
{
    if (orientation == Qt::Horizontal)
        emit dataChanged(createIndex(0, index), createIndex(rowCount() - 1, index), { Qt::DisplayRole });
    else
        emit dataChanged(createIndex(index, 0), createIndex(index, columnCount() - 1), { Qt::DisplayRole });
}
