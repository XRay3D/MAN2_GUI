#include "measuremodel.h"

MeasureModel::MeasureModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_u{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
    , m_i{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
    , m_acU(0.0)
{
}

int MeasureModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

int MeasureModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 8;
}

QVariant MeasureModel::data(const QModelIndex& index, int role) const
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
    default:
        QVariant();
    }
    return QVariant();
}

Qt::ItemFlags MeasureModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;
}

void MeasureModel::setRms(double val)
{
    m_acU = val;
    dataChanged(createIndex(2, 0), createIndex(2, 7), { Qt::DisplayRole });
}

void MeasureModel::setMeasuredValueSignal(const QMap<int, MeasuredValue_t>& data)
{
    QMapIterator<int, MeasuredValue_t> iterator(data);
    while (iterator.hasNext()) {
        iterator.next();
        m_u[iterator.key() - 1] = iterator.value().Value1;
        m_i[iterator.key() - 1] = iterator.value().Value2;
    }
    dataChanged(createIndex(0, 0), createIndex(1, 7), { Qt::DisplayRole });
}

QVariant MeasureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return QString::number(section + 1);
        else
            switch (section) {
            case 0:
                return "Входное\nнапряжение";
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
