#ifndef rigolOsc_H
#define rigolOsc_H

#include <QObject>
#include <QSemaphore>

class QProcess;

class DigitalOsc : public QObject {
    Q_OBJECT

    DigitalOsc(const DigitalOsc&) = delete;
    void operator=(const DigitalOsc&) = delete;

    QString m_conected;
    QProcess* process;
    QSemaphore semaphore;

public:
    explicit DigitalOsc(QObject* parent = nullptr);
    ~DigitalOsc();

    void ping();
    void close();

    //    ViStatus SetChannel(int chNum, const QString& s);
    //    ViStatus SetComand(const QString& s);
    QByteArray wrRdData(QByteArray wrData);

    bool isConnected() const { return !m_conected.isEmpty(); }
    QString idn() const { return m_conected; }

    double PKPK(int ch);
    double MIN(int ch);
    double AVERage(int ch);
    double VAMP(int ch);
    double CYCRms(int ch);

signals:
    //    void wavData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2, const QVector<double>& scale);
    //    void peakData(double u, double i);
    void errStr(const QString&);
};

#endif // rigolOsc_H
