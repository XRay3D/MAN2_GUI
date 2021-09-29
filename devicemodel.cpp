#include "devicemodel.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>

DeviceModel* DeviceModel::instance() { return m_instance; }

DeviceModel::DeviceModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    qDebug() << QFileInfo::exists("modify.txt");
    QFile file("modify.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "", "Не найден файл \"modify.txt\" с параметрами блоков питания!");
        return;
    }
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        m_data.append(ScanSettings(in.readLine().split(';')));
        m_cbxData.append(m_data.last().Type + " (" + m_data.last().Cipher + ")");
    }
    m_instance = this;
}

DeviceModel::~DeviceModel() { m_instance = nullptr; }

int DeviceModel::rowCount(const QModelIndex& /*parent*/) const { return 14 /*15*/; }

int DeviceModel::columnCount(const QModelIndex& /*parent*/) const { return 2; }

QVariant DeviceModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (m_data.size()) {
            ScanSettings s(m_data.value(m_index));
            if (!index.column()) {
                switch (index.row()) {
                case 0:
                    return s.Type;
                case 1:
                    return s.Cipher;
                case 2:
                    return s.RatedVoltage;
                case 3:
                    return s.NumberOfChannels;
                case 4:
                    return s.RatedCurrent;
                case 5:
                    return s.RestrictionTest2;
                case 6:
                    return s.VisualControl;
                case 7:
                    return s.LimitationsTest4_5;
                case 8:
                    return s.LimitTest6;
                case 9:
                    return s.RestrictionsTest7Min;
                case 10:
                    return s.Voltageerrortest5U1;
                case 11:
                    return s.Voltageerrortest3_4U1;
                case 12:
                    return s.VoltageErrorTest7;
                case 13:
                    return s.ParameterDLTest7;
                default:
                    return QVariant();
                }
            } else {
                switch (index.row()) {
                case 9:
                    return s.RestrictionsTest7Max;
                case 10:
                    return s.Voltageerrortest5U2;
                case 11:
                    return s.Voltageerrortest3_4U2;
                default:
                    return QVariant();
                }
            }
        }
        return QVariant();
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::ToolTipRole:
        if (!index.column()) {
            switch (index.row()) {
            case 0:
                return "Type";
            case 1:
                return "Cipher";
            case 2:
                return "RatedVoltage";
            case 3:
                return "NumberOfChannels";
            case 4:
                return "RatedCurrent";
            case 5:
                return "RestrictionTest2";
            case 6:
                return "VisualControl";
            case 7:
                return "LimitationsTest4_5";
            case 8:
                return "LimitTest6";
            case 9:
                return "RestrictionsTest7Min";
            case 10:
                return "Voltageerrortest5U1";
            case 11:
                return "Voltageerrortest3_4U1";
            case 12:
                return "VoltageErrorTest7";
            case 13:
                return "ParameterDLTest7";
            default:
                return QVariant();
            }
        } else {
            switch (index.row()) {
            case 9:
                return "RestrictionsTest7Max";
            case 10:
                return "Voltageerrortest5U2";
            case 11:
                return "Voltageerrortest3_4U2";
            default:
                return QVariant();
            }
        }

    default:
        return QVariant();
    }
    return QVariant();
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QStringList horizontal({ "", "" }); //({ "value", "delta" });
    static const QStringList vertical({
        "Тип",
        "Шифр",
        "Номинальное напряжение",
        "Количество каналов",
        "Номинальный ток",
        "Ограничение, тест №2",
        "Условие визуального контроля№3",
        "Ограничения для теста №4 и №5",
        "Ограничение для теста №6",
        "Ограничения для теста №7 (мин./макс.)",
        "Ограничения для теста №7 (макс.)",
        "Напряжение и ошибка для тестов №3 и №4",
        "Напряжение и ошибка для теста №5",
        "Ошибка напряжения для теста №7",
        "Параметр dL для теста №7",
    });
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return horizontal[section];
        } else {
            return vertical[section];
        }
    }
    return QVariant();
}

Qt::ItemFlags DeviceModel::flags(const QModelIndex& /*index*/) const { return Qt::ItemIsEnabled; }

QList<QString> DeviceModel::cbxData()
{
    if (!m_instance)
        return {};
    return m_instance->m_cbxData;
}

int DeviceModel::index()
{
    if (!m_instance)
        return -1;
    return m_instance->m_index;
}

void DeviceModel::setIndex(int index)
{
    if (!m_instance)
        return;
    m_instance->m_index = index;
    m_instance->dataChanged(m_instance->createIndex(0, 0), m_instance->createIndex(14, 1), { Qt::DisplayRole });
}

ScanSettings DeviceModel::scanSettings()
{ //    if (!instance)
    //        return {};
    return m_instance->m_data.value(m_instance->m_index);
}
