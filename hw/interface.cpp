#include "interface.h"
#include <QObject>

QThread thread;
QSemaphore semafore;

MAN2* mi::man = nullptr;
SCPI* mi::scpi = nullptr;
//IRT59XX* irt_ = nullptr;

mi::mi()
{
    if (!semafore.available()) {
        man = new MAN2;
        scpi = new SCPI;
        //irt_ = new IRT59XX;

        man->moveToThread(&thread);
        scpi->moveToThread(&thread);
        //irt_->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, man, &QObject::deleteLater);
        thread.connect(&thread, &QThread::finished, scpi, &QObject::deleteLater);
        //thread.connect(&thread, &QThread::finished, irt_, &QObject::deleteLater);

        thread.start(QThread::NormalPriority);
    }
    semafore.release();
}

mi::~mi()
{
    semafore.acquire();
    if (!semafore.available()) {
        man->DisableAll();
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
