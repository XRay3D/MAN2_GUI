#include "tester.h"

#include "autoform.h"
#include "devicemodel.h"
#include "hw/interface.h"
#include "settings.h"
#include "testmodel.h"

enum {
    SkipRms = 1,
    SkipRmsMsg = 1,
    DelayBeforeMeasure1 = 1000,
    DelayBeforeMeasure2 = 1000,
};

Tester::Tester(bool* doNotSkip, QObject* parent)
    : QThread(parent)
    , doNotSkip(doNotSkip)
{
}

Tester::~Tester()
{
    mi::man->disableAll();
    semaphore.acquire(semaphore.available());
}

void Tester::continueTests() { semaphore.release(1); }

void Tester::finishTests() { semaphore.release(2); }

void Tester::run()
{
    setTerminationEnabled();
    semaphore.acquire(semaphore.available());

    try {
        while (!mi::man->disableAll())
            waitAnswerManConnErr();

        while (!mi::man->switchCurrent(On))
            waitAnswerManConnErr();

        if (mi::osc->isConnected()) {
            const double rc = DeviceModel::scanSettings().RatedCurrent / 6;
            qDebug() << "RatedCurrent" << rc;
            /*  */ if (rc < 10) {
                mi::osc->wrRdData(":CH1:SCALe 5mv");
            } else if (rc < 100) {
                mi::osc->wrRdData(":CH1:SCALe 50mv");
            } else if (rc < 1000) {
                mi::osc->wrRdData(":CH1:SCALe 500mv");
            } else {
                mi::osc->wrRdData(":CH1:SCALe 5v");
            }
            mi::osc->wrRdData(":CH1:COUPling DC");
            mi::osc->wrRdData(":CH1:OFFSet -3");
        }

        if (TestModel::instance()->dontSkip(TestModel::Test4)) {
            setVoltage(SetInputVoltageUpper);
            test4(); // Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test7)) {
            setVoltage(SetInputVoltageNormal);
            test7(); // Тест К.З. - восстановление напряжения при подключенной нагрузке Номинальное (220) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test6)) {
            setVoltage(SetInputVoltageNormal);
            test6(); // Поиск точки срабатывания защиты Номинальное (220) Плавное нарастание
        }
        if (TestModel::instance()->dontSkip(TestModel::Test1)) {
            setVoltage(SetInputVoltageNormal);
            test1(); // Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test5)) {
            setVoltage(SetInputVoltageNormal);
            test5(); // Измерение выходного напряжения без нагрузки Номинальное (220) Нет
        }
        if (TestModel::instance()->dontSkip(TestModel::Test3)) {
            setVoltage(SetInputVoltageLower);
            test3(); // Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test2)) {
            setVoltage(SetInputVoltageLower);
            test2(); // Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        }

        if (mi::osc->isConnected())
            mi::osc->wrRdData(":CH1:SCALe 500mv");

        TestModel::instance()->setCurrentTest(TestModel::None);

        while (!mi::man->disableAll())
            waitAnswerManConnErr();
    } catch (int i) {
        Q_UNUSED(i)
        semaphore.acquire(semaphore.available());
        terminate();
    }
}

void Tester::waitAnswer(int question)
{
    emit showMessage(question);
    while (!semaphore.tryAcquire(1, 200)) {
        if (question != NoConnectionWithMan)
            mi::man->getMeasuredValue(list);
    }
    checkFinished();
}

void Tester::waitAnswerManConnErr()
{
    emit showMessage(NoConnectionWithMan);
    semaphore.acquire();
    checkFinished();
}

void Tester::checkFinished()
{
    if (!semaphore.available())
        return;
    semaphore.acquire(semaphore.available());
    mi::man->Write(mi::man->createParcel(Ping));
    mi::man->disableAll();
    throw int(1);
}

void Tester::setVoltage(int voltage)
{
    //    if constexpr (SkipRmsMsg)
    //        return;

    static int lastVoltage = -1;
    switch (voltage) {
    case SetInputVoltageUpper:
        minAcVoltage = DeviceModel::scanSettings().Voltageerrortest5U1 - (DeviceModel::scanSettings().Voltageerrortest5U2 + 2);
        maxAcVoltage = DeviceModel::scanSettings().Voltageerrortest5U1 + (DeviceModel::scanSettings().Voltageerrortest5U2 + 2);
        break;
    case SetInputVoltageLower:
        minAcVoltage = DeviceModel::scanSettings().Voltageerrortest3_4U1 - (DeviceModel::scanSettings().Voltageerrortest3_4U2 + 2);
        maxAcVoltage = DeviceModel::scanSettings().Voltageerrortest3_4U1 + (DeviceModel::scanSettings().Voltageerrortest3_4U2 + 2);
        break;
    case SetInputVoltageNormal:
        minAcVoltage = 220 - 4.4;
        maxAcVoltage = 220 + 4.4;
        break;
    default:
        break;
    }

    if (lastVoltage == voltage)
        return;
    lastVoltage = voltage;

    waitAnswer(voltage);
}

void Tester::test1()
{
    qDebug() << "Test1";

    TestModel::instance()->setCurrentTest(TestModel::Test1);

    waitAndMeasure(DelayBeforeMeasure1);

    if (mi::osc->isConnected()) {
        configCh2();
        list.clear();
        for (int i = 1; i < 9; ++i) {
            if (!doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);

            waitAndMeasure(DelayBeforeMeasure2);

            list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest1(list);
            checkFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(list))
                waitAnswerManConnErr();
            TestModel::instance()->setTest1(list);
            checkFinished();
        } while (list.size() < 9);
    }
    emit updateProgresBar();
}

