#include "measurements.h"
#include <QEvent>
#include <QSignalMapper>
#include <measuringinterface/measuringinterface.h>

#define SignalMapperInt static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped)

Measurements::Measurements(QWidget* parent)
    : QWidget(parent)
    , m_timerMeasure(0)
{
    setupUi(this);

    m_listDsbCurrent << dsbCurrent_1 << dsbCurrent_2 << dsbCurrent_3 << dsbCurrent_4 << dsbCurrent_5 << dsbCurrent_6 << dsbCurrent_7 << dsbCurrent_8;
    m_listDsbSetCurrent << dsbSetCurrent_1 << dsbSetCurrent_2 << dsbSetCurrent_3 << dsbSetCurrent_4 << dsbSetCurrent_5 << dsbSetCurrent_6 << dsbSetCurrent_7 << dsbSetCurrent_8;
    m_listDsbVoltage << dsbVoltage_1 << dsbVoltage_2 << dsbVoltage_3 << dsbVoltage_4 << dsbVoltage_5 << dsbVoltage_6 << dsbVoltage_7 << dsbVoltage_8;
    m_listPbCurrent << pbCurrent_1 << pbCurrent_2 << pbCurrent_3 << pbCurrent_4 << pbCurrent_5 << pbCurrent_6 << pbCurrent_7 << pbCurrent_8;
    m_listPbOsc << pbOsc_1 << pbOsc_2 << pbOsc_3 << pbOsc_4 << pbOsc_5 << pbOsc_6 << pbOsc_7 << pbOsc_8;
    m_listPbShort << pbShort_1 << pbShort_2 << pbShort_3 << pbShort_4 << pbShort_5 << pbShort_6 << pbShort_7 << pbShort_8;
    m_listGroupBox << gbChannel_1 << gbChannel_2 << gbChannel_3 << gbChannel_4 << gbChannel_5 << gbChannel_6 << gbChannel_7 << gbChannel_8;

    smCurrent = new QSignalMapper(this);
    for (int i = 0; i < m_listPbCurrent.size(); ++i) {
        smCurrent->connect(m_listPbCurrent[i], SIGNAL(clicked()), smCurrent, SLOT(map()));
        smCurrent->setMapping(m_listPbCurrent[i], i);
    }
    smCurrent->connect(smCurrent, SignalMapperInt, this, &Measurements::PbCurrentClicked, Qt::DirectConnection);

    smShort = new QSignalMapper(this);
    for (int i = 0; i < m_listPbShort.size(); ++i) {
        smShort->connect(m_listPbShort[i], SIGNAL(clicked()), smShort, SLOT(map()));
        smShort->setMapping(m_listPbShort[i], i);
    }
    smShort->connect(smShort, SignalMapperInt, this, &Measurements::PbShortClicked, Qt::DirectConnection);

    smOsc = new QSignalMapper(this);
    for (int i = 0; i < m_listPbOsc.size(); ++i) {
        smOsc->connect(m_listPbOsc[i], SIGNAL(clicked()), smOsc, SLOT(map()));
        smOsc->setMapping(m_listPbOsc[i], i);
    }
    smOsc->connect(smOsc, SignalMapperInt, this, &Measurements::PbOscClicked, Qt::DirectConnection);

    smSetCurrent = new QSignalMapper(this);
    for (int i = 0; i < m_listDsbSetCurrent.size(); ++i) {
        smSetCurrent->connect(m_listDsbSetCurrent[i], SIGNAL(valueChanged(double)), smSetCurrent, SLOT(map()));
        smSetCurrent->setMapping(m_listDsbSetCurrent[i], i);
    }
    smSetCurrent->connect(smSetCurrent, SignalMapperInt, this, &Measurements::DsbSetCurrent, Qt::DirectConnection);

    smGroupBox = new QSignalMapper(this);
    for (int i = 0; i < m_listGroupBox.size(); ++i) {
        smGroupBox->connect(m_listGroupBox[i], SIGNAL(clicked(bool)), smGroupBox, SLOT(map()));
        smGroupBox->setMapping(m_listGroupBox[i], i);
    }
    smGroupBox->connect(smGroupBox, SignalMapperInt, this, &Measurements::GbChanneClicked, Qt::DirectConnection);

    widget->legend->setVisible(true);
    widget->legend->setFont(QFont("Helvetica", 9));
    widget->legend->setRowSpacing(-3);
    QVector<QCPScatterStyle::ScatterShape> shapes;
    //    shapes << QCPScatterStyle::ssCross;
    //    shapes << QCPScatterStyle::ssPlus;
    //    shapes << QCPScatterStyle::ssCircle;
    //    shapes << QCPScatterStyle::ssDisc;
    //    shapes << QCPScatterStyle::ssSquare;
    //    shapes << QCPScatterStyle::ssDiamond;
    //    shapes << QCPScatterStyle::ssStar;
    //    shapes << QCPScatterStyle::ssTriangle;
    //    shapes << QCPScatterStyle::ssTriangleInverted;
    //    shapes << QCPScatterStyle::ssCrossSquare;
    //    shapes << QCPScatterStyle::ssPlusSquare;
    //    shapes << QCPScatterStyle::ssCrossCircle;
    //    shapes << QCPScatterStyle::ssPlusCircle;
    //    shapes << QCPScatterStyle::ssPeace;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;
    shapes << QCPScatterStyle::ssCustom;

    QPen pen;
    // add graphs with different scatter styles:
    for (int i = 0; i < shapes.size(); ++i) {
        widget->addGraph();
        //        pen.setColor(QColor(
        //            qSin(qDegreesToRadians(i * (360 / 8.0))) * 100 + 100,
        //            qSin(qDegreesToRadians(i * (360 / 8.0) + 100)) * 100 + 100,
        //            qSin(qDegreesToRadians(i * (360 / 8.0) + 200)) * 100 + 100));
        pen.setColor(QColor::fromHsv((360.0 / 8.0) * i, 255, 200));
        // generate data:
        //        QVector<double> x(10), y(10);
        //        for (int k = 0; k < 10; ++k) {
        //            x[k] = k / 10.0 * 4 * 3.14 + 0.01;
        //            y[k] = 7 * qSin(x[k]) / x[k] + (shapes.size() - i) * 5;
        //        }
        //        widget->graph()->setData(x, y);
        //        widget->graph()->rescaleAxes(true);
        widget->graph()->setPen(pen);
        //        widget->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
        widget->graph()->setName(QString("Канал %1, В.").arg(i + 1));
        widget->graph()->setLineStyle(QCPGraph::lsLine);
        // set scatter style:
        if (shapes.at(i) != QCPScatterStyle::ssCustom) {
            widget->graph()->setScatterStyle(QCPScatterStyle(shapes.at(i), 5));
        }
        else {
            QPainterPath customScatterPath;
            QString str = QString().setNum(i + 1);
            QFont font("Courier");
            QRectF rect = QFontMetricsF(font).boundingRect(str);
            customScatterPath.addText(-rect.width() / 2, rect.height() / 3, font, str);
            //            for (int i = 0; i < 3; ++i)
            //                customScatterPath.cubicTo(qCos(2 * M_PI * i / 3.0) * 9, qSin(2 * M_PI * i / 3.0) * 9, qCos(2 * M_PI * (i + 0.9) / 3.0) * 9, qSin(2 * M_PI * (i + 0.9) / 3.0) * 9, 0, 0);
            //            widget->graph()->setScatterStyle(QCPScatterStyle(customScatterPath, QPen(Qt::black, 0), QColor(40, 70, 255, 50), 10));
            widget->graph()->setScatterStyle(QCPScatterStyle(customScatterPath, QPen(Qt::black), Qt::NoBrush, 5));
        }
    }
    // set blank axis lines:
    //    widget->rescaleAxes();
    //    widget->xAxis->setTicks(false);
    //    widget->yAxis->setTicks(false);
    //    widget->xAxis->setTickLabels(false);
    //    widget->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    widget->axisRect()->setupFullAxesBox();
    widget->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    widget->xAxis2->setTickLabels(false);
    widget->xAxis->setDateTimeFormat("hh:mm:ss");
    widget->connect(
        widget->yAxis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged),
        widget->yAxis2, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::setRange), Qt::DirectConnection);
    widget->connect(
        widget->xAxis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged),
        widget->xAxis2, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::setRange), Qt::DirectConnection);
}

