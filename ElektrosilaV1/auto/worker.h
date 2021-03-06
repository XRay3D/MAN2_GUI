#ifndef WORKER_H
#define WORKER_H

#include "automeasure.h"
#include <QThread>

class Worker : public QThread {
    Q_OBJECT
public:
    Worker(bool* doNotSkip, QObject* parent = nullptr);
    ~Worker() override;
    void Continue();
    void FinishMeasurements();
    void run() override;

signals:
    void showMessage(int num);
    void updateProgresBar();

private:
    void WaitAnswer(int question);
    void WaitAnswerManConnErr();
    void CheckFinished();
    void SetVoltage(int voltage);
    void Test1();
    void Test2();
    void Test3();
    void Test4();
    void Test5();
    void Test6();
    void Test7();

    void Msleep(unsigned long time);

    QVector<MeasuredValue> m_list;
    double m_inVoltage;
    double m_minInVoltage;
    double m_maxInVoltage;

    const bool* m_doNotSkip;
    QSemaphore m_semaphore;
    int m_counter;
};

#endif // WORKER_H
