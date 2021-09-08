#ifndef MEASUREMODEL_H
#define MEASUREMODEL_H

#include <QAbstractTableModel>

#include <hw/man2.h>

class ManModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ManModel(QObject* parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setMeasuredValues(const QMap<int, MeasuredValue>&);

private:
    double m_u[ManCount] = { 0.0 };
    double m_i[ManCount] = { 0.0 };
    double m_acU = 0.0;
};

#endif // MEASUREMODEL_H
