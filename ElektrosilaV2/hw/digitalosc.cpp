#include "digitalosc.h"
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QMutex>
#include <QProcess>
#include <QThread>
#include <qt_windows.h>

int taskkill() { return ::system("taskkill /F /T /IM ADS_6142H_x32.exe"); };

DigitalOsc::DigitalOsc(QObject* parent)
    : QObject(parent)
    , proc(new QProcess)
//    , thread(new QThread)
{
    connect(proc, &QProcess::errorOccurred, [/*this*/](QProcess::ProcessError error) { qDebug() << "errorOccurred" << error; });
    connect(proc, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), [/*this*/](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "finished" << exitCode << exitStatus;
        if (exitCode == -1)
            qDebug() << taskkill();
    });

    connect(proc, &QProcess::readyReadStandardError, []() { qDebug() << "readyReadStandardError"; });
    connect(proc, &QProcess::readyReadStandardOutput, [this]() { data = proc->readAllStandardOutput(); semaphore.release(); });
    connect(proc, &QProcess::started, [this]() {  semaphore.release(); qDebug() << "started"; });
    connect(proc, &QProcess::stateChanged, [](QProcess::ProcessState newState) { qDebug() << "stateChanged" << newState; });

    //    proc->moveToThread(thread);
    //    connect(thread, &QThread::finished, proc, &QObject::deleteLater);
    //    thread->start();
}

DigitalOsc::~DigitalOsc()
{
    taskkill();
    //    thread->quit(), thread->wait();
}

void DigitalOsc::ping()
{
    close();

    if (proc->state() != QProcess::Running) {
        proc->start("ADS_6142H_x32.exe");
        if (!proc->waitForStarted()) {
            m_conected.clear();
            qDebug() << __FUNCTION__ << "!!!!!!!!!!";
        }
    }
    semaphore.acquire(semaphore.available());

    proc->write("*IDN?\r\n");
    proc->waitForBytesWritten();

    semaphore.tryAcquire(1, 1000);

    proc->waitForReadyRead(100);

    m_conected = data;
}

void DigitalOsc::close()
{
    //qDebug() << __FUNCTION__ << taskkill();
    //    if (proc && proc->state() == QProcess::Running) {
    //        proc->terminate();
    //        proc->waitForFinished(3000);
    //    }
    //    delete proc;
    //    proc = nullptr;
}

QByteArray DigitalOsc::wrRdData(QByteArray wrData)
{
    semaphore.acquire(semaphore.available());
    const bool skipRead = !wrData.endsWith('?');
    data.clear();
    do {
        while (proc->state() != QProcess::Running)
            ping();

        QElapsedTimer t;
        t.start();
        proc->write(wrData + "\r\n");
        proc->waitForBytesWritten(1000);

        if (skipRead)
            return {};

        semaphore.tryAcquire(1, 10000);

        qDebug() << t.elapsed() << "RD >>" << data.data();

        if (data.startsWith("Code 0x")) {
            proc->terminate();
            ping();
        }

    } while (data.startsWith("Code 0x") || data.isEmpty());
    return data;
}

double DigitalOsc::PKPK(int ch)
{
    try {
        //V : -154.3mV->\n"
        QRegExp exp("Vpp : ([+-]?\\d+\\.?\\d*)(\\w+).*");
        const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
        exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":PKPK?"));
        return exp.cap(1).toDouble() * k[exp.cap(2)];
    } catch (const QString& str) {
        qDebug() << str;
    } catch (...) {
        qDebug() << "Unknown Err";
    }
    return 9999.9;
}

double DigitalOsc::MIN(int ch)
{
    try {
        //V : -154.3mV->\n"
        QRegExp exp("Mi : ([+-]?\\d+\\.?\\d*)(\\w+).*");
        const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
        exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":MIN?"));
        return exp.cap(1).toDouble() * k[exp.cap(2)];
    } catch (const QString& str) {
        qDebug() << str;
    } catch (...) {
        qDebug() << "Unknown Err";
    }
    return 9999.9;
}

double DigitalOsc::AVERage(int ch)
{
    try {
        //V : -154.3mV->\n"
        QRegExp exp("V : ([+-]?\\d+\\.?\\d*)(\\w+).*");
        const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
        exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":AVERage?"));
        return exp.cap(1).toDouble() * k[exp.cap(2)];
    } catch (const QString& str) {
        qDebug() << str;
    } catch (...) {
        qDebug() << "Unknown Err";
    }
    return 9999.9;
}

double DigitalOsc::VAMP(int ch)
{
    try {
        //V : -154.3mV->\n"
        QRegExp exp("Va : ([+-]?\\d+\\.?\\d*)(\\w+).*");
        const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
        exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":VAMP?"));
        return exp.cap(1).toDouble() * k[exp.cap(2)];
    } catch (const QString& str) {
        qDebug() << str;
    } catch (...) {
        qDebug() << "Unknown Err";
    }
    return 9999.9;
}

double DigitalOsc::CYCRms(int ch)
{
    try {
        //V : -154.3mV->\n"
        QRegExp exp("TR : ([+-]?\\d+\\.?\\d*)(\\w+).*");
        const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
        //    QStringList list(QString("MAX|MIN|PKPK|VTOP|VBASe|VAMP|AVERage"
        //                             "|SQUAresum|CYCRms|CURSorrms|OVERShoot"
        //                             "|PREShoot|PERiod|FREQuency|RTime|FTime"
        //                             "|PWIDth|NWIDth|PDUTy|NDUTy|SCREenduty"
        //                             "|FRR|FRF|FFR|FFF|LRR|LRF|LFR|LFF|RDELay"
        //                             "|FDELay|RPHAse|PPULsenum|NPULsenum"
        //                             "|RISEedgenum|FALLedgenum|AREA|CYCLearea"
        //                             "|HARDfrequency")
        //                         .split('|'));
        //    for (auto var : list) {
        //        wrRdData(":MEASUrement:CH2:" + var.toLocal8Bit() + "?");
        //    }
        exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":CYCRms?"));
        return exp.cap(1).toDouble() * k[exp.cap(2)];
    } catch (const QString& str) {
        qDebug() << str;
    } catch (...) {
        qDebug() << "Unknown Err";
    }
    return 9999.9;
}
