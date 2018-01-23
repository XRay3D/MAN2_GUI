#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "automaticmeasurements.h"

class Worker : public QThread {
    Q_OBJECT
public:
    Worker(bool* doNotSkip, Result_t* result, const ScanSettings* scanSettings, QObject* parent = Q_NULLPTR);
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
    bool IsBreak();
    bool WaitUserSolution(int question);
    bool CheckFinished();
    bool SetVoltage(int voltage);
    bool Test1();
    bool Test2();
    bool Test3();
    bool Test4();
    bool Test5();
    bool Test6();
    bool Test7();

    //    bool GetVoltage(double);
    //    bool GetMeasuredValue(m_list);
    //    bool GetMeasuredValue(MeasuredValue_t& value, int i);
    //    bool Oscilloscope(int val);
    //    bool SetCurrent(double current);
    //    bool SetCurrent(double current, int i);
    //    bool ShortCircuitTest(int val);
    //    bool SwitchCurrent(int val);

    bool Msleep(unsigned long time);

    QList<MeasuredValue_t> m_list;
    double m_inVoltage;
    double m_minInVoltage;
    double m_maxInVoltage;

    const bool* m_doNotSkip;
    Result_t* m_result;
    const ScanSettings* m_pScanSettings;
    QSemaphore m_semaphore;
    int m_counter;
};

#endif // WORKER_H
