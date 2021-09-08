#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include "digitalosc.h"
#include "man2.h"

class mi {
public:
    mi();
    ~mi();
    inline static MAN2* man;
    inline static DigitalOsc* osc;
};

#endif // MEASURINGINTERFACE_H
