#ifndef MEASUREMODEL_H
#define MEASUREMODEL_H

#include <QAbstractTableModel>

class MeasureModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit MeasureModel(QObject* parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    double m_u[8];
    double m_i[8];
    double m_acU;
};

#endif // MEASUREMODEL_H
