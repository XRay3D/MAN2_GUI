#include "digitalosc.h"
#include <QDebug>
#include <QMutex>
#include <QProcess>
#include <QThread>
#include <qt_windows.h>

DigitalOsc::DigitalOsc(QObject* parent)
    : QObject(parent)
    , process(new QProcess(/*this*/))
{
    connect(process, &QProcess::errorOccurred, [](QProcess::ProcessError error) {
        qDebug() << "errorOccurred" << error;
    });
    connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), [](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "finished" << exitCode << exitStatus;
        if (exitCode == -1)
            ::system("taskkill /F /T /IM Osc_ADS_6142H_x32.exe");
    });

    connect(process, &QProcess::readyReadStandardError, []() { qDebug() << "readyReadStandardError"; });
    connect(process, &QProcess::readyReadStandardOutput, [this]() { semaphore.release(); });
    connect(process, &QProcess::started, []() { qDebug() << "started"; });
    connect(process, &QProcess::stateChanged, [](QProcess::ProcessState newState) { qDebug() << "stateChanged" << newState; });
}

DigitalOsc::~DigitalOsc() { close(); }

void DigitalOsc::ping()
{
    qDebug() << "DigitalOsc::ping()";

    close();

    process->start("Osc_ADS_6142H_x32.exe");
    process->waitForStarted();

    thread()->sleep(1);
    process->write("*IDN?\n");
    process->waitForBytesWritten(1000);
    int ctr = 0;
    while (!process->canReadLine() && ++ctr < 10)
        process->waitForReadyRead(100);

    qDebug() << "ping" << (m_conected = process->readAll());
    //    process->write("*IDN?");
    //    process->waitForBytesWritten(1000);
}

void DigitalOsc::close()
{
    //int retval = ::system("taskkill /F /T /IM Osc_ADS_6142H_x32.exe");
    //qDebug() << "close" << retval;
    m_conected.clear();
    if (process->state() == QProcess::Running) {
        process->write("exit");
        process->waitForBytesWritten(1000);
        process->terminate();
    }
}

QByteArray DigitalOsc::wrRdData(QByteArray wrData)
{
    const bool skipRead = !wrData.endsWith('?');
    QByteArray rdData;
    do {

        while (!process->isOpen()) {
            ping();
        }

        if (0 && skipRead)
            qDebug() << "WR<<" << wrData;

        process->write(wrData + "\r\n");
        process->waitForBytesWritten(1000);
        if (skipRead) {
            thread()->msleep(100);
            return {};
        }
        semaphore.tryAcquire(1, 10000);
        rdData = process->readAll();

        if (0 && !skipRead)
            qDebug() << QString("WR<<").append(wrData).append(" RD>>").append(rdData).toLocal8Bit().data();

        qDebug() << QString("RD >> ").append(rdData).toLocal8Bit().data();

        if (rdData.startsWith("Code 0x"))
            ping();

    } while (rdData.startsWith("Code 0x") || rdData.isEmpty());
    return rdData;
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
