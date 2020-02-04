#include "worker.h"
#include "hw/interface.h"
#include "mesuremodel.h"
#include "preparation/devicemodel.h"
Worker::Worker(bool* doNotSkip, QObject* parent)
    : QThread(parent)
    , m_doNotSkip(doNotSkip)
{
}

Worker::~Worker()
{
    mi::man->DisableAll();
    m_semaphore.acquire(m_semaphore.available());
}

void Worker::Continue()
{
    m_semaphore.release(1);
}

void Worker::FinishMeasurements()
{
    mi::man->DisableAll();
    m_semaphore.release(2);
}

void Worker::run()
{
    m_counter = 0;
    setTerminationEnabled();
    m_semaphore.acquire(m_semaphore.available());
    try {
        while (!mi::man->DisableAll())
            WaitAnswer(NoConnectionWithMan);

        while (!mi::man->SwitchCurrent(On))
            WaitAnswer(NoConnectionWithMan);

        SetVoltage(SetInputVoltageUpper);
        Test4(); // Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
        SetVoltage(SetInputVoltageNormal);
        Test7(); // Тест К.З. - восстановление напряжения при подключенной нагрузке Номинальное (220) Да
        Test6(); // Поиск точки срабатывания защиты Номинальное (220) Плавное нарастание
        Test1(); // Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
        Test5(); // Измерение выходного напряжения без нагрузки Номинальное (220) Нет
        SetVoltage(SetInputVoltageLower);
        Test3(); // Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        Test2(); // Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да

    } catch (int i) {
        Q_UNUSED(i)
        //        emit ShowMessage(VERIFICATION_IS_COMPLETE);
        m_semaphore.acquire(m_semaphore.available());
        terminate();
    }
}

void Worker::WaitAnswer(int question)
{
    emit showMessage(question);
    while (!m_semaphore.tryAcquire(1, 100))
        mi::man->GetMeasuredValue(m_list);
    if (m_semaphore.available()) {
        qDebug() << "IsBreak";
        m_semaphore.acquire(m_semaphore.available());
        throw int(1);
    }
}

void Worker::CheckFinished()
{
    if (m_semaphore.available()) {
        m_semaphore.acquire(m_semaphore.available());
        throw int(1);
    }
}

void Worker::SetVoltage(int voltage)
{
    switch (voltage) {
    case SetInputVoltageUpper:
        m_minInVoltage = DeviceModel::scanSettings().Voltageerrortest5U1 - (DeviceModel::scanSettings().Voltageerrortest5U2 + 2) /** 1.5*/;
        m_maxInVoltage = DeviceModel::scanSettings().Voltageerrortest5U1 + DeviceModel::scanSettings().Voltageerrortest5U2 + 2 /** 1.5*/;
        break;
    case SetInputVoltageLower:
        m_minInVoltage = DeviceModel::scanSettings().Voltageerrortest3_4U1 - (DeviceModel::scanSettings().Voltageerrortest3_4U2 + 2) /** 1.5*/;
        m_maxInVoltage = DeviceModel::scanSettings().Voltageerrortest3_4U1 + DeviceModel::scanSettings().Voltageerrortest3_4U2 + 2 /** 1.5*/;
        break;
    case SetInputVoltageNormal:
        m_minInVoltage = 220 - 4.4;
        m_maxInVoltage = 220 + 4.4;
        break;
    default:
        break;
    }
    WaitAnswer(voltage);
}

