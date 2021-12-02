#pragma once

#include "digitalosc.h"
#include "man2.h"

class mi {
    inline static MAN2* man_;
    inline static DigitalOsc* osc_;

public:
    mi();
    ~mi();
    static MAN2* man() { return man_; }
    static DigitalOsc* osc() { return osc_; }
};
