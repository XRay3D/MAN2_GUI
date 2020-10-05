#ifndef MYPROTOKOL_H
#define MYPROTOKOL_H

#include <QByteArray>
#include <stdint.h>

enum {
    TX = 0xAA55,
    RX = 0x55AA,
    MIN_LEN = 6
};

#pragma pack(push, 1)

struct Parcel {
    uint16_t start;
    uint8_t length;
    uint8_t addres;
    uint8_t command;
    uint8_t data[64];
    uint8_t crc() const { return data[length - MIN_LEN]; }
    template <typename T>
    const T& value() const { return *reinterpret_cast<const T*>(data); }
    QByteArray text() const { return QByteArray(reinterpret_cast<const char*>(data), length - MIN_LEN); }
};

#pragma pack(pop)

class MyProtokol {
public:
    MyProtokol();

    template <typename T>
    QByteArray& createParcel(uint8_t cmd, const T& data, uint8_t cannel)
    {
        m_data.resize(MIN_LEN + sizeof(T));
        Parcel* d = reinterpret_cast<Parcel*>(m_data.data());
        d->start = TX;
        d->length = static_cast<uint8_t>(m_data.size());
        d->addres = cannel;
        d->command = cmd;
        memcpy(d->data, &data, sizeof(T));
        d->data[sizeof(T)] = calcCrc(m_data); //crc
        return m_data;
    }

    QByteArray& createParcel(uint8_t cmd, uint8_t cannel = 0);
    bool checkData(const QByteArray& data);
    uint8_t calcCrc(const QByteArray& data);

private:
    QByteArray m_data;
    enum { POLYNOMIAL = 0x1D }; // x^8 + x^4 + x^3 + x^2 + 1
};

#endif // MYPROTOKOL_H
