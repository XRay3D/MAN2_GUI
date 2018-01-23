#include "interface.h"

QThread thread;
QSemaphore semafore;

MAN2* man_ = nullptr;
SCPI* scpi_ = nullptr;
//IRT59XX* irt_ = nullptr;

MI::MI()
{
    if (!semafore.available()) {
        man_ = new MAN2;
        scpi_ = new SCPI;
        //irt_ = new IRT59XX;

        man_->moveToThread(&thread);
        scpi_->moveToThread(&thread);
        //irt_->moveToThread(&thread);

        thread.connect(&thread, &QThread::finished, man_, &QObject::deleteLater);
        thread.connect(&thread, &QThread::finished, scpi_, &QObject::deleteLater);
        //thread.connect(&thread, &QThread::finished, irt_, &QObject::deleteLater);

        thread.start(QThread::NormalPriority);
    }
    semafore.release();
}

MI::~MI()
{
    semafore.acquire();
    if (!semafore.available()) {
        man_->DisableAll();
        thread.quit();
        thread.wait();
        //        thread.terminate();
        //        delete man_;
        //        delete scpi_;
    }
}

MAN2* MI::man() { return man_; }
//IRT59XX* MI::irt() { return irt_; }
SCPI* MI::scpi() { return scpi_; }
