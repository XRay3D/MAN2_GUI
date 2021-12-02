#include "digitalosc.h"
#include <QApplication>
#include <QAuthenticator>
#include <QDebug>
#include <QElapsedTimer>
#include <QMutex>
#include <QProcess>
#include <QThread>
#include <qt_windows.h>

DigitalOsc::DigitalOsc(QObject* parent)
    : QObject(parent)
    , thrd(new QThread)
    , socket(new QTcpSocket(thrd))
{

    connect(socket, &QAbstractSocket::hostFound, [] { qDebug("hostFound"); });
    connect(socket, &QAbstractSocket::connected, [] { qDebug("connected"); });
    connect(socket, &QAbstractSocket::disconnected, [] { qDebug("disconnected"); });
    //    connect(socket, &QAbstractSocket::stateChanged, [](QAbstractSocket::SocketState state) { qDebug() << "stateChanged" << state; });
    //    connect(socket, qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::error), [](QAbstractSocket::SocketError error) {
    //        qDebug() << "error" << error;
    //    });
    //    connect(socket, &QAbstractSocket::proxyAuthenticationRequired, [](const QNetworkProxy& proxy, QAuthenticator* authenticator) {
    //        qDebug() << "proxyAuthenticationRequired" << &proxy << authenticator;
    //    });

    connect(this, &DigitalOsc::Write, socket, qOverload<const QByteArray&>(&QIODevice::write), Qt::QueuedConnection);
    connect(socket, &QIODevice::readyRead, [this] {
        data = socket->readAll();
        semaphore.release();
    });

    connect(thrd, &QThread::finished, socket, &QObject::deleteLater);
    thrd->start();
    qDebug() << __FUNCTION__ << thrd;
}

DigitalOsc::~DigitalOsc()
{
    thrd->quit();
    thrd->wait();
}

void DigitalOsc::ping()
{
    socket->connectToHost("192.168.1.72", 3000);
    socket->write("*IDN?");
    int ctr = 10;
    while (data.isEmpty() && ctr--) {
        socket->waitForReadyRead(100);
        qDebug() << ctr;
    }
    m_conected = data;
    qDebug() << m_conected;
}

void DigitalOsc::close()
{
    socket->disconnectFromHost();
}

QByteArray DigitalOsc::wrRdData(QByteArray wrData)
{
    semaphore.acquire(semaphore.available());
    const bool skipRead = !wrData.endsWith('?');
    data.clear();
    QElapsedTimer t;
    t.start();
    do {
        qDebug() << t.elapsed() << "WR <<" << wrData.data();
        emit Write(wrData);
        if (skipRead) {
            thread()->msleep(250);
            return {};
        }
        semaphore.tryAcquire(1, 10000);
        qDebug() << t.elapsed() << "ms RD >>" << data.data();
    } while (data.startsWith("Code 0x") || data.isEmpty());
    return data;
}

bool DigitalOsc::isConnected() const
{
    if (EMU)
        return true;
    return !m_conected.isEmpty();
}

double DigitalOsc::PKPK(int ch)
{
    try {
        //V : -154.3mV->\n"
        QRegExp exp(R"(Vpp : ([+-]?\d+\.?\d*)(\w+).*)");
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
        QRegExp exp(R"(Mi : ([+-]?\d+\.?\d*)(\w+).*)");
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
        QRegExp exp(R"(V : ([+-]?\d+\.?\d*)(\w+).*)");
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
        QRegExp exp(R"(Va : ([+-]?\d+\.?\d*)(\w+).*)");
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
        QRegExp exp(R"(TR : ([+-]?\d+\.?\d*)(\w+).*)");
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
