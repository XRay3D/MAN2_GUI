#include "manmodel.h"

ManModel::ManModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_u{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
    , m_i{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
    , m_acU(0.0)
{
}

int ManModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

int ManModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 8;
}

QVariant ManModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (index.row()) {
        case 0: // u
            return QString("%1В").arg(m_u[index.column()], 0, 'f', 2);
        case 1: // i
            return QString("%1мА").arg(m_i[index.column()], 0, 'f', 2);
        case 2: // ac u
            return QString("~%1B").arg(m_acU, 0, 'f', 2);
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::FontRole:
        if (index.row() == 2) {
            QFont f;
            f.setPointSize(40);
            return f;
        }
        return {};
    default: {
    }
    }
    return {};
}

Qt::ItemFlags ManModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;
}

void ManModel::setRms(double val)
{
    m_acU = val;
    dataChanged(createIndex(2, 0), createIndex(2, 7), { Qt::DisplayRole });
}

void ManModel::setMeasuredValueSignal(const QMap<int, MeasuredValue_t>& data)
{
    QMapIterator<int, MeasuredValue_t> iterator(data);
    while (iterator.hasNext()) {
        iterator.next();
        m_u[iterator.key() - 1] = iterator.value().Value1;
        m_i[iterator.key() - 1] = iterator.value().Value2;
    }
    dataChanged(createIndex(0, 0), createIndex(1, 7), { Qt::DisplayRole });
}

QVariant ManModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return QString("Канал №%1").arg(section + 1);
        else
            switch (section) {
            case 0:
                return "Выходное\nнапряжение";
            case 1:
                return "Ток\nнагрузки";
            case 2:
                return "Сеть";
            }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        QVariant();
    }
    return QVariant();
}
