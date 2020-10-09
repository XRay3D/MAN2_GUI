#ifndef rigolOsc_H
#define rigolOsc_H

#include <string>
#include <vector>
#include <visa.h>

using namespace std;

class DigitalOsc {
    DigitalOsc(const DigitalOsc&) = delete;
    void operator=(const DigitalOsc&) = delete;

    ViSession rm = 0;
    ViSession vi = 0;

public:
    explicit DigitalOsc();
    ~DigitalOsc();

    void close();
    void ping();

    string wrRdData(string data, int len = 128, bool exception = false);
};

#endif // rigolOsc_H
