#include "worker.h"
#include "hw/interface.h"

///////////////////////////////////////
/// \brief Worker::Worker
/// \param doNotSkip
/// \param result
/// \param scanSettings
/// \param man
/// \param parent
///
Worker::Worker(bool* doNotSkip, Result_t* result, const ScanSettings* scanSettings, QObject* parent)
    : QThread(parent)
    , m_doNotSkip(doNotSkip)
    , m_result(result)
    , m_pScanSettings(scanSettings)
{
}

Worker::~Worker()
{
    MI::man()->DisableAll();
    m_semaphore.acquire(m_semaphore.available());
    // quit();
    // wait();
}

void Worker::Continue()
{
    m_semaphore.release(1);
}

void Worker::FinishMeasurements()
{
    MI::man()->DisableAll();
    m_semaphore.release(2);
}

void Worker::Quit()
{
    MI::man()->DisableAll();
}

void Worker::run()
{
    m_counter = 0;
    setTerminationEnabled();
    m_semaphore.acquire(m_semaphore.available());
    do {
        while (!MI::man()->DisableAll()) {
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                break;
        }
        while (!MI::man()->SwitchCurrent(On)) {
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                break;
        }
        // 3 Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
        if (SetVoltage(SET_INPUT_VOLTAGE_2))
            break;
        if (Test1())
            break;
        // 4 Измерение выходного напряжения без нагрузки Номинальное (220) Нет
        if (Test5())
            break;
        // 5 Поиск точки срабатывания защиты Номинальное (220) Плавное нарастание
        if (Test6())
            break;
        // 6 Тест К.З. - восстановление напряжения при подключенной нагрузке Номинальное (220) Да
        if (Test7())
            break;
        // 8 Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        if (SetVoltage(SET_INPUT_VOLTAGE_1))
            break;
        if (Test3())
            break;
        // 7 Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да
        if (Test2())
            break;
        // 2 Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
        if (SetVoltage(SET_INPUT_VOLTAGE_0))
            break;
        if (Test4())
            break;
    } while (0);

    emit ShowMessage(VERIFICATION_IS_COMPLETE);
    m_semaphore.acquire(m_semaphore.available());
}

bool Worker::IsBreak()
{
    while (!m_semaphore.tryAcquire(1, 100))
        MI::man()->GetMeasuredValue(m_list);
    if (m_semaphore.available()) {
        qDebug() << "IsBreak";
        m_semaphore.acquire(m_semaphore.available());
        return true;
    }
    return false;
}

bool Worker::WaitUserSolution(int question)
{
    emit ShowMessage(question);
    if (IsBreak())
        return true;
    return false;
}

bool Worker::CheckFinished()
{
    if (m_semaphore.available()) {
        m_semaphore.acquire(m_semaphore.available());
        return true;
    }
    return false;
}

bool Worker::SetVoltage(int voltage)
{
    switch (voltage) {
    case SET_INPUT_VOLTAGE_0:
        m_minInVoltage = m_pScanSettings->Voltageerrortest5U1 - m_pScanSettings->Voltageerrortest5U2 * 1.5;
        m_maxInVoltage = m_pScanSettings->Voltageerrortest5U1 + m_pScanSettings->Voltageerrortest5U2 * 1.5;
        break;
    case SET_INPUT_VOLTAGE_1:
        m_minInVoltage = m_pScanSettings->Voltageerrortest3_4U1 - m_pScanSettings->Voltageerrortest3_4U2 * 1.5;
        m_maxInVoltage = m_pScanSettings->Voltageerrortest3_4U1 + m_pScanSettings->Voltageerrortest3_4U2 * 1.5;
        break;
    case SET_INPUT_VOLTAGE_2:
        m_minInVoltage = 220 - 4.4;
        m_maxInVoltage = 220 + 4.4;
        break;
    default:
        break;
    }
    if (WaitUserSolution(voltage))
        return true;
    return false;
}

bool Worker::Test1()
{
    qDebug() << "Test1";
    Msleep(5000);
    do {
        Msleep(500);
        while (!MI::man()->GetMeasuredValue(m_list)) {
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        }
        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test1 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        if (CheckFinished())
            return true;
    } while (m_list.size() != 8);
    if (CheckFinished())
        return true;
    emit ShowMessage(TEST_1);
    return false;
}

bool Worker::Test2()
{
    qDebug() << "Test2";
    for (int i = 0; i < 8; ++i) {
        if (m_doNotSkip[i]) {
            while (!MI::man()->Oscilloscope(0)) {

                if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                    return true;
            }
            while (!MI::man()->Oscilloscope(i + 1)) {

                if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                    return true;
            }

            if (WaitUserSolution(i + PULSATIONS_ON_THE_CHANNEL_1))
                return true;
        }
    }
    while (!MI::man()->Oscilloscope(0)) {

        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
            return true;
    }
    emit ShowMessage(TEST_2);
    return false;
}

