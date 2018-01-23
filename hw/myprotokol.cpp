#include "myprotokol.h"
#include <QDebug>

MyProtokol::MyProtokol()
{
}

bool MyProtokol::CheckData(const QByteArray& data)
{
    if (data.size() > 5) {
        if (data.at(0) == -86 && data.at(1) == 85) {
            if (int(data[2]) == data.size()) {
                if (uint8_t(data[data.size() - 1]) == CalcCrc(data)) {
                    return true;
                }
            }
        }
    }
    return false;
}

QByteArray& MyProtokol::Parcel(uint8_t cmd, uint8_t cannel)
{
    m_data.resize(MIN_LEN);
    Parcel_t* d = reinterpret_cast<Parcel_t*>(m_data.data());
    d->start = TX;
    d->len = MIN_LEN;
    d->cannel = cannel;
    d->cmd = cmd;
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