void Measurements::on_cbOsc_currentIndexChanged(int index)
{
    cbOsc->setCurrentIndex(index);
    for (int i = 0; i < m_listPbOsc.size(); ++i) {
        if (i == index - 1) {
            m_listPbOsc[i]->setChecked(true);
            m_listPbOsc[i]->setText("Выкл.");
        }
        else {
            m_listPbOsc[i]->setChecked(false);
            m_listPbOsc[i]->setText("Вкл.");
        }
    }
    MI::man()->Oscilloscope(0);
    if (index) {
        MI::man()->Oscilloscope(index);
    }
}

void Measurements::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerMeasure) {
        QMutexLocker locker(&m_mutex);
        if (m_semaphore.tryAcquire()) {
            m_semaphore.acquire(m_semaphore.available());
            StartMeasure();
        }
    }
}
void Measurements::on_pbStart_clicked(bool checked)
{
    QMutexLocker locker(&m_mutex);
    pbStart->setChecked(checked);
    if (checked) {
        m_timerMeasure = startTimer(sbTimeout->value());
        pbStart->setText("Стоп");
        m_semaphore.release();
    }
    else {
        killTimer(m_timerMeasure);
        m_timerMeasure = 0;
        pbStart->setText("Старт");
    }
}

void Measurements::on_sbTimeout_valueChanged(int arg1)
{
    QMutexLocker locker(&m_mutex);
    if (m_timerMeasure) {
        killTimer(m_timerMeasure);
        m_timerMeasure = startTimer(arg1);
    }
}

