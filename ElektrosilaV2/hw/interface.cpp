#include "interface.h"
#include <QObject>

QThread thread;
QSemaphore semafore;

mi::mi()
{
    if (!semafore.available()) {

        //irt_ = new IRT59XX;
        //irt_->moveToThread(&thread);
        //thread.connect(&thread, &QThread::finished, irt_, &QObject::deleteLater);
        man = new MAN2;
        man->moveToThread(&thread);

        scpi = new SCPI;
        scpi->moveToThread(&thread);

        osc = new DigitalOsc;
        osc->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, man, &QObject::deleteLater);
        thread.connect(&thread, &QThread::finished, scpi, &QObject::deleteLater);
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
        //        thread.terminate();
        //        delete man;
        //        delete scpi;
    }
}

//MAN2* MI::man { return man; }
////IRT59XX* MI::irt() { return irt_; }
//SCPI* MI::scpi { return scpi; }
