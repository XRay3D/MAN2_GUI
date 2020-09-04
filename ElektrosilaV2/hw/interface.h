#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include "digitalosc.h"
#include "man2.h"
#include "scpi.h"

class mi {
public:
    mi();
    ~mi();
    inline static MAN2* man;
    inline static SCPI* scpi;
    inline static DigitalOsc* osc;
};

#endif // MEASURINGINTERFACE_H
