#ifndef MEASURINGINTERFACE_H
#define MEASURINGINTERFACE_H

#include <QObject>
#include "man2.h"
#include "irt59xx.h"
#include "scpi.h"

class MI {
public:
    MI();
    ~MI();
    static MAN2* man();
    static IRT59XX* irt();
    static SCPI* scpi();
};

#endif // MEASURINGINTERFACE_H
