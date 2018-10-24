#include "devicemodel.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

DeviceModel* DeviceModel::self = nullptr;

DeviceModel::DeviceModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    self = this;
    QFile file("modify.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, "", "Не найден файл \"modify.txt\" с параметрами блоков питания!");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        m_data.append(ScanSettings(in.readLine().split(';')));
        m_cbxData.append(m_data.last().Type + " (" + m_data.last().Cipher + ")");
    }
}

DeviceModel::~DeviceModel()
{
    self = nullptr;
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
        if (m_data.size()) {
            if (!index.column())
                switch (index.row()) {
                case 0:
                    return m_data[m_index].Type;
                case 1:
                    return m_data[m_index].Cipher;
                case 2:
                    return m_data[m_index].RatedVoltage;
                case 3:
                    return m_data[m_index].NumberOfChannels;
                case 4:
                    return m_data[m_index].RatedCurrent;
                case 5:
                    return m_data[m_index].RestrictionTest2;
                case 6:
                    return m_data[m_index].VisualControl;
                case 7:
                    return m_data[m_index].LimitationsTest4_5;
                case 8:
                    return m_data[m_index].LimitTest6;
                case 9:
                    return m_data[m_index].RestrictionsTest7Min;
                case 10:
                    return m_data[m_index].RestrictionsTest7Max;
                case 11:
                    return m_data[m_index].Voltageerrortest3_4U1; // Voltageerrortest5U1
                case 12:
                    return m_data[m_index].Voltageerrortest3_4U2; //       Voltageerrortest5U2
                case 13:
                    return m_data[m_index].VoltageErrorTest7;
                case 14:
                    return m_data[m_index].ParameterDLTest7;

                default:
                    return QVariant();
                }
            else
                switch (index.row()) {
                case 11:
                    return m_data[m_index].Voltageerrortest5U1;
                case 12:
                    return m_data[m_index].Voltageerrortest5U2;
                default:
                    return QVariant();
                }
        }
        return QVariant();
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        return QVariant();
    }
    return QVariant();
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QStringList horizontal({ "", "" }); //({ "value", "delta" });
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

QList<QString> DeviceModel::cbxData() const
{
    return m_cbxData;
}

int DeviceModel::index() const
{
    return m_index;
}

void DeviceModel::setIndex(int index)
{
    m_index = index;
    dataChanged(createIndex(0, 0), createIndex(14, 1), { Qt::DisplayRole });
}
