#include "tester.h"

#include "autoform.h"
#include "devicemodel.h"
#include "hw/interface.h"
#include "testmodel.h"
#include <QSettings>

enum { SkipRms = 1 };
Tester::Tester(bool* doNotSkip, QObject* parent)
    : QThread(parent)
    , m_doNotSkip(doNotSkip)
{
}

Tester::~Tester()
{
    mi::man->disableAll();
    m_semaphore.acquire(m_semaphore.available());
}

void Tester::Continue()
{
    m_semaphore.release(1);
}

void Tester::FinishMeasurements()
{
    mi::man->disableAll();
    m_semaphore.release(2);
}

void Tester::run()
{
    m_counter = 0;
    setTerminationEnabled();
    m_semaphore.acquire(m_semaphore.available());
    try {
        while (!mi::man->disableAll())
            WaitAnswerManConnErr();

        while (!mi::man->switchCurrent(On))
            WaitAnswerManConnErr();

        if (TestModel::instance()->dontSkip(TestModel::Test4)) {
            SetVoltage(SetInputVoltageUpper);
            Test4(); // Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test7)) {
            SetVoltage(SetInputVoltageNormal);
            Test7(); // Тест К.З. - восстановление напряжения при подключенной нагрузке Номинальное (220) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test6)) {
            SetVoltage(SetInputVoltageNormal);
            Test6(); // Поиск точки срабатывания защиты Номинальное (220) Плавное нарастание
        }
        if (TestModel::instance()->dontSkip(TestModel::Test1)) {
            SetVoltage(SetInputVoltageNormal);
            Test1(); // Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test5)) {
            SetVoltage(SetInputVoltageNormal);
            Test5(); // Измерение выходного напряжения без нагрузки Номинальное (220) Нет
        }
        if (TestModel::instance()->dontSkip(TestModel::Test3)) {
            SetVoltage(SetInputVoltageLower);
            Test3(); // Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        }
        if (TestModel::instance()->dontSkip(TestModel::Test2)) {
            SetVoltage(SetInputVoltageLower);
            Test2(); // Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        }
    } catch (int i) {
        Q_UNUSED(i)
        m_semaphore.acquire(m_semaphore.available());
        terminate();
    }
}

void Tester::WaitAnswer(int question)
{
    emit showMessage(question);
    while (!m_semaphore.tryAcquire(1, 100))
        if (question != NoConnectionWithMan)
            mi::man->getMeasuredValue(m_list);
    CheckFinished();
}

void Tester::WaitAnswerManConnErr()
{
    emit showMessage(NoConnectionWithMan);
    while (!m_semaphore.tryAcquire(1, 100))
        continue;
    CheckFinished();
}

void Tester::CheckFinished()
{
    if (m_semaphore.available())
        throw int(1);
}

void Tester::SetVoltage(int voltage)
{
    static int lastVoltage = -1;
    switch (voltage) {
    case SetInputVoltageUpper:
        m_minInVoltage = DeviceModel::scanSettings().Voltageerrortest5U1 - (DeviceModel::scanSettings().Voltageerrortest5U2 + 2);
        m_maxInVoltage = DeviceModel::scanSettings().Voltageerrortest5U1 + (DeviceModel::scanSettings().Voltageerrortest5U2 + 2);
        break;
    case SetInputVoltageLower:
        m_minInVoltage = DeviceModel::scanSettings().Voltageerrortest3_4U1 - (DeviceModel::scanSettings().Voltageerrortest3_4U2 + 2);
        m_maxInVoltage = DeviceModel::scanSettings().Voltageerrortest3_4U1 + (DeviceModel::scanSettings().Voltageerrortest3_4U2 + 2);
        break;
    case SetInputVoltageNormal:
        m_minInVoltage = 220 - 4.4;
        m_maxInVoltage = 220 + 4.4;
        break;
    default:
        break;
    }

    if (lastVoltage == voltage)
        return;
    lastVoltage = voltage;

    WaitAnswer(voltage);
}

void Tester::Test1()
{
    qDebug() << "Test1";
    TestModel::instance()->setCurrentTest(TestModel::Test1);
    Msleep(100); ////5000
    if (mi::osc->isConnected()) {
        m_list.clear();
        mi::osc->wrRdData(":CH2:COUPling DC");
        mi::osc->wrRdData(":CH2:SCALe 5v");
        mi::osc->wrRdData(":CH2:OFFSet -2");
        for (int i = 1; i < 9; ++i) {
            if (!m_doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);
            Msleep(500);
            m_list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest1(m_list);
            CheckFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(m_list))
                WaitAnswerManConnErr();
            TestModel::instance()->setTest1(m_list);
            CheckFinished();
        } while (m_list.size() < 9);
    }
    emit updateProgresBar();
}

void Tester::Test2()
{
    qDebug() << "Test2";
    TestModel::instance()->setCurrentTest(TestModel::Test2);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        WaitAnswerManConnErr();

    while (!mi::man->switchCurrent(1))
        WaitAnswerManConnErr();

    if (mi::osc->isConnected()) {
        m_list.clear();
        mi::osc->wrRdData(":CH2:COUPling AC");
        mi::osc->wrRdData(":CH2:SCALe 500mv");
        mi::osc->wrRdData(":CH2:OFFSet 0");
        for (int i = 1; i < 9; ++i) {
            if (!m_doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);
            Msleep(500);
            qDebug() << DeviceModel::scanSettings().VisualControl << mi::osc->PKPK(2);
            TestModel::instance()->setTest2(i - 1, DeviceModel::scanSettings().VisualControl > mi::osc->PKPK(2) * 1000.0);
            WaitAnswerManConnErr(); ///////
            CheckFinished();
            emit updateProgresBar();
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            if (m_doNotSkip[i]) {
                while (!mi::man->oscilloscope(0))
                    WaitAnswerManConnErr();
                while (!mi::man->oscilloscope(i + 1))
                    WaitAnswerManConnErr();
                WaitAnswer(i + PulsationsOnTheChannel_1);
            }
            emit updateProgresBar();
        }
        while (!mi::man->oscilloscope(0))
            WaitAnswerManConnErr();
    }
    emit updateProgresBar();
}

void Tester::Test3()
{
    qDebug() << "Test3";
    TestModel::instance()->setCurrentTest(TestModel::Test3);
    Msleep(5000);
    if (mi::osc->isConnected()) {
        m_list.clear();
        mi::osc->wrRdData(":CH2:COUPling DC");
        mi::osc->wrRdData(":CH2:SCALe 5v");
        mi::osc->wrRdData(":CH2:OFFSet -2");
        for (int i = 1; i < 9; ++i) {
            if (!m_doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);
            Msleep(500);
            m_list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest3(m_list);
            CheckFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(m_list))
                WaitAnswerManConnErr();
            TestModel::instance()->setTest3(m_list);
            CheckFinished();
        } while (m_list.size() < 9);
    }

    emit updateProgresBar();
}

