#ifndef WORKER_H
#define WORKER_H

#include "hw/man2.h"
#include <QThread>

class Tester : public QThread {
    Q_OBJECT
public:
    Tester(bool* doNotSkip, QObject* parent = nullptr);
    ~Tester() override;
    void continueTests();
    void finishTests();
    void run() override;

signals:
    void showMessage(int num);
    void updateProgresBar();

private:
    void waitAnswer(int question);
    void waitAnswerManConnErr();
    void checkFinished();
    void setVoltage(int voltage);
    void test1();
    void test2();
    void test3();
    void test4();
    void test5();
    void test6();
    void test7();

    void configCh2(bool slow = false);

    void waitAndMeasure(unsigned long time);

    QMap<int, MeasuredValue> list;
    double acVoltage;
    double minAcVoltage;
    double maxAcVoltage;

    const bool* const doNotSkip;
    QSemaphore semaphore;
};

#endif // WORKER_H
