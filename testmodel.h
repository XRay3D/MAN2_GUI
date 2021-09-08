#ifndef MESUREMODEL_H
#define MESUREMODEL_H

#include "hw/man2.h"
#include <QAbstractTableModel>

struct TestData {
    enum Val {
        Undefined,
        True,
        False
    };
    double test1 = 0.0;
    double test3 = 0.0;
    double test4 = 0.0;
    double test5 = 0.0;
    double test6 = 0.0;
    double test7 = 0.0;
    Val test2 = Undefined;

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
    void reset(const QVector<bool>* flags)
    {
        if (!flags || (flags && flags->isEmpty()))
            return;
        if (flags->at(0))
            test1 = 0.0;
        if (flags->at(1))
            test2 = Undefined;
        if (flags->at(2))
            test3 = 0.0;
        if (flags->at(3))
            test4 = 0.0;
        if (flags->at(4))
            test5 = 0.0;
        if (flags->at(5))
            test6 = 0.0;
        if (flags->at(6))
            test7 = 0.0;
    }

    friend inline QDataStream& operator>>(QDataStream& s, TestData& set)
    {
        s.readRawData(reinterpret_cast<char*>(&set), sizeof(TestData));
        return s;
    }

    friend inline QDataStream& operator<<(QDataStream& s, const TestData& set)
    {
        s.writeRawData(reinterpret_cast<const char*>(&set), sizeof(TestData));
        return s;
    }
};

class TestModel : public QAbstractTableModel {
    inline static TestModel* m_instance;

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
        None = -1
    };

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setCurrentTest(int val);

    void setTest1(const QMap<int, MeasuredValue>& data);
    void setTest2(int ch, bool result);
    void setTest3(const QMap<int, MeasuredValue>& data);
    void setTest4(const QMap<int, MeasuredValue>& data);
    void setTest5(const QMap<int, MeasuredValue>& data);
    void setTest6(int ch, double value);
    void setTest7(const QMap<int, MeasuredValue>& data);

    void saveProtokol(const QString& serialNumber, int number);
    void showProtocol(int num);
    bool dontSkip(int num);

    void onChecked(int index, int orientation);
    QString path(int index)
    {
        assert(index < 8);
        return m_paths[index];
    }
    static TestModel* instance();
    void reset();

private:
    const QVector<bool>* m_hChecked;
    const QVector<bool>* m_vChecked;

    int m_currentTest = -1;
    QVector<TestData> m_data;
    QVector<QString> m_paths;
    QVector<QString> m_serNum;
    bool m_protocolVisible[ManCount] = { 0 };
};

#endif // MESUREMODEL_H