void Tester::test2()
{
    qDebug() << "Test2";
    TestModel::instance()->setCurrentTest(TestModel::Test2);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        waitAnswerManConnErr();

    if (mi::osc->isConnected()) {
        list.clear();
        mi::osc->wrRdData(":CH2:COUPling AC");
        mi::osc->wrRdData(":CH2:SCALe 100mv");
        mi::osc->wrRdData(":CH2:OFFSet 0");

        mi::osc->wrRdData(":TRIGger:SINGle:EDGE:SOURce CH2");
        mi::osc->wrRdData(":TRIGger:SINGle:SLOPe:LLevel 0v");

        mi::osc->wrRdData(":HORIzontal:SCALe 1.0ms");

        waitAndMeasure(DelayBeforeMeasure1);

        for (int i = 1; i < 9; ++i) {
            if (!doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);

            waitAndMeasure(DelayBeforeMeasure2);
            TestModel::instance()->setTest2(i - 1, DeviceModel::scanSettings().VisualControl > abs(mi::osc->AVERage(2)) * 1000.0);
            checkFinished();
            emit updateProgresBar();
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            if (doNotSkip[i]) {
                while (!mi::man->oscilloscope(0))
                    waitAnswerManConnErr();
                while (!mi::man->oscilloscope(i + 1))
                    waitAnswerManConnErr();
                waitAnswer(i + PulsationsOnTheChannel_1);
            }
            emit updateProgresBar();
        }
        while (!mi::man->oscilloscope(0))
            waitAnswerManConnErr();
    }
    emit updateProgresBar();
}

void Tester::test3()
{
    qDebug() << "Test3";
    TestModel::instance()->setCurrentTest(TestModel::Test3);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        waitAnswerManConnErr();

    waitAndMeasure(DelayBeforeMeasure1);

    if (mi::osc->isConnected()) {
        list.clear();
        configCh2();
        for (int i = 1; i < 9; ++i) {
            if (!doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);

            waitAndMeasure(DelayBeforeMeasure2);

            list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest3(list);
            checkFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(list))
                waitAnswerManConnErr();
            TestModel::instance()->setTest3(list);
            checkFinished();
        } while (list.size() < 9);
    }

    emit updateProgresBar();
}

void Tester::test4()
{
    qDebug() << "Test4";
    TestModel::instance()->setCurrentTest(TestModel::Test4);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        waitAnswerManConnErr();

    waitAndMeasure(DelayBeforeMeasure1);

    if (mi::osc->isConnected()) {
        list.clear();
        configCh2();
        for (int i = 1; i < 9; ++i) {
            if (!doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);

            waitAndMeasure(DelayBeforeMeasure2);

            list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest4(list);
            checkFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(list))
                waitAnswerManConnErr();
            TestModel::instance()->setTest4(list);
            checkFinished();
        } while (list.size() < 9);
    }

    emit updateProgresBar();
}

void Tester::test5()
{
    qDebug() << "Test5";
    TestModel::instance()->setCurrentTest(TestModel::Test5);

    while (!mi::man->setCurrent(0))
        waitAnswerManConnErr();

    waitAndMeasure(DelayBeforeMeasure1);

    if (mi::osc->isConnected()) {
        list.clear();
        configCh2();
        for (int i = 1; i < 9; ++i) {
            if (!doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);

            waitAndMeasure(DelayBeforeMeasure2);

            list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest5(list);
            checkFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(list))
                waitAnswerManConnErr();
            TestModel::instance()->setTest5(list);
            checkFinished();
        } while (list.size() < 9);
    }
    emit updateProgresBar();
}