void Worker::Test1()
{
    MesureModel::setCurrentTest(0);
    qDebug() << "Test1";
    Msleep(5000);
    do {
        Msleep(500);
        while (!mi::man->GetMeasuredValue(m_list))
            WaitAnswer(NoConnectionWithMan);
        MesureModel::setTest1(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test2()
{
    MesureModel::setCurrentTest(1);
    qDebug() << "Test2";
    for (int i = 0; i < 8; ++i) {
        if (m_doNotSkip[i]) {
            while (!mi::man->Oscilloscope(0))
                WaitAnswer(NoConnectionWithMan);
            while (!mi::man->Oscilloscope(i + 1))
                WaitAnswer(NoConnectionWithMan);
            WaitAnswer(i + PulsationsOnTheChannel_1);
        }
        emit updateProgresBar();
    }

    while (!mi::man->Oscilloscope(0))
        WaitAnswer(NoConnectionWithMan);

    emit updateProgresBar();
}

void Worker::Test3()
{
    MesureModel::setCurrentTest(2);
    qDebug() << "Test3";
    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitAnswer(NoConnectionWithMan);
        MesureModel::setTest3(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test4()
{
    MesureModel::setCurrentTest(3);
    qDebug() << "Test4";

    while (!mi::man->SetCurrent(DeviceModel::scanSettings().RatedCurrent))
        WaitAnswer(NoConnectionWithMan);

    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitAnswer(NoConnectionWithMan);
        MesureModel::setTest4(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test5()
{
    MesureModel::setCurrentTest(4);
    qDebug() << "Test5";

    while (!mi::man->SetCurrent(0))
        WaitAnswer(NoConnectionWithMan);

    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitAnswer(NoConnectionWithMan);
        MesureModel::setTest5(m_list);
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit updateProgresBar();
}

void Worker::Test6()
{
    MesureModel::setCurrentTest(5);
    qDebug() << "Test6";
    MeasuredValue_t value;

    while (!mi::man->SetCurrent(0))
        WaitAnswer(NoConnectionWithMan);

    QList<double> peak;
    for (uint8_t i = 1; i <= 8; ++i) {
        if (m_doNotSkip[i - 1]) {
            peak.append(-1.0);

            while (!mi::man->SetCurrent(DeviceModel::scanSettings().RestrictionsTest7Min, i))
                WaitAnswer(NoConnectionWithMan);

            Msleep(5000);
            for (double current = DeviceModel::scanSettings().RestrictionsTest7Min, step = (DeviceModel::scanSettings().RestrictionsTest7Max - DeviceModel::scanSettings().RestrictionsTest7Min) / 20.0;
                 current <= DeviceModel::scanSettings().RestrictionsTest7Max;
                 current += step) {
                Msleep(100);

                while (!mi::man->SetCurrent(current, i))
                    WaitAnswer(NoConnectionWithMan);

                Msleep(100);

                while (!mi::man->GetMeasuredValue(value, i))
                    WaitAnswer(NoConnectionWithMan);

                qDebug() << i << value.Value2 << value.Value1;
                if (value.Value1 < 1.0)
                    break;

                peak.append(value.Value2);
                CheckFinished();
                MesureModel::setTest6(i - 1, value.Value2);
            }
            while (!mi::man->SetCurrent(0, i))
                WaitAnswer(NoConnectionWithMan);

            std::sort(peak.begin(), peak.end());
            MesureModel::setTest6(i - 1, peak.last());
        }
        emit updateProgresBar();
    }
    WaitAnswer(RestoreTheOperationOfChannels);
    emit updateProgresBar();
}

void Worker::Test7()
{
    MesureModel::setCurrentTest(6);
    qDebug() << "Test7";
    while (!mi::man->SetCurrent(DeviceModel::scanSettings().RatedCurrent)) {
        WaitAnswer(NoConnectionWithMan);
    }
    Msleep(5000);
    do {
        Msleep(500);
        while (!mi::man->ShortCircuitTest(1))
            WaitAnswer(NoConnectionWithMan);

        Msleep(200);
        while (!mi::man->ShortCircuitTest(0))
            WaitAnswer(NoConnectionWithMan);

        Msleep(50);
        while (!mi::man->GetMeasuredValue(m_list, RAW_DATA))
            WaitAnswer(NoConnectionWithMan);

        MesureModel::setTest7(m_list);
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
        msleep(10);
        CheckFinished();

        while (!mi::man->GetMeasuredValue(m_list))
            WaitAnswer(NoConnectionWithMan);

        while ((m_inVoltage = mi::man->GetRmsValue()) == 0.0)
            WaitAnswer(NoConnectionWithMan);
    }

    while (!mi::man->GetMeasuredValue(m_list))
        WaitAnswer(NoConnectionWithMan);

    while (m_minInVoltage > m_inVoltage || m_inVoltage > m_maxInVoltage) {
        if (m_inVoltage == 0.0)
            WaitAnswer(NoConnectionWithMan);

        WaitAnswer(CheckInputVoltage);
        m_inVoltage = mi::man->GetRmsValue();
    }
}
