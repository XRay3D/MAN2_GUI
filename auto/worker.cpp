#include "worker.h"
#include "hw/interface.h"

Worker::Worker(bool* doNotSkip, Result_t* result, const ScanSettings* scanSettings, QObject* parent)
    : QThread(parent)
    , m_doNotSkip(doNotSkip)
    , m_result(result)
    , m_pScanSettings(scanSettings)
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

void Worker::Quit()
{
    mi::man->DisableAll();
}

void Worker::run()
{
    m_counter = 0;
    setTerminationEnabled();
    m_semaphore.acquire(m_semaphore.available());
    try {
        while (!mi::man->DisableAll())
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        while (!mi::man->SwitchCurrent(On))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        // 3 Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
        SetVoltage(SET_INPUT_VOLTAGE_2);
        Test1();
        // 4 Измерение выходного напряжения без нагрузки Номинальное (220) Нет
        Test5();
        // 5 Поиск точки срабатывания защиты Номинальное (220) Плавное нарастание
        Test6();
        // 6 Тест К.З. - восстановление напряжения при подключенной нагрузке Номинальное (220) Да
        Test7();
        // 8 Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        SetVoltage(SET_INPUT_VOLTAGE_1);
        Test3();
        // 7 Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        Test2();
        // 2 Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
        SetVoltage(SET_INPUT_VOLTAGE_0);
        Test4();
    }
    catch (int i) {
        Q_UNUSED(i)
        emit ShowMessage(VERIFICATION_IS_COMPLETE);
        m_semaphore.acquire(m_semaphore.available());
        terminate();
    }
}

void Worker::WaitSolutionOrEnd(int question)
{
    emit ShowMessage(question);
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
    case SET_INPUT_VOLTAGE_0:
        m_minInVoltage = m_pScanSettings->Voltageerrortest5U1 - m_pScanSettings->Voltageerrortest5U2 /** 1.5*/;
        m_maxInVoltage = m_pScanSettings->Voltageerrortest5U1 + m_pScanSettings->Voltageerrortest5U2 /** 1.5*/;
        break;
    case SET_INPUT_VOLTAGE_1:
        m_minInVoltage = m_pScanSettings->Voltageerrortest3_4U1 - m_pScanSettings->Voltageerrortest3_4U2 /** 1.5*/;
        m_maxInVoltage = m_pScanSettings->Voltageerrortest3_4U1 + m_pScanSettings->Voltageerrortest3_4U2 /** 1.5*/;
        break;
    case SET_INPUT_VOLTAGE_2:
        m_minInVoltage = 220 - 4.4;
        m_maxInVoltage = 220 + 4.4;
        break;
    default:
        break;
    }
    return WaitSolutionOrEnd(voltage);
}

void Worker::Test1()
{
    qDebug() << "Test1";
    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test1 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit ShowMessage(TEST_1);
}

void Worker::Test2()
{
    qDebug() << "Test2";
    for (int i = 0; i < 8; ++i) {
        if (m_doNotSkip[i]) {
            while (!mi::man->Oscilloscope(0))
                WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

            while (!mi::man->Oscilloscope(i + 1))
                WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

            WaitSolutionOrEnd(i + PULSATIONS_ON_THE_CHANNEL_1);
        }
    }

    while (!mi::man->Oscilloscope(0))
        WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

    emit ShowMessage(TEST_2);
}

void Worker::Test3()
{
    qDebug() << "Test3";
    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test3 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit ShowMessage(TEST_3);
}

void Worker::Test4()
{
    qDebug() << "Test4";

    while (!mi::man->SetCurrent(m_pScanSettings->RatedCurrent))
        WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        for (int i = 0; i < m_list.size(); ++i)
            m_result[i].test4 = m_list[i].Value1;
        CheckFinished();

    } while (m_list.size() != 8);
    CheckFinished();
    emit ShowMessage(TEST_4);
}

void Worker::Test5()
{
    qDebug() << "Test5";

    while (!mi::man->SetCurrent(0))
        WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

    Msleep(5000);
    do {
        Msleep(500);

        while (!mi::man->GetMeasuredValue(m_list))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test5 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        CheckFinished();
    } while (m_list.size() != 8);
    CheckFinished();
    emit ShowMessage(TEST_5);
}

void Worker::Test6()
{
    qDebug() << "Test6";
    MeasuredValue_t value;
    while (!mi::man->SetCurrent(0)) {
        WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);
    };
    QList<double> peak;
    for (uint8_t i = 1; i <= 8; ++i) {
        if (m_doNotSkip[i - 1]) {
            peak.append(-1.0);

            while (!mi::man->SetCurrent(m_pScanSettings->RestrictionsTest7Min, i))
                WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

            Msleep(5000);
            for (double current = m_pScanSettings->RestrictionsTest7Min, step = (m_pScanSettings->RestrictionsTest7Max - m_pScanSettings->RestrictionsTest7Min) / 20.0;
                 current <= m_pScanSettings->RestrictionsTest7Max;
                 current += step) {
                Msleep(100);

                while (!mi::man->SetCurrent(current, i))
                    WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

                Msleep(100);

                while (!mi::man->GetMeasuredValue(value, i))
                    WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

                qDebug() << i << value.Value2 << value.Value1;
                if (value.Value1 < 1.0)
                    break;

                peak.append(value.Value2);
                CheckFinished();
            }

            while (!mi::man->SetCurrent(0, i))
                WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

            std::sort(peak.begin(), peak.end());
            m_result[i - 1].test6 = peak.last();
        }
    }
    emit ShowMessage(TEST_6);
    WaitSolutionOrEnd(RESTORE_THE_OPERATION_OF_CHANNELS);
}

void Worker::Test7()
{
    qDebug() << "Test7";
    while (!mi::man->SetCurrent(m_pScanSettings->RatedCurrent)) {
        WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);
    }
    Msleep(5000);
    do {
        Msleep(500);
        while (!mi::man->ShortCircuitTest(1))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        Msleep(2000);
        while (!mi::man->ShortCircuitTest(0))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        Msleep(100);
        while (!mi::man->GetMeasuredValue(m_list))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test7 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        CheckFinished();

    } while (m_list.size() != 8);
    emit ShowMessage(TEST_7);
    WaitSolutionOrEnd(RESTORE_THE_OPERATION_OF_CHANNELS);
}

void Worker::Msleep(unsigned long time)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < time) {
        emit UpdateProgresBar();
        msleep(10);
        CheckFinished();

        while (!mi::man->GetMeasuredValue(m_list))
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        while ((m_inVoltage = mi::man->GetRmsValue()) == 0)
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);
    }

    while (!mi::man->GetMeasuredValue(m_list))
        WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

    while (m_minInVoltage > m_inVoltage || m_inVoltage > m_maxInVoltage) {
        if (m_inVoltage == 0.0)
            WaitSolutionOrEnd(NO_CONNECTION_WITH_MAN);

        WaitSolutionOrEnd(CHECK_INPUT_VOLTAGE);
        m_inVoltage = mi::man->GetRmsValue();
    }
}
