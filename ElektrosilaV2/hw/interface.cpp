#include "interface.h"
#include <QObject>

QThread thread;
QSemaphore semafore;

mi::mi()
{
    if (!semafore.available()) {
        man = new MAN2;
        man->moveToThread(&thread);

        osc = new DigitalOsc;
        osc->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, man, &QObject::deleteLater);
        thread.connect(&thread, &QThread::finished, osc, &QObject::deleteLater);

        thread.start(QThread::NormalPriority);
    }
    semafore.release();
}

mi::~mi()
{
    semafore.acquire();
    if (!semafore.available()) {
        man->disableAll();
        thread.quit();
        thread.wait();
    }
}
