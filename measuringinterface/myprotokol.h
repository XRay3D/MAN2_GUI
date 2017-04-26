#ifndef MYPROTOKOL_H
#define MYPROTOKOL_H

#include <QByteArray>
#include <stdint.h>

#define POLYNOMIAL 0x1D // x^8 + x^4 + x^3 + x^2 + 1
class MyProtokol {
public:
    MyProtokol();

    QByteArray& Parcel(uint8_t cmd, uint8_t* pData, uint8_t Len, uint8_t cannel = 0);
    QByteArray& Parcel(uint8_t cmd, const QByteArray& data, uint8_t cannel = 0);
    QByteArray& Parcel(uint8_t cmd, uint8_t cannel = 0);
    bool CheckData(const QByteArray& data);
    uint8_t CalcCrc(const QByteArray& data);

private:
    QByteArray m_data;
};

#endif // MYPROTOKOL_H
