#include "interface.h"
#include <QObject>

QThread thread;
QSemaphore semafore;

mi::mi()
{
    if (!semafore.available()) {
        man_ = new MAN2;
        man_->moveToThread(&thread);

        osc_ = new DigitalOsc;
        osc_->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, man_, &QObject::deleteLater);
        thread.connect(&thread, &QThread::finished, osc_, &QObject::deleteLater);
        thread.start(QThread::NormalPriority);
    }
    semafore.release();
}

mi::~mi()
{
    semafore.acquire();
    if (!semafore.available()) {
        man_->disableAll();
        thread.quit();
        thread.wait();
    }
}
