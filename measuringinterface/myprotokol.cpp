#include "myprotokol.h"
//#include <QDebug>

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

QByteArray& MyProtokol::Parcel(uint8_t cmd, uint8_t* pData, uint8_t Len, uint8_t cannel)
{
    m_data.clear();
    m_data.push_back(85); //старт 0x55
    m_data.push_back(-86); //старт 0xAA
    if (pData != nullptr) {
        m_data.push_back(Len + 6); //размер посылки
    }
    else {
        m_data.push_back(6); //размер посылки
    }
    m_data.push_back(cannel);
    m_data.push_back(cmd);
    if (pData != nullptr) {
        for (int i = 0; i < Len; ++i) {
            m_data.push_back(*pData++); //данные
        }
    }
    m_data.push_back(1); //байт для crc
    m_data[m_data.size() - 1] = CalcCrc(m_data); //crc
    //    qDebug() << "Parcel" << data.toHex().toUpper();
    return m_data;
}

QByteArray& MyProtokol::Parcel(uint8_t cmd, uint8_t cannel)
{
    return Parcel(cmd, nullptr, 0, cannel);
}

uint8_t MyProtokol::CalcCrc(const QByteArray& data)
{
    uint8_t crc8 = 0;
    uint8_t len = data.size() - 1;
    for (uint16_t i = 0; i < len; ++i) {
        crc8 ^= data[i];
        for (uint16_t j = 0; j < 8; ++j) {
            if (crc8 & 0x80) {
                crc8 = (crc8 << 1) ^ POLYNOMIAL;
            }
            else {
                crc8 <<= 1;
            }
        }
    }
    return crc8;
}

QByteArray& MyProtokol::Parcel(uint8_t cmd, const QByteArray& data, uint8_t cannel)
{
    return Parcel(cmd, (uint8_t*)data.data(), (uint8_t)data.size(), cannel);
}