void Tester::Test4()
{
    qDebug() << "Test4";
    TestModel::instance()->setCurrentTest(TestModel::Test4);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        WaitAnswerManConnErr();
    Msleep(5000);
    if (mi::osc->isConnected()) {
        m_list.clear();
        mi::osc->wrRdData(":CH2:COUPling DC");
        mi::osc->wrRdData(":CH2:SCALe 5v");
        mi::osc->wrRdData(":CH2:OFFSet -2");
        for (int i = 1; i < 9; ++i) {
            if (!m_doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);
            Msleep(500);
            m_list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest4(m_list);
            CheckFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(m_list))
                WaitAnswerManConnErr();
            TestModel::instance()->setTest4(m_list);
            CheckFinished();
        } while (m_list.size() < 9);
    }

    emit updateProgresBar();
}

void Tester::Test5()
{
    qDebug() << "Test5";
    TestModel::instance()->setCurrentTest(TestModel::Test5);

    while (!mi::man->setCurrent(0))
        WaitAnswerManConnErr();
    Msleep(5000);
    if (mi::osc->isConnected()) {
        m_list.clear();
        mi::osc->wrRdData(":CH2:COUPling DC");
        mi::osc->wrRdData(":CH2:SCALe 5v");
        mi::osc->wrRdData(":CH2:OFFSet -2");
        for (int i = 1; i < 9; ++i) {
            if (!m_doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);
            Msleep(500);
            m_list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest5(m_list);
            CheckFinished();
        }
    } else {
        do {
            while (!mi::man->getMeasuredValue(m_list))
                WaitAnswerManConnErr();
            TestModel::instance()->setTest5(m_list);
            CheckFinished();
        } while (m_list.size() < 9);
    }
    emit updateProgresBar();
}

