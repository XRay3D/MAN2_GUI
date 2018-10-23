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

typedef struct Parcel_t {
    uint16_t start;
    uint8_t length;
    uint8_t addres;
    uint8_t command;
    uint8_t data[128];
    uint8_t crc() const { return data[length - MIN_LEN]; }
    template <typename T>
    T value() const { return *reinterpret_cast<const T*>(data); }
    QByteArray text() const { return QByteArray(reinterpret_cast<const char*>(data), length - MIN_LEN); }
} Parcel_t;

#pragma pack(pop)

class MyProtokol {
public:
    MyProtokol();

    template <typename T>
    QByteArray& Parcel(uint8_t cmd, const T& data, uint8_t cannel)
    {
        m_data.resize(MIN_LEN + sizeof(T));
        Parcel_t* d = reinterpret_cast<Parcel_t*>(m_data.data());
        d->start = TX;
        d->length = m_data.size();
        d->addres = cannel;
        d->command = cmd;
        memcpy(d->data, &data, sizeof(T));
        d->data[sizeof(T)] = CalcCrc(m_data); //crc
        return m_data;
    }

    QByteArray& Parcel(uint8_t cmd, uint8_t cannel = 0);
    bool CheckData(const QByteArray& data);
    uint8_t CalcCrc(const QByteArray& data);

private:
    QByteArray m_data;
    enum { POLYNOMIAL = 0x1D }; // x^8 + x^4 + x^3 + x^2 + 1
};

#endif // MYPROTOKOL_H
