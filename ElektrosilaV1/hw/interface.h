#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include "man2.h"
//#include "irt59xx.h"
#include "scpi.h"

class mi {
public:
    mi();
    ~mi();
    static MAN2* man;
    static SCPI* scpi;
};

#endif // MEASURINGINTERFACE_H
