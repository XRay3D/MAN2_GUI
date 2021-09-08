#include "sernummodel.h"

#include "settings.h"
#include "testmodel.h"
#include <QMessageBox>

SerNumModel* SerNumModel::instance() { return m_instance; }

SerNumModel::SerNumModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_data(8)
{
    m_instance = this;
    readSerNum();
}

SerNumModel::~SerNumModel()
{
    writeSerNum();
    m_instance = nullptr;
}

int SerNumModel::rowCount(const QModelIndex&) const
{
    return m_count;
}

int SerNumModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant SerNumModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (!index.column()) {
            return m_data[index.row()];
        } else {
            return TestModel::instance()->path(index.row());
        }
    case Qt::EditRole:
        if (!index.column()) {
            return m_data[index.row()];
        } else {
            TestModel::instance()->showProtocol(index.row());
            return TestModel::instance()->path(index.row());
        }
    default:
        return {};
    }
    return {};
}

bool SerNumModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (!index.column()) {
            if (m_data.indexOf(value.toString()) != -1 && m_data.indexOf(value.toString()) != index.row() && !value.toString().isEmpty()) {
                QMessageBox::warning(nullptr, "", "Такой серийник уже есть!");
                return false;
            } else {
                m_data[index.row()] = value.toString();
                serNumCount();
                return false;
            }
        }
        return false;
    default:
        return false;
    }
    return false;
}

QVariant SerNumModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (!section)
                return "Заводской №";
            return "Протокол";
        } else {
            switch (section) {
            case 0:
                return "1-ый прибор";
            case 1:
                return "2-ой прибор";
            case 2:
                return "3-ий прибор";
            case 3:
                return "4-ый прибор";
            case 4:
                return "5-ый прибор";
            case 5:
                return "6-ой прибор";
            case 6:
                return "7-ой прибор";
            case 7:
                return "8-ой прибор";
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags SerNumModel::flags(const QModelIndex& index) const
{
    if (!index.column())
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | (TestModel::instance()->path(index.row()).isEmpty() ? Qt::NoItemFlags : Qt::ItemIsEditable);
}

void SerNumModel::clear()
{
    for (QString& var : m_data)
        var.clear();
    dataChanged(createIndex(0, 0), createIndex(7, 0));
}

bool SerNumModel::isEmpty()
{
    for (QString& var : m_data) {
        if (!var.isEmpty())
            return false;
    }
    return true;
}

QString SerNumModel::serNum(int index) { return m_data[index]; }

int SerNumModel::count() const { return m_count; }

int SerNumModel::serNumCount() /*const*/
{
    int i = !m_data[0].isEmpty()
        + !m_data[1].isEmpty()
        + !m_data[2].isEmpty()
        + !m_data[3].isEmpty()
        + !m_data[4].isEmpty()
        + !m_data[5].isEmpty()
        + !m_data[6].isEmpty()
        + !m_data[7].isEmpty();

    //    std::sort(m_data.begin(), m_data.end());
    //    std::rotate(m_data.begin(), m_data.end() - i, m_data.end());
    dataChanged(createIndex(0, 0), createIndex(7, 0), { Qt::DisplayRole });
    return i;
}

void SerNumModel::setCount(int count)
{
    if (m_count < count) {
        beginInsertRows(QModelIndex(), m_count, count - 1);
        m_count = count;
        endInsertRows();
    } else if (m_count > count) {
        beginRemoveRows(QModelIndex(), count, m_count - 1);
        m_count = count;
        for (int i = m_count; i < 8; ++i)
            m_data[i].clear();
        endRemoveRows();
    }
}

void SerNumModel::readSerNum()
{
    MySettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("InputParameters");
    for (int i = 0; i < 8; ++i) {
        m_data[i] = settings.value("leSerialNumberDevice_" + QString::number(i), "").toString();
    }
    settings.endGroup();
}

void SerNumModel::writeSerNum()
{
    MySettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("InputParameters");
    for (int i = 0; i < 8; ++i) {
        settings.setValue("leSerialNumberDevice_" + QString::number(i), m_data[i]);
    }
    settings.endGroup();
}
