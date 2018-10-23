#include "devicemodel.h"

DeviceModel::DeviceModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int DeviceModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 15;
}

int DeviceModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

QVariant DeviceModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QString("%1 %2").arg(index.row()).arg(index.column());
    default:
        return QVariant();
    }
    return QVariant();
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QStringList horizontal({ "value", "delta" });
    static const QStringList vertical({ "Тип",
        "Шифр",
        "Номинальное напряжение",
        "Количество каналов",
        "Номинальный ток",
        "Ограничение, тест №2",
        "Условие визуального контроля№3",
        "Ограничения для теста №4 и №5",
        "Ограничение для теста №6",
        "Ограничения для теста №7 (мин.)",
        "Ограничения для теста №7 (макс.)",
        "Напряжение и ошибка для тестов №3 и №4",
        "Напряжение и ошибка для теста №5",
        "Ошибка напряжения для теста №7",
        "Параметр dL для теста №7" });
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return horizontal[section];
        } else {
            return vertical[section];
        }
    }
    return QVariant();
}

Qt::ItemFlags DeviceModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;
}
