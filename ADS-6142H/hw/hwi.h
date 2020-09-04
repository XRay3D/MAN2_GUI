#ifndef HWI_H
#define HWI_H

#include "digitalosc.h"

class HWI {
    HWI(const HWI&) = delete;
    void operator=(const HWI&) = delete;

public:
    explicit HWI();
    ~HWI();
    static DigitalOsc* osc;
};

#endif // HWI_H