void Measurements::on_dsbSetCurrentAll_valueChanged(double arg1)
{
    smSetCurrent->disconnect(smSetCurrent, SignalMapperInt, this, &Measurements::DsbSetCurrent);
    foreach (QDoubleSpinBox* spinBox, m_listDsbSetCurrent) {
        spinBox->setValue(arg1);
    }
    MI::man()->SetCurrent(arg1);
    smSetCurrent->connect(smSetCurrent, SignalMapperInt, this, &Measurements::DsbSetCurrent, Qt::DirectConnection);
}

void Measurements::on_pbCurrentAll_clicked(bool checked)
{
    foreach (QPushButton* button, m_listPbCurrent) {
        button->setChecked(checked);
        button->setText(checked ? "Выкл." : "Вкл.");
    }
    MI::man()->SwitchCurrent(checked);
}

void Measurements::on_pbShortAll_clicked(bool checked)
{
    foreach (QPushButton* button, m_listPbShort) {
        button->setChecked(checked);
        button->setText(checked ? "Выкл." : "Вкл.");
    }
    MI::man()->ShortCircuitTest(checked);
}

void Measurements::PbCurrentClicked(int channel)
{
    QPushButton* btn = m_listPbCurrent[channel];
    bool checked = btn->isChecked();
    if (checked) {
        btn->setText("Выкл.");
    }
    else {
        btn->setText("Вкл.");
    }
    MI::man()->SwitchCurrent(checked, channel + 1);
}

void Measurements::DsbSetCurrent(int channel)
{
    double value = m_listDsbSetCurrent[channel]->value();
    MI::man()->SetCurrent(value, channel + 1);
}

void Measurements::PbShortClicked(int channel)
{
    QPushButton* btn = m_listPbShort[channel];
    bool checked = btn->isChecked();
    if (checked) {
        btn->setText("Выкл.");
    }
    else {
        btn->setText("Вкл.");
    }
    MI::man()->ShortCircuitTest(checked, channel + 1);
}

void Measurements::PbOscClicked(int channel)
{
    QPushButton* btn = m_listPbOsc[channel];
    if (btn->isChecked()) {
        on_cbOsc_currentIndexChanged(channel + 1);
    }
    else {
        on_cbOsc_currentIndexChanged(0);
    }
}

