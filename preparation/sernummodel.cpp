#include "sernummodel.h"

#include <QMessageBox>

SerNumModel* SerNumModel::self = nullptr;

SerNumModel::SerNumModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_data(8)
{
    self = this;
}

SerNumModel::~SerNumModel()
{
    self = nullptr;
}

int SerNumModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 8;
}

int SerNumModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

QVariant SerNumModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return m_data[index.row()];
    default:
        return QVariant();
    }
    return QVariant();
}

bool SerNumModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (m_data.indexOf(value.toString()) != -1 && m_data.indexOf(value.toString()) != index.row()) {
            QMessageBox::warning(nullptr, "", "Такой серийник уже есть!");
            return false;
        } else {
            m_data[index.row()] = value.toString();
            return true;
        }
    default:
        return false;
    }
    return false;
}

QVariant SerNumModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return "Заводской №";
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

Qt::ItemFlags SerNumModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

//int SerNumModel::count() const
//{
//    return m_count;
//}

//void SerNumModel::setCount(int count)
//{
//    m_count = count;
//    dataChanged(createIndex(0, 0), createIndex(7, 0));
//}
