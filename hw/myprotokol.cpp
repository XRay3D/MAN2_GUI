#include "myprotokol.h"
#include <QDebug>

MyProtokol::MyProtokol()
{
}

bool MyProtokol::CheckData(const QByteArray& data)
{
    const Parcel_t* const d = reinterpret_cast<const Parcel_t*>(data.data());
    if (data.size() > 5)
        if (d->start == RX)
            if (d->length == data.size())
                if (d->crc() == CalcCrc(data))
                    return true;
    return false;
}

QByteArray& MyProtokol::Parcel(uint8_t cmd, uint8_t cannel)
{
    m_data.resize(MIN_LEN);
    Parcel_t* d = reinterpret_cast<Parcel_t*>(m_data.data());
    d->start = TX;
    d->length = MIN_LEN;
    d->addres = cannel;
    d->command = cmd;
    d->data[0] = CalcCrc(m_data); //crc
    return m_data;
}

uint8_t MyProtokol::CalcCrc(const QByteArray& data)
{
    uint8_t crc8 = 0;
    for (uint16_t i = 0, len = data.size() - 1; i < len; ++i) {
        crc8 ^= data[i];
        for (uint16_t j = 0; j < 8; ++j) {
            crc8 = (crc8 & 0x80) ? (crc8 << 1) ^ POLYNOMIAL : crc8 << 1;
        }
    }
    return crc8;
}