void Measurements::GetMeasuredValueSlot(const QMap<int, MeasuredValue_t>& list)
{
    QMutexLocker locker(&m_mutex);
    qDebug() << "Measurements::GetMeasuredValueSlot";
    static QTime time(3, 0, 0, 0); //(/*QTime::currentTime() - */QTime(3, 0, 0, 0));
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
    QMapIterator<int, MeasuredValue_t> iterator(list);
    while (iterator.hasNext()) {
        iterator.next();
        m_listDsbVoltage[iterator.key() - 1]->setValue(iterator.value().Value1);
        m_listDsbCurrent[iterator.key() - 1]->setValue(iterator.value().Value2);
        m_listDsbSetCurrent[iterator.key() - 1]->setValue(iterator.value().Value3);
        widget->graph(iterator.key() - 1)->addData(key, iterator.value().Value1);
    }
    if (list.size() == 8) {
        m_semaphore.release();
    }
    else {
        if (m_timerMeasure) {
            qDebug() << "else Measurements::GetMeasuredValueSlot";
            killTimer(m_timerMeasure);
            m_timerMeasure = startTimer(sbTimeout->value());
            m_semaphore.release();
        }
    }
    widget->xAxis->rescale();
    widget->yAxis->rescale(true);
    widget->replot();
}

void Measurements::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    qDebug() << "showEvent";
    if (MI::man()->IsConnected()) {

        QList<MeasuredValue_t> list;
        if (MI::man()->GetMeasuredValue(list)) {
            smSetCurrent->disconnect(smSetCurrent, SignalMapperInt, this, &Measurements::DsbSetCurrent);
            for (int i = 0; i < list.size(); ++i) {
                m_listDsbVoltage[i]->setValue(list[i].Value1);
                m_listDsbCurrent[i]->setValue(list[i].Value2);
                m_listDsbSetCurrent[i]->setValue(list[i].Value3);
                if (list[i].ManState.Load) {
                    m_listPbCurrent[i]->setChecked(true);
                    m_listPbCurrent[i]->setText("Выкл.");
                }
                if (list[i].ManState.ShortCircuit) {
                    m_listPbShort[i]->setChecked(true);
                    m_listPbShort[i]->setText("Выкл.");
                }
                if (list[i].ManState.Oscilloscope) {
                    m_listPbOsc[i]->setChecked(true);
                    m_listPbOsc[i]->setText("Выкл.");
                }
            }
            smSetCurrent->connect(smSetCurrent, SignalMapperInt, this, &Measurements::DsbSetCurrent, Qt::DirectConnection);
        }
        connect(this, &Measurements::StartMeasure, MI::man(), &MAN2::GetMeasuredValueSlot);
        connect(MI::man(), &MAN2::GetMeasuredValueSignal, this, &Measurements::GetMeasuredValueSlot);
        return;
    }
    setEnabled(false);
}

void Measurements::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    on_pbStart_clicked(false);
    disconnect(this, &Measurements::StartMeasure, MI::man(), &MAN2::GetMeasuredValueSlot);
    disconnect(MI::man(), &MAN2::GetMeasuredValueSignal, this, &Measurements::GetMeasuredValueSlot);
}

void Measurements::GbChanneClicked(int channel)
{
    if (m_listGroupBox[channel]->isChecked()) {
        widget->graph(channel)->setVisible(true);
    }
    else {
        widget->graph(channel)->setVisible(false);
    }
    widget->xAxis->rescale();
    widget->yAxis->rescale();
    widget->replot();
}

void Measurements::on_pbClear_clicked()
{
    widget->graph(0)->data()->clear();
    widget->graph(1)->data()->clear();
    widget->graph(2)->data()->clear();
    widget->graph(3)->data()->clear();
    widget->graph(4)->data()->clear();
    widget->graph(5)->data()->clear();
    widget->graph(6)->data()->clear();
    widget->graph(7)->data()->clear();
}
