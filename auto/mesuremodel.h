#ifndef MESUREMODEL_H
#define MESUREMODEL_H

#include "hw/man2.h"
#include <QAbstractTableModel>

typedef struct Result_t {
    double test1 = 0.0;
    bool test2 = false;
    double test3 = 0.0;
    double test4 = 0.0;
    double test5 = 0.0;
    double test6 = 0.0;
    double test7 = 0.0;
    void reset()
    {
        test1 = 0.0;
        test2 = false;
        test3 = 0.0;
        test4 = 0.0;
        test5 = 0.0;
        test6 = 0.0;
        test7 = 0.0;
    }
} Result_t;

class MesureModel : public QAbstractTableModel {
public:
    MesureModel(QObject* parent = Q_NULLPTR);
    ~MesureModel();

    // QAbstractItemModel interface
public:
    static MesureModel* self;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void reset();

    void setTest1(const QList<MeasuredValue_t>& list);
    void setTest2(int ch, bool result);
    void setTest3(const QList<MeasuredValue_t>& list);
    void setTest4(const QList<MeasuredValue_t>& list);
    void setTest5(const QList<MeasuredValue_t>& list);
    void setTest6(int ch, double value);
    void setTest7(const QList<MeasuredValue_t>& list);

    void saveProtokol(const QString& serialNumber, int number);
    void showProtocol(int num);
private:
    Result_t m_data[8];
    QVector<QString> m_paths;
    QVector<QString> m_serNum;
};

#endif // MESUREMODEL_H
