#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include "preparation.h"
#include <QAbstractTableModel>

class DeviceModel : public QAbstractTableModel {
    Q_OBJECT
    static DeviceModel* instance;

public:
    explicit DeviceModel(QObject* parent = nullptr);
    ~DeviceModel() override;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    static QList<QString> cbxData();

    static int index();
    static void setIndex(int index);
    static ScanSettings scanSettings();

private:
    QList<ScanSettings> m_data;
    QList<QString> m_cbxData;
    int m_index = 0;
};

#endif // DEVICEMODEL_H
