#ifndef rigolOsc_H
#define rigolOsc_H

#include "config.h"
#include <QObject>
#include <QSemaphore>
#include <QTcpSocket>

class DigitalOsc : public QObject {
    Q_OBJECT

    DigitalOsc(const DigitalOsc&) = delete;
    void operator=(const DigitalOsc&) = delete;

    QString m_conected;
    QThread* thrd;
    QTcpSocket* socket;
    QSemaphore semaphore;
    QByteArray data;

public:
    explicit DigitalOsc(QObject* parent = nullptr);
    ~DigitalOsc();

    void ping();
    void close();

    QByteArray wrRdData(QByteArray wrData);

    bool isConnected() const;
    QString idn() const { return m_conected; }

    double PKPK(int ch);
    double MIN(int ch);
    double AVERage(int ch);
    double VAMP(int ch);
    double CYCRms(int ch);

signals:

    void errStr(const QString&);
    void Write(const QByteArray&);
};

#endif // rigolOsc_H
