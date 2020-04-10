#include "worker.h"
#include "hw/interface.h"
#include "preparation/devicemodel.h"
#include "testmodel.h"

#include <QSettings>
Worker::Worker(bool* doNotSkip, QObject* parent)
    : QThread(parent)
    , m_doNotSkip(doNotSkip)
{
}

Worker::~Worker()
{
    mi::man->disableAll();
    m_semaphore.acquire(m_semaphore.available());
}

void Worker::Continue()
{
    m_semaphore.release(1);
}

void Worker::FinishMeasurements()
{
    mi::man->disableAll();
    m_semaphore.release(2);
}

void Worker::run()
{
    m_counter = 0;
    setTerminationEnabled();
    m_semaphore.acquire(m_semaphore.available());
    try {
        while (!mi::man->disableAll())
            WaitAnswerManConnErr();

        while (!mi::man->switchCurrent(On))
            WaitAnswerManConnErr();

        if (TestModel::dontSkip(TestModel::Test4)) {
            SetVoltage(SetInputVoltageUpper);
            Test4(); // Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
        }
        if (TestModel::dontSkip(TestModel::Test7)) {
            SetVoltage(SetInputVoltageNormal);
            Test7(); // Тест К.З. - восстановление напряжения при подключенной нагрузке Номинальное (220) Да
        }
        if (TestModel::dontSkip(TestModel::Test6)) {
            SetVoltage(SetInputVoltageNormal);
            Test6(); // Поиск точки срабатывания защиты Номинальное (220) Плавное нарастание
        }
        if (TestModel::dontSkip(TestModel::Test1)) {
            SetVoltage(SetInputVoltageNormal);
            Test1(); // Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
        }
        if (TestModel::dontSkip(TestModel::Test5)) {
            SetVoltage(SetInputVoltageNormal);
            Test5(); // Измерение выходного напряжения без нагрузки Номинальное (220) Нет
        }
        if (TestModel::dontSkip(TestModel::Test3)) {
            SetVoltage(SetInputVoltageLower);
            Test3(); // Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        }
        if (TestModel::dontSkip(TestModel::Test2)) {
            SetVoltage(SetInputVoltageLower);
            Test2(); // Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        }
    } catch (int i) {
        Q_UNUSED(i)
        m_semaphore.acquire(m_semaphore.available());
        terminate();
    }
}

void Worker::WaitAnswer(int question)
{
    emit showMessage(question);
    while (!m_semaphore.tryAcquire(1, 100))
        if (question != NoConnectionWithMan)
            mi::man->getMeasuredValue(m_list);
    CheckFinished();
}

void Worker::WaitAnswerManConnErr()
{
    emit showMessage(NoConnectionWithMan);
    while (!m_semaphore.tryAcquire(1, 100))
        continue;
    CheckFinished();
}

void Worker::CheckFinished()
{
    if (m_semaphore.available())
        throw int(1);
}

void Worker::SetVoltage(int voltage)
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

void Worker::Test1()
{
    TestModel::setCurrentTest(TestModel::Test1);
    qDebug() << "Test1";
    Msleep(5000);
    do {
        Msleep(500);
        while (!mi::man->getMeasuredValue(m_list))
            WaitAnswerManConnErr();
        TestModel::setTest1(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test2()
{
    TestModel::setCurrentTest(TestModel::Test2);
    qDebug() << "Test2";
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

    emit updateProgresBar();
}

void Worker::Test3()
{
    TestModel::setCurrentTest(TestModel::Test3);
    qDebug() << "Test3";
    Msleep(5000);
    do {
        Msleep(500);
        while (!mi::man->getMeasuredValue(m_list))
            WaitAnswerManConnErr();
        TestModel::setTest3(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test4()
{
    TestModel::setCurrentTest(TestModel::Test4);
    qDebug() << "Test4";

    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent))
        WaitAnswerManConnErr();

    Msleep(5000);
    do {
        Msleep(500);
        while (!mi::man->getMeasuredValue(m_list))
            WaitAnswerManConnErr();
        TestModel::setTest4(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test5()
{
    TestModel::setCurrentTest(TestModel::Test5);
    qDebug() << "Test5";

    while (!mi::man->setCurrent(0))
        WaitAnswerManConnErr();

    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->getMeasuredValue(m_list))
            WaitAnswerManConnErr();
        TestModel::setTest5(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test6()
{
    TestModel::setCurrentTest(TestModel::Test6);
    qDebug() << "Test6";
    MeasuredValue value;

    while (!mi::man->setCurrent(0))
        WaitAnswerManConnErr();

    QList<double> peak;
    for (uint8_t i = 1; i <= 8; ++i) {
        if (m_doNotSkip[i - 1]) {
            peak.append(-1.0);

            while (!mi::man->setCurrent(DeviceModel::scanSettings().RestrictionsTest7Min, i))
                WaitAnswerManConnErr();

            Msleep(5000);
            double step = DeviceModel::scanSettings().RestrictionsTest7Max - DeviceModel::scanSettings().RestrictionsTest7Min;
            if (step > 100)
                step = 2.0;
            else
                step = 1.0;
            for (double current = DeviceModel::scanSettings().RestrictionsTest7Min;
                 current <= DeviceModel::scanSettings().RestrictionsTest7Max;
                 current += step) {

                while (!mi::man->setCurrent(current, i))
                    WaitAnswerManConnErr();
                msleep(50);
                while (!mi::man->getMeasuredValue(value, i, RawData))
                    WaitAnswerManConnErr();

                qDebug() << i << value.Value1 << value.Value1 << value.Value3;
                if (std::min(value.Value1, std::min(value.Value2, value.Value3)) < 1.0)
                    break;

                Msleep(100);

                peak.append(value.Value3);
                CheckFinished();
                TestModel::setTest6(i - 1, value.Value3);
            }
            while (!mi::man->setCurrent(0, i))
                WaitAnswerManConnErr();

            std::sort(peak.begin(), peak.end());
            TestModel::setTest6(i - 1, peak.last());
            peak.clear();
        }
        emit updateProgresBar();
    }
    WaitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Worker::Test7()
{
    TestModel::setCurrentTest(TestModel::Test7);
    qDebug() << "Test7";
    while (!mi::man->setCurrent(DeviceModel::scanSettings().RatedCurrent)) {
        WaitAnswerManConnErr();
    }
    Msleep(5000);

    QSettings settings("Settings.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginGroup("Test7");
    int t1 = settings.value("t1", 0).toInt();
    if (!t1)
        settings.setValue("t1", t1 = 200);
    int t2 = settings.value("t1", 0).toInt();
    if (!t2)
        settings.setValue("t2", t2 = 200);
    int t3 = settings.value("t2", 0).toInt();
    if (!t3)
        settings.setValue("t3", t3 = 50);

    do {

        for (uint8_t ch = 1; ch <= 8; ++ch) {
            if (!m_doNotSkip[ch - 1])
                continue;
            Msleep(t1); // Добавить для каждого свои в будущем
            while (!mi::man->thortCircuitTest(1, ch))
                WaitAnswerManConnErr();
            Msleep(t2); // Добавить для каждого свои в будущем
            while (!mi::man->thortCircuitTest(0, ch))
                WaitAnswerManConnErr();
            msleep(t3);
            while (!mi::man->getMeasuredValue(m_list[ch - 1], ch, RawData))
                WaitAnswerManConnErr();
        }
        //        Msleep(500);
        //        while (!mi::man->thortCircuitTest(1))
        //            WaitAnswerManConnErr();
        //        Msleep(200);
        //        while (!mi::man->thortCircuitTest(0))
        //            WaitAnswerManConnErr();
        //        msleep(50);
        //        while (!mi::man->getMeasuredValue(m_list, RawData))
        //            WaitAnswerManConnErr();

        TestModel::setTest7(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    WaitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Worker::Msleep(unsigned long time)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < time) {
        msleep(50);
        CheckFinished();
        while (!mi::man->getMeasuredValue(m_list))
            WaitAnswerManConnErr();
        while ((m_inVoltage = mi::man->getRmsValue()) == 0.0)
            WaitAnswerManConnErr();
    }
    while (!mi::man->getMeasuredValue(m_list))
        WaitAnswerManConnErr();
    while (m_minInVoltage > m_inVoltage || m_inVoltage > m_maxInVoltage) {
        if (m_inVoltage == 0.0)
            WaitAnswerManConnErr();
        WaitAnswer(CheckInputVoltage);
        m_inVoltage = mi::man->getRmsValue();
    }
}
