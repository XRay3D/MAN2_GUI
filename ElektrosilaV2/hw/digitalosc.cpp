#include "digitalosc.h"
#include <QDebug>
#include <QMutex>

DigitalOsc::DigitalOsc(QObject* parent)
    : QObject(parent)
{
}

DigitalOsc::~DigitalOsc() { close(); }

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
        if (vi)
            qDebug() << (m_conected = wrRdData("*IDN?", false, VI_FIND_BUFLEN));
    }
}

void DigitalOsc::close()
{
    m_conected.clear();
    if (vi)
        viClose(vi);
    if (rm)
        viClose(rm);
    rm = 0;
    vi = 0;
}

ViStatus DigitalOsc::SetChannel(int chNum, const QString& s)
{
    if (vi == 0)
        return -1;
    return viVPrintf(vi, QString(":CHAN%1:%2\n").arg(chNum).arg(s).toLocal8Bit().data(), nullptr);
}

ViStatus DigitalOsc::SetComand(const QString& s)
{
    if (vi == 0)
        return -1;
    return viPrintf(vi, QString(s).append('\n').toLocal8Bit().data());
}

QByteArray DigitalOsc::wrRdData(QByteArray data, bool exception, int len)
{
    qDebug() << "Wr" << data;
    static QMutex m;
    QMutexLocker ml(&m);
    ViStatus status;
    int c = 0;
    do {
        if (/* DISABLES CODE */ (0))
            do {
                // viReadAsync/viWriteAsync example --
                // These commands can potentially decrease test time by allowing
                // several read or write commands to happen in parallel.   ViJobId jobid;
                ViEventType eventType;
                ViEvent event;
                ViJobId jobid;
                QByteArray retData(len, 0);
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
            } while (0);
        else
            do {
                ViUInt32 size;
                QByteArray retData(len, 0);
                status = viWrite(vi, reinterpret_cast<ViBuf>(data.data()), static_cast<ViUInt32>(data.size()), &size);
                if (status < VI_SUCCESS)
                    break;
                status = viRead(vi, reinterpret_cast<ViBuf>(retData.data()), static_cast<ViUInt32>(retData.size()), &size);
                if (status < VI_SUCCESS)
                    break;
                retData.resize(static_cast<int>(strlen(retData.data())));
                emit errStr("");
                qDebug() << "Rd" << retData;
                return retData;
            } while (0);
        if (status != VI_SUCCESS) { //          VI_ERROR_IO;            VI_ERROR_TMO;
            char desc[512]; //The size of the desc parameter should be at _least_ 256 bytes.
            viStatusDesc(vi, status, desc);
            QString str(QString("Code 0x%1 (%2)").arg(status, 0, 16).arg(desc));
            if (exception)
                throw str;
            else
                qDebug() << "qDebug" << str << ++c;
            emit errStr(str);
        }
    } while (c < 10);
    return {};
}

//void DigitalOsc::setup(const Tvs& d)
//{
//    if (vi == 0)
//        return;
//    diode = d;

//    SetChannel(1, "COUP DC");
//    SetChannel(1, "DISP ON");
//    SetChannel(1, "INV ON");
//    SetChannel(1, "PROB 10"); //ATTENUATOR
//    SetChannel(1, QString("SCAL %1").arg(diode.data[11].toInt())); //current
//    SetChannel(1, QString("OFFS %1").arg(diode.data[12].toInt()));

//    SetChannel(2, "COUP DC");
//    SetChannel(2, "INV OFF");
//    SetChannel(2, "DISP ON");
//    SetChannel(2, "PROB 10"); //ATTENUATOR
//    SetChannel(2, QString("SCAL %1").arg(diode.data[13].toInt())); //voltage
//    SetChannel(2, QString("OFFS %1").arg(diode.data[14].toInt()));

//    SetComand(":MATH:DISPlay OFF");
//    SetComand(":TRIG:EDGE:SOUR EXT");
//    SetComand(":TRIG:EDGE:LEV 1");
//    SetComand(":TRIG:EDGE:SWE NORM");
//    SetComand(":TRIG:EDGE:COUP DC");
//    SetComand(":TRIG:EDGE:SLOP POS");

//    SetComand(":TIM:MODE MAIN");
//    SetComand(":TIM:OFFS 0.001");
//    SetComand(":TIM:SCAL 0.0002");
//    SetComand(":TIM:FORM YT");
//}