void Tester::Test6()
{
    qDebug() << "Test6";
    TestModel::instance()->setCurrentTest(TestModel::Test6);
    MeasuredValue value;

    while (!mi::man->setCurrent(0))
        WaitAnswerManConnErr();

    /*    if (mi::osc->isConnected()) {
        m_list.clear();
        mi::osc->wrRdData(":CH2:COUPling DC");
        mi::osc->wrRdData(":CH2:SCALe 5v");
        mi::osc->wrRdData(":CH2:OFFSet -2");
        for (int i = 1; i < 9; ++i) {
            if (!m_doNotSkip[i - 1])
                continue;
            mi::man->oscilloscope(i);
            Msleep(500);
            m_list[i].valCh1 = mi::osc->AVERage(2);
            TestModel::instance()->setTest5(m_list);
            CheckFinished();
        }
    } else */
    {
        //        QList<double> peak;
        for (uint8_t i = 1; i <= 8; ++i) {
            if (!m_doNotSkip[i - 1]) {
                emit updateProgresBar();
                continue;
            }

            while (!mi::man->setCurrent(DeviceModel::scanSettings().RestrictionsTest7Min, i))
                WaitAnswerManConnErr();
            Msleep(5000);
            double step = DeviceModel::scanSettings().RestrictionsTest7Max - DeviceModel::scanSettings().RestrictionsTest7Min;
            if (step > 100)
                step = 0.002;
            else
                step = 0.001;
            while (!mi::man->startTest(
                DeviceModel::scanSettings().RestrictionsTest7Min / 1000.0f,
                DeviceModel::scanSettings().RestrictionsTest7Max / 1000.0f,
                step, i))
                WaitAnswerManConnErr();

            //            for (double current = DeviceModel::scanSettings().RestrictionsTest7Min;
            //                 current <= DeviceModel::scanSettings().RestrictionsTest7Max;
            //                 current += step) {
            //                while (!mi::man->setCurrent(current, i))
            //                    WaitAnswerManConnErr();
            //                msleep(50);
            //                while (!mi::man->getMeasuredValue(value, i, RawData))
            //                    WaitAnswerManConnErr();
            //                qDebug() << i << value.valCh1 << value.valCh1 << value.valCh3;
            //                if (std::min(value.valCh1, std::min(value.valCh2, value.valCh3)) < 1.0)
            //                    break;
            //                Msleep(100);
            //                peak.append(value.valCh3);
            //                CheckFinished();
            //                TestModel::instance()->setTest6(i - 1, value.valCh3);
            //            }
            //            while (!mi::man->setCurrent(0, i))
            //                WaitAnswerManConnErr();
            //            std::sort(peak.begin(), peak.end());
            TestModel::instance()->setTest6(i - 1, mi::man->valueMap()[i].valCh3);
            //            peak.clear();
        }
    }
    WaitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Tester::Test7()
{
    qDebug() << "Test7";
    TestModel::instance()->setCurrentTest(TestModel::Test7);

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        WaitAnswerManConnErr();

    Msleep(5000);

    m_list.clear();

    if (mi::osc->isConnected()) {
        mi::osc->wrRdData(":CH2:COUPling DC");
        mi::osc->wrRdData(":CH2:SCALe 5v");
        mi::osc->wrRdData(":CH2:OFFSet -2");
        for (uint8_t ch = 1; ch < 9; ++ch) {
            if (!m_doNotSkip[ch - 1])
                continue;
            mi::osc->wrRdData(":HORIzontal:SCALe 0.5s");
            mi::man->oscilloscope(ch);
            Msleep(500);
            while (!mi::man->shortCircuitTest(1, ch)) //вкл
                WaitAnswerManConnErr();
            while (!mi::man->shortCircuitTest(0, ch)) //выкл
                WaitAnswerManConnErr();
            msleep(50);
            m_list[ch].valCh1 = mi::osc->MIN(2);
            TestModel::instance()->setTest7(m_list);
            mi::osc->wrRdData(":HORIzontal:SCALe 100us");
        }
    } else {
        for (uint8_t ch = 1; ch < 9; ++ch) {
            if (!m_doNotSkip[ch - 1])
                continue;
            while (!mi::man->shortCircuitTest(1, ch)) //вкл
                WaitAnswerManConnErr();
            while (!mi::man->shortCircuitTest(0, ch)) //выкл
                WaitAnswerManConnErr();
            while (!mi::man->getMeasuredValue(m_list[ch], ch, RawData)) //измерение
                WaitAnswerManConnErr();
            TestModel::instance()->setTest7(m_list);
        }
    }
    CheckFinished();
    WaitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Tester::Msleep(unsigned long time)
{
    QElapsedTimer timer;
    timer.start();
    QMap<int, MeasuredValue> list;
    //    {
    //        mi::man->getMeasuredValue(list);
    //        return;
    //    }

    while (timer.elapsed() < time) {
        msleep(50);
        CheckFinished();
        while (!mi::man->getMeasuredValue(list))
            WaitAnswerManConnErr();
        if (SkipRms)
            continue;
        while ((m_inVoltage = mi::man->getRmsValue()) == 0.0)
            WaitAnswerManConnErr();
    }

    while (!mi::man->getMeasuredValue(list))
        WaitAnswerManConnErr();

    while (m_minInVoltage > m_inVoltage || m_inVoltage > m_maxInVoltage) {
        if (SkipRms)
            break;
        if (m_inVoltage == 0.0)
            WaitAnswerManConnErr();
        WaitAnswer(CheckInputVoltage);
        m_inVoltage = mi::man->getRmsValue();
    }
}
