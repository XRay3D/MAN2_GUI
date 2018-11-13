#include "mesuremodel.h"

int id4 = qRegisterMetaType<QVector<int>>("QVector<int>");

MesureModel* MesureModel::self = nullptr;

MesureModel::MesureModel(QObject* parent)
    : QAbstractTableModel(parent)
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
    }
    return QVariant();
}

QVariant MesureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return QString("Канал %1").arg(section + 1);
        else
            return QString("Тест %1").arg(section + 1);
    }
    return QVariant();
}

Qt::ItemFlags MesureModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;
}

void MesureModel::setTest1(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i) {
        m_data[i].test1 = list[i].Value1;
        qDebug() << i << list[i].Value1;
    }
    dataChanged(createIndex(0, 0), createIndex(0, 7), { Qt::DisplayRole });
}

void MesureModel::setTest2(int ch, bool result)
{
    //for (int i = 0; i < m_list.size(); ++i) {
    m_data[ch].test2 = result;
    //qDebug() << i << m_list[i].Value1;
    //}
    dataChanged(createIndex(1, ch), createIndex(1, ch), { Qt::DisplayRole });
}

void MesureModel::setTest3(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i) {
        m_data[i].test3 = list[i].Value1;
        qDebug() << i << list[i].Value1;
    }
    dataChanged(createIndex(2, 0), createIndex(2, 7), { Qt::DisplayRole });
}

void MesureModel::setTest4(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i) {
        m_data[i].test4 = list[i].Value1;
        qDebug() << i << list[i].Value1;
    }
    dataChanged(createIndex(3, 0), createIndex(3, 7), { Qt::DisplayRole });
}

void MesureModel::setTest5(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i) {
        m_data[i].test5 = list[i].Value1;
        qDebug() << i << list[i].Value1;
    }
    dataChanged(createIndex(4, 0), createIndex(4, 7), { Qt::DisplayRole });
}

void MesureModel::setTest6(int ch, double value)
{
    m_data[ch].test6 = value;
    dataChanged(createIndex(5, ch), createIndex(5, ch), { Qt::DisplayRole });
}

void MesureModel::setTest7(const QList<MeasuredValue_t>& list)
{
    for (int i = 0; i < list.size(); ++i) {
        m_data[i].test7 = list[i].Value1;
        qDebug() << i << list[i].Value1;
    }
    dataChanged(createIndex(6, 0), createIndex(6, 7), { Qt::DisplayRole });
}