void DigitalOsc::getWav() // rigol ds1000
{
    //    try {
    //        if (vi == 0)
    //            return;
    //        // thread()->sleep(10);
    //        QVector<double> scale(4);
    //        double ch1Offs = wrRdData(":CHAN1:OFFS?").toDouble(); // Return Channel vertical offset = <Vert_Offset>
    //        double ch1Scal = wrRdData(":CHAN1:SCAL?").toDouble(); // Return Channel 1 scale = <Volts_Div>
    //        double ch2Offs = wrRdData(":CHAN2:OFFS?").toDouble(); // Return Channel vertical offset = <Vert_Offset>
    //        double ch2Scal = wrRdData(":CHAN2:SCAL?").toDouble(); // Return Channel 1 scale = <Volts_Div>
    //        double tOffs = wrRdData(":TIM:OFFS?").toDouble(); //     Return current trigger offset = <Time_Offset>
    //        double tScal = wrRdData(":TIM:SCAL?").toDouble(); //     Return current time/division setting = <Time_Div>
    //        QByteArray d1(wrRdData(":WAV:DATA? CHAN1", true, 610));
    //        QByteArray d2(wrRdData(":WAV:DATA? CHAN2", true, 610));
    //        quint8* u1 = reinterpret_cast<quint8*>(d1.data() + 10);
    //        quint8* u2 = reinterpret_cast<quint8*>(d2.data() + 10);
    //        double maxC = 0.0;
    //        double maxV = 0.0;
    //        QVector<QPointF> ch1(600);
    //        QVector<QPointF> ch2(600);
    //        for (int i = 0; i < 600; ++i) {
    //            const double x1 = (i * (tScal / 50) - ((tScal * 6) - tOffs)) * 1000000;
    //            const double x2 = (i * (tScal / 50) - ((tScal * 6) - tOffs)) * 1000000;
    //            const double y1 = -((240 - u1[i]) * (ch1Scal / 25) - (ch1Offs + ch1Scal * 4.6) + 2 * ch1Offs);
    //            const double y2 = (240 - u2[i]) * (ch2Scal / 25) - (ch2Offs + ch2Scal * 4.6);
    //            ch1[i] = { x1, y1 };
    //            ch2[i] = { x2, y2 };
    //            if (maxC < y1) {
    //                maxC = y1;
    //                maxV = y2;
    //            }
    //        }
    //        scale[0] = ch1.first().x();
    //        scale[1] = ch1.last().x();
    //        scale[2] = std::min(-ch1Scal * 4 - ch1Offs, -ch2Scal * 4 - ch2Offs);
    //        scale[3] = std::max(ch1Scal * 4 - ch1Offs, ch2Scal * 4 - ch2Offs);
    //        qDebug() << QString("%1\t%2\t%3").arg(maxC).arg(maxV).arg(maxC * maxV).replace('.', ',').toLocal8Bit().data();
    //        emit wavData(ch1, ch2, scale);
    //        emit peakData(maxV, maxC);
    //    } catch (QString s) {
    //        qDebug() << s;
    //    }
}

//        mi::man->oscilloscope(1);
//        QStringList list(QString("MAX|MIN|PKPK|VTOP|VBASe|VAMP|AVERage"
//                                 "|SQUAresum|CYCRms|CURSorrms|OVERShoot"
//                                 "|PREShoot|PERiod|FREQuency|RTime|FTime"
//                                 "|PWIDth|NWIDth|PDUTy|NDUTy|SCREenduty"
//                                 "|FRR|FRF|FFR|FFF|LRR|LRF|LFR|LFF|RDELay"
//                                 "|FDELay|RPHAse|PPULsenum|NPULsenum"
//                                 "|RISEedgenum|FALLedgenum|AREA|CYCLearea"
//                                 "|HARDfrequency")
//                             .split('|'));
//        for (auto var : list) {
//            mi::osc->wrRdData(":MEASUrement:CH2:" + var.toLocal8Bit() + "?");
//            //break;
//        }

double DigitalOsc::PKPK(int ch)
{
    //V : -154.3mV->\n"
    QRegExp exp("Vpp : ([+-]?\\d+\\.?\\d*)(\\w+).*");
    const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
    exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":PKPK?"));
    return exp.cap(1).toDouble() * k[exp.cap(2)];
}

double DigitalOsc::MIN(int ch)
{
    //V : -154.3mV->\n"
    QRegExp exp("Mi : ([+-]?\\d+\\.?\\d*)(\\w+).*");
    const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
    exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":MIN?"));
    return exp.cap(1).toDouble() * k[exp.cap(2)];
}

double DigitalOsc::AVERage(int ch)
{
    //V : -154.3mV->\n"
    QRegExp exp("V : ([+-]?\\d+\\.?\\d*)(\\w+).*");
    const QMap<QString, double> k({ { "mV", 0.001 }, { "V", 1.0 } });
    exp.exactMatch(wrRdData(":MEASUrement:CH" + QByteArray::number(ch) + ":AVERage?"));
    return exp.cap(1).toDouble() * k[exp.cap(2)];
}
