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

public:
    explicit DigitalOsc(QObject* parent = nullptr);
    ~DigitalOsc();

    void ping();
    void close();

    QString conected;

    ViStatus SetChannel(int chNum, const QString& s);
    ViStatus SetComand(const QString& s);
    QByteArray wrRdData(QByteArray data, bool exception = true, int len = 20);

    void getWav();

signals:
    //    void wavData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2, const QVector<double>& scale);
    //    void peakData(double u, double i);
    void errStr(const QString&);
};

#endif // rigolOsc_H
