#ifndef MESUREMODEL_H
#define MESUREMODEL_H

#include "hw/man2.h"
#include <QAbstractTableModel>

struct Result_t {
    enum Val {
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

    friend inline QDataStream& operator>>(QDataStream& s, Result_t& set)
    {
        s.readRawData(reinterpret_cast<char*>(&set), sizeof(Result_t));
        return s;
    }

    friend inline QDataStream& operator<<(QDataStream& s, const Result_t& set)
    {
        s.writeRawData(reinterpret_cast<const char*>(&set), sizeof(Result_t));
        return s;
    }
};

class TestModel : public QAbstractTableModel {
    static TestModel* instance;

public:
    TestModel(QObject* parent = nullptr,
        const QVector<bool>* hChecked = nullptr,
        const QVector<bool>* vChecked = nullptr);
    ~TestModel() override;

    enum {
        Test1,
        Test2,
        Test3,
        Test4,
        Test5,
        Test6,
        Test7,
        TestCount,
        Channels = 8
    };

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    static void setCurrentTest(int val);

    static void setTest1(const QVector<MeasuredValue>& data);
    static void setTest2(int ch, bool result);
    static void setTest3(const QVector<MeasuredValue>& data);
    static void setTest4(const QVector<MeasuredValue>& data);
    static void setTest5(const QVector<MeasuredValue>& data);
    static void setTest6(int ch, double value);
    static void setTest7(const QVector<MeasuredValue>& data);

    static void saveProtokol(const QString& serialNumber, int number);
    static void showProtocol(int num);
    static bool dontSkip(int num);

    void onChecked(int index, int orientation);

private:
    void reset();

    const QVector<bool>* m_hChecked;
    const QVector<bool>* m_vChecked;
    int m_row = 0;
    int m_column = 0;

    int m_currentTest = -1;
    Result_t m_data[8];
    QVector<QString> m_paths;
    QVector<QString> m_serNum;
};

#endif // MESUREMODEL_H
