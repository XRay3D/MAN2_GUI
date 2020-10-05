#ifndef rigolOsc_H
#define rigolOsc_H

#include <QObject>
#include <visa.h>

class DigitalOsc : public QObject {
    Q_OBJECT

    DigitalOsc(const DigitalOsc&) = delete;
    void operator=(const DigitalOsc&) = delete;

    ViSession rm = 0;
    ViSession vi = 0;
    QString m_conected;

public:
    explicit DigitalOsc(QObject* parent = nullptr);
    ~DigitalOsc();

    void ping();
    void close();

    ViStatus SetChannel(int chNum, const QString& s);
    ViStatus SetComand(const QString& s);
    QByteArray wrRdData(QByteArray data, int len = 128, bool exception = true);

    void getWav();

    bool isConnected() const { return !m_conected.isEmpty(); }

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
