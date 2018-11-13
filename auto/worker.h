#ifndef WORKER_H
#define WORKER_H

#include "automaticmeasurements.h"
#include <QThread>

class Worker : public QThread {
    Q_OBJECT
public:
    Worker(bool* doNotSkip, /*Result_t* result,*/ QObject* parent = Q_NULLPTR);
    ~Worker();
    void SetMan();
    void Continue();
    void FinishMeasurements();
    void Quit();
    void Setup();
    void run() override;

signals:
    void ShowMessage(int num);
    void UpdateProgresBar();

private:
    void WaitSolutionOrEnd(int question);
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

    QList<MeasuredValue_t> m_list;
    double m_inVoltage;
    double m_minInVoltage;
    double m_maxInVoltage;

    const bool* m_doNotSkip;
    QSemaphore m_semaphore;
    int m_counter;
};

#endif // WORKER_H
