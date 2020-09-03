#ifndef SERNUMMODEL_H
#define SERNUMMODEL_H

#include <QAbstractTableModel>

class SerNumModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit SerNumModel(QObject* parent = nullptr);
    ~SerNumModel();
    static SerNumModel* self;
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void clear();
    bool isEmpty();
    QString serNum(int index) const { return m_data[index]; }
    //    int count() const;
    //    void setCount(int count);

    int count() const;
    int serNumCount();
    void setCount(int count);

private:
    QVector<QString> m_data;
    int m_count = 8;

    void readSerNum();
    void writeSerNum();
};

#endif // SERNUMMODEL_H
