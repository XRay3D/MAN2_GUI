#include "digitalosc.h"
//#include <QDebug>
//#include <QMutex>
#include <sstream>

DigitalOsc::DigitalOsc() { ping(); }

DigitalOsc::~DigitalOsc() { close(); }

string DigitalOsc::wrRdData(string data, int len, bool exception)
{
    ViStatus status;
    int c = 0;
    do {
        do {
            if constexpr (/* DISABLES CODE */ (0)) {
                // viReadAsync/viWriteAsync example --
                // These commands can potentially decrease test time by allowing
                // several read or write commands to happen in parallel.   ViJobId jobid;
                ViEventType eventType;
                ViEvent event;
                ViJobId jobid;
                string retData(len, 0);
                // Enable waiting on the events
                status = viEnableEvent(vi, VI_EVENT_IO_COMPLETION, VI_QUEUE, VI_NULL);
                if (status < VI_SUCCESS)
                    break;
                // Write commands to several devices (this allows several writes to be done in parallel)
                status = viWriteAsync(vi, reinterpret_cast<ViBuf>(data.data()), static_cast<ViUInt32>(data.size()), &jobid);
                if (status < VI_SUCCESS)
                    break;
                // Wait for completion on all of the devices
                viWaitOnEvent(vi, VI_EVENT_IO_COMPLETION, VI_INFINITE, &eventType, &event);
                // Queue the read for all the devices (this allows several reads to be done im parallel)
                status = viReadAsync(vi, reinterpret_cast<ViBuf>(retData.data()), static_cast<ViUInt32>(retData.size()), &jobid);
                if (status < VI_SUCCESS)
                    break;
                // Wait for all the reads to complete
                status = viWaitOnEvent(vi, VI_EVENT_IO_COMPLETION, VI_INFINITE, &eventType, &event);
                if (status < VI_SUCCESS)
                    break;
                // Cleanup and exit
                status = viDisableEvent(vi, VI_EVENT_IO_COMPLETION, VI_QUEUE);
                if (status < VI_SUCCESS)
                    break;
                retData.resize(static_cast<int>(strlen(retData.data())));
                return retData;
            } else {
                ViUInt32 size = 0;
                string retData(len, 0);
                status = viWrite(vi, reinterpret_cast<ViBuf>(data.data()), static_cast<ViUInt32>(data.size()), &size);
                if (status < VI_SUCCESS)
                    break;
                status = viRead(vi, reinterpret_cast<ViBuf>(retData.data()), static_cast<ViUInt32>(retData.size()), &size);
                if (status < VI_SUCCESS)
                    break;
                retData.resize(static_cast<int>(strlen(retData.data())));
                return retData;
            }
        } while (0);
        if (status != VI_SUCCESS) {
            // VI_ERROR_IO;
            // VI_ERROR_TMO;
            char desc[512]; //The size of the desc parameter should be at _least_ 256 bytes.
            viStatusDesc(vi, status, desc);
            stringstream str;
            str << "Code 0x" << hex << status;
            str << " (" << desc << ')' << endl;
            if (exception) {
                throw str.str();
            } else {
                return str.str();
            }
        }
    } while (c < 10);
    return {};
}

void DigitalOsc::ping()
{
    ViChar buffer[VI_FIND_BUFLEN];
    ViRsrc matches = buffer;
    close();
    viOpenDefaultRM(&rm);
    // acquire USB resource of visa
    if (rm) {
        viFindRsrc(rm, const_cast<const ViString>("USB?*"), nullptr, nullptr, matches);
        viOpen(rm, matches, VI_NULL, VI_NULL, &vi);
        if (vi) {
            //qDebug() << (m_conected = wrRdData("*IDN?", VI_FIND_BUFLEN, false));
        }
    }
}

void DigitalOsc::restart()
{
    close();
    ping();
}

void DigitalOsc::close()
{
    if (vi)
        viClose(vi);
    if (rm)
        viClose(rm);
    rm = 0;
    vi = 0;
}