bool Worker::Test3()
{
    qDebug() << "Test3";
    Msleep(5000);
    do {
        Msleep(500);

        while (!MI::man()->GetMeasuredValue(m_list)) {

            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        }
        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test3 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        if (CheckFinished())
            return true;
    } while (m_list.size() != 8);
    if (CheckFinished())
        return true;
    emit ShowMessage(TEST_3);
    return false;
}

bool Worker::Test4()
{
    qDebug() << "Test4";
    while (!MI::man()->SetCurrent(m_pScanSettings->RatedCurrent)) {
        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
            return true;
    }

    Msleep(5000);

    do {
        Msleep(500);
        while (!MI::man()->GetMeasuredValue(m_list))
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        for (int i = 0; i < m_list.size(); ++i)
            m_result[i].test4 = m_list[i].Value1;
        if (CheckFinished())
            return true;
    } while (m_list.size() != 8);
    if (CheckFinished())
        return true;
    emit ShowMessage(TEST_4);
    return false;
}

bool Worker::Test5()
{
    qDebug() << "Test5";
    while (!MI::man()->SetCurrent(0)) {

        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
            return true;
    }
    Msleep(5000);
    do {
        Msleep(500);
        while (!MI::man()->GetMeasuredValue(m_list)) {

            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        }
        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test5 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        if (CheckFinished())
            return true;
    } while (m_list.size() != 8);
    if (CheckFinished())
        return true;
    emit ShowMessage(TEST_5);
    return false;
}

bool Worker::Test6()
{
    qDebug() << "Test6";
    MeasuredValue_t value;
    while (!MI::man()->SetCurrent(0)) {

        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
            return true;
    };
    QList<double> peak;
    for (uint8_t i = 1; i <= 8; ++i) {
        if (m_doNotSkip[i - 1]) {
            peak.append(-1.0);
            while (!MI::man()->SetCurrent(m_pScanSettings->RestrictionsTest7Min, i)) {
                Msleep(100);
                while (!MI::man()->SetCurrent(m_pScanSettings->RestrictionsTest7Min, i)) {

                    if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                        return true;
                };
            };
            Msleep(5000);
            for (double current = m_pScanSettings->RestrictionsTest7Min;
                 current <= m_pScanSettings->RestrictionsTest7Max * 1.1;
                 current += (m_pScanSettings->RestrictionsTest7Max - m_pScanSettings->RestrictionsTest7Min) / 20.0) {
                Msleep(100);
                while (!MI::man()->SetCurrent(current, i)) {
                    Msleep(100);
                    while (!MI::man()->SetCurrent(current, i)) {

                        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                            return true;
                    };
                };
                Msleep(100);
                while (!MI::man()->GetMeasuredValue(value, i)) {
                    Msleep(100);
                    while (!MI::man()->GetMeasuredValue(value, i)) {

                        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                            return true;
                    }
                }
                qDebug() << i << value.Value2 << value.Value1;
                if (value.Value1 < 1.0) {
                    break;
                }
                peak.append(value.Value2);
                if (CheckFinished())
                    return true;
            }
            while (!MI::man()->SetCurrent(0, i)) {

                if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                    return true;
            };
            std::sort(peak.begin(), peak.end());
            m_result[i - 1].test6 = peak.last();
        }
    }
    emit ShowMessage(TEST_6);
    return false;
}

bool Worker::Test7()
{

    if (WaitUserSolution(RESTORE_THE_OPERATION_OF_CHANNELS))
        return true;
    qDebug() << "Test7";
    while (!MI::man()->SetCurrent(m_pScanSettings->RatedCurrent)) {

        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
            return true;
    }
    Msleep(5000);
    do {
        Msleep(500);
        while (!MI::man()->ShortCircuitTest(1)) {

            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        }
        Msleep(2000);
        while (!MI::man()->ShortCircuitTest(0)) {

            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        }
        Msleep(100);
        while (!MI::man()->GetMeasuredValue(m_list)) {

            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return true;
        }
        for (int i = 0; i < m_list.size(); ++i) {
            m_result[i].test7 = m_list[i].Value1;
            qDebug() << i << m_list[i].Value1;
        }
        if (CheckFinished())
            return true;
    } while (m_list.size() != 8);
    emit ShowMessage(TEST_7);
    return false;
}

bool Worker::Msleep(unsigned long time)
{
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < time) {
        emit UpdateProgresBar();
        msleep(10);
        if (CheckFinished()) {
            terminate();
            return false;
        }
        while (!MI::man()->GetMeasuredValue(m_list)) {
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return false;
        }
        while ((m_inVoltage = MI::man()->GetMeasuredValueRMS()) == 0) {
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return false;
        }
    }

    while (!MI::man()->GetMeasuredValue(m_list)) {
        if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
            return false;
    }

    while (m_minInVoltage > m_inVoltage || m_inVoltage > m_maxInVoltage) {
        if (m_inVoltage == 0.0) {
            if (WaitUserSolution(NO_CONNECTION_WITH_MAN))
                return false;
        }
        else if (WaitUserSolution(CHECK_INPUT_VOLTAGE)) {
            terminate();
            return false;
        }
        m_inVoltage = MI::man()->GetMeasuredValueRMS();
    }
}
