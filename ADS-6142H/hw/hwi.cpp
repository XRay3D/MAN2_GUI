#include "hwi.h"
#include <QSemaphore>
#include <QThread>

static QSemaphore semaphore;
static QThread thread;
DigitalOsc* HWI::osc = nullptr;

HWI::HWI()
{
    if (!semaphore.available()) {
        osc = new DigitalOsc();
        osc->moveToThread(&thread);
        thread.connect(&thread, &QThread::finished, osc, &QObject::deleteLater);

        thread.start();
    }
    semaphore.release(); //++
}

HWI::~HWI()
{
    semaphore.acquire(); //--
    if (!semaphore.available()) {
        thread.quit();
        thread.wait();
        osc = nullptr;
    }
}
