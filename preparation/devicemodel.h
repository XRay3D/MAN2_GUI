#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include "preparation.h"
#include <QAbstractTableModel>

class DeviceModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit DeviceModel(QObject* parent = nullptr);
    ~DeviceModel();
    static DeviceModel* self;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QList<QString> cbxData() const;

    int index() const;
    void setIndex(int index);
    ScanSettings scanSettings() { return m_data[m_index]; }

private:
    QList<ScanSettings> m_data;
    QList<QString> m_cbxData;
    int m_index = 0;
};

#endif // DEVICEMODEL_H
