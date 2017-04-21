#ifndef IRT59XX_H
#define IRT59XX_H

#include <QSerialPort>

class Irt59xx : public QSerialPort {
    Q_OBJECT
public:
    explicit Irt59xx(QObject* parent = 0);
    bool Ping(const QString& potName);
    bool IsFound() const;
    double GetVoltage();

private:
    QByteArray& WriteRead(const QByteArray& data, int timeout = 10);
    QByteArray m_data;
    int m_counter;
    bool m_isFound;
};

#endif // IRT59XX_H
