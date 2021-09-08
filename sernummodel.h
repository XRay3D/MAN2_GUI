#ifndef SERNUMMODEL_H
#define SERNUMMODEL_H

#include <QAbstractTableModel>

class SerNumModel : public QAbstractTableModel {
    Q_OBJECT
    inline static SerNumModel* m_instance;

public:
    explicit SerNumModel(QObject* parent = nullptr);
    ~SerNumModel();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& = {}) const override;
    int columnCount(const QModelIndex& = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void clear();
    bool isEmpty();
    QString serNum(int index);

    int count() const;
    int serNumCount();
    void setCount(int count);

    static SerNumModel* instance();

private:
    QVector<QString> m_data;
    int m_count = 8;

    void readSerNum();
    void writeSerNum();
};

#endif // SERNUMMODEL_H