void Tester::test6()
{
    qDebug() << "Test6";
    TestModel::instance()->setCurrentTest(TestModel::Test6);
    MeasuredValue value;

    while (!mi::man->setCurrent(0))
        waitAnswerManConnErr();

    {
        for (uint8_t ch = 1; ch <= 8; ++ch) {
            if (!doNotSkip[ch - 1]) {
                emit updateProgresBar();
                continue;
            }

            while (!mi::man->setCurrent(DeviceModel::scanSettings().RestrictionsTest7Min, ch))
                waitAnswerManConnErr();

            mi::man->oscilloscope(ch);

            if (mi::osc->isConnected()) {
                configCh2();
            }

            waitAndMeasure(DelayBeforeMeasure2);
            //            double step = DeviceModel::scanSettings().RestrictionsTest7Max - DeviceModel::scanSettings().RestrictionsTest7Min;
            // Всё в амперах

            while (!mi::man->tripCurrentTest(
                { static_cast<float>(DeviceModel::scanSettings().RestrictionsTest7Min * 0.8),
                    static_cast<float>(DeviceModel::scanSettings().RestrictionsTest7Max * 2),
                    static_cast<float>(DeviceModel::scanSettings().RestrictionsTest7Max - DeviceModel::scanSettings().RestrictionsTest7Min) / 100.f }, // mA
                ch))
                waitAnswerManConnErr();
            TestModel::instance()->setTest6(ch, mi::man->valueMap()[ch].valCh3);
        }
    }
    //if constexpr (!SkipRmsMsg)
    mi::man->setCurrent(0.0);
    waitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Tester::test7()
{
    qDebug() << "Test7";
    TestModel::instance()->setCurrentTest(TestModel::Test7);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        waitAnswerManConnErr();

    waitAndMeasure(DelayBeforeMeasure1);

    list.clear();

    if (mi::osc->isConnected()) {
        configCh2();
        for (uint8_t ch = 1; ch < 9; ++ch) {
            if (!doNotSkip[ch - 1])
                continue;

            mi::man->oscilloscope(ch);
            mi::osc->wrRdData(":HORIzontal:SCALe 0.5s");

            waitAndMeasure(DelayBeforeMeasure2);

            while (!mi::man->shortCircuitTest(ScShunt, ch)) //вкл
                waitAnswerManConnErr();
            //list[ch].valCh1 = mi::osc->MIN(2);
            //TestModel::instance()->setTest7(list);
            TestModel::instance()->setTest7({ { ch, mi::man->valueMap()[ch] } });
            mi::osc->wrRdData(":HORIzontal:SCALe 100us");
        }
    } else {
        for (uint8_t ch = 1; ch < 9; ++ch) {
            if (!doNotSkip[ch - 1])
                continue;
            while (!mi::man->shortCircuitTest(ScShunt, ch)) //вкл
                waitAnswerManConnErr();
            while (!mi::man->getMeasuredValue(list[ch], ch, RawData)) //измерение
                waitAnswerManConnErr();
            TestModel::instance()->setTest7(mi::man->valueMap());
        }
    }
    mi::man->setCurrent(0.0);
    checkFinished();
    //if constexpr (!SkipRmsMsg)
    waitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Tester::configCh2(bool slow)
{
    mi::osc->wrRdData(":CH2:COUPling DC");
    mi::osc->wrRdData(":CH2:SCALe 5v");
    mi::osc->wrRdData(":CH2:OFFSet -3");
    if (slow)
        mi::osc->wrRdData(":HORIzontal:SCALe 1.0s");
    else
        mi::osc->wrRdData(":HORIzontal:SCALe 1.0us");
}

void Tester::waitAndMeasure(unsigned long time)
{
    QElapsedTimer timer;
    timer.start();
    QMap<int, MeasuredValue> list;

    while (timer.elapsed() < time) {
        msleep(50);
        checkFinished();
        mi::man->getMeasuredValue(list);
        if (SkipRms)
            continue;
        while ((acVoltage = mi::man->getRmsValue()) == 0.0)
            waitAnswerManConnErr();
    }

    mi::man->getMeasuredValue(list);

    while (minAcVoltage > acVoltage || acVoltage > maxAcVoltage) {
        if (SkipRms)
            break;
        if (acVoltage == 0.0)
            waitAnswerManConnErr();
        waitAnswer(CheckInputVoltage);
        acVoltage = mi::man->getRmsValue();
    }
}
