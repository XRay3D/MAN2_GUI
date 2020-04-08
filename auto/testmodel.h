#ifndef MESUREMODEL_H
#define MESUREMODEL_H

#include "hw/man2.h"
#include <QAbstractTableModel>

typedef struct Result_t {
    enum Val{
        Undefined,
        True,
        False
    };
    double test1 = 0.0;
    Val test2 = Undefined;
    double test3 = 0.0;
    double test4 = 0.0;
    double test5 = 0.0;
    double test6 = 0.0;
    double test7 = 0.0;
    void reset()
    {
        test1 = 0.0;
        test2 = Undefined;
        test3 = 0.0;
        test4 = 0.0;
        test5 = 0.0;
        test6 = 0.0;
        test7 = 0.0;
    }
} Result_t;

class MesureModel : public QAbstractTableModel {
    static MesureModel* instance;

public:
    MesureModel(QObject* parent = Q_NULLPTR);
    ~MesureModel() override;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    static void reset();

    static void setCurrentTest(int val);

    static void setTest1(const QList<MeasuredValue>& list);
    static void setTest2(int ch, bool result);
    static void setTest3(const QList<MeasuredValue>& list);
    static void setTest4(const QList<MeasuredValue>& list);
    static void setTest5(const QList<MeasuredValue>& list);
    static void setTest6(int ch, double value);
    static void setTest7(const QList<MeasuredValue>& list);

    static void saveProtokol(const QString& serialNumber, int number);
    static void showProtocol(int num);

private:
    int m_currentTest = -1;
    Result_t m_data[8];
    QVector<QString> m_paths;
    QVector<QString> m_serNum;
};

#endif // MESUREMODEL_H
