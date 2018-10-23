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
            return QString("~%1B").arg(m_acU, 0, 'f', 0);
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
