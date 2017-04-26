#include "measuringinterface.h"

MAN2* m_man = new MAN2;
IRT59XX* m_irt = new IRT59XX;
SCPI* m_scpi = new SCPI;
QThread m_manThread;
QSemaphore m_semafore;

MI::MI()
{
    if (!m_semafore.available()) {
        m_man = new MAN2;
        m_irt = new IRT59XX;
        m_scpi = new SCPI;
        MI::man()->moveToThread(&m_manThread);
        m_manThread.connect(&m_manThread, &QThread::finished, MI::man(), &QObject::deleteLater);
        m_manThread.start(QThread::NormalPriority);
    }
    m_semafore.release();
}

MI::~MI()
{
    m_semafore.acquire();
    if (!m_semafore.available()) {
        m_manThread.quit();
        m_manThread.wait();
        //        delete m_man;
        delete m_irt;
        delete m_scpi;
    }
}

MAN2* MI::man()
{
    return m_man;
}

IRT59XX* MI::irt()
{
    return m_irt;
}

SCPI* MI::scpi()
{
    return m_scpi;
}
