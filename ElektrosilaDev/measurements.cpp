#include "measurements.h"
#include "hw/interface.h"
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QEvent>
#include <QLineSeries>
#include <QPen>
#include <QValueAxis>
#include <limits>

QT_CHARTS_USE_NAMESPACE

Measurements::Measurements(QWidget* parent)
    : QWidget(parent)
    , m_timerMeasure(0)
{
    setupUi(this);

    graphicsView = new QChartView(this);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    gridLayout->addWidget(graphicsView, 13, 0, 1, 3);

    connect(pbCurrent_1, &QPushButton::clicked, [=](bool checked) {
        if (mi::man->switchCurrent(checked, mi::man->address()))
            pbCurrent_1->setText(checked ? "Выкл." : "Вкл.");
        else
            pbCurrent_1->setChecked(!checked);
    });
    connect(pbShort_1, &QPushButton::clicked, [=](bool checked) {
        //        if (mi::man->shortCircuitTest(checked ? cbxSt->currentIndex() + 1 : 0, mi::man->address()))
        //            pbShort_1->setText(checked ? "Выкл." : "Вкл.");
        //        else
        //            pbShort_1->setChecked(!checked);
    });
    connect(pbOsc_1, &QPushButton::clicked, [=](bool checked) {
        if (mi::man->oscilloscope(checked ? mi::man->address() : 0))
            pbOsc_1->setText(checked ? "Выкл." : "Вкл.");
        else
            pbOsc_1->setChecked(!checked);
    });

    connect(dsbSetCurrent_1, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double val) {
        mi::man->setCurrent(val, mi::man->address());
    });

    connect(chbx_1, &QCheckBox::clicked, [=](bool checked) { m_series.at(0)->setVisible(checked); });
    connect(chbx_2, &QCheckBox::clicked, [=](bool checked) { m_series.at(1)->setVisible(checked); });
    connect(chbx_3, &QCheckBox::clicked, [=](bool checked) { m_series.at(2)->setVisible(checked); });

    QChart* chart = new QChart();
    QValueAxis* axisY = new QValueAxis;
    QPen pen(axisY->gridLinePen().color(), 0.0, Qt::DotLine);
    axisY->setMinorGridLinePen(pen);
    axisY->setMinorTickCount(4);
    chart->addAxis(axisY, Qt::AlignLeft);

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setFormat("h:mm:ss");
    axisX->setTickCount(7);
    chart->addAxis(axisX, Qt::AlignBottom);
    for (int i = 0; i < 3; ++i) {
        m_series.append(new QLineSeries(chart));
        QPen pen(Qt::black, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen.setColor(QColor::fromHsv((360.0 / 3.0) * i, 255, 255));
        m_series.last()->setPen(pen);
        m_series.last()->setName(QString("Канал %1, В.").arg(i + 1));
        chart->addSeries(m_series.last());
        m_series.last()->attachAxis(axisY);
        m_series.last()->attachAxis(axisX);
    }

    //    chart->createDefaultAxes();
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundBrush(Qt::white);

    graphicsView->setChart(chart);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setPalette(QPalette(Qt::white));
}

void Measurements::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerMeasure) {
        qDebug(Q_FUNC_INFO);
        QMutexLocker locker(&m_mutex);
        if (1 || m_semaphore.tryAcquire()) {
            m_semaphore.acquire(m_semaphore.available());
            StartMeasure(static_cast<ValueType>(cbxValType->currentIndex()), mi::man->address());
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
        m_keyX = m_minX = QDateTime();
        minY = +std::numeric_limits<double>::max();
        maxY = -std::numeric_limits<double>::max();
    } else {
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

void Measurements::GetMeasuredValueSlot(const MeasuredValue& list)
{
    qDebug(Q_FUNC_INFO);

    QMutexLocker locker(&m_mutex);
    enum { MaxCount = 5000 };
    if (m_minX == m_keyX)
        m_minX = m_keyX = QDateTime::currentDateTime();

    //    QElapsedTimer timer;
    //    timer.start();

    bool flag = false;

    //    double rms = 0.0;
    //    rms = mi::man->getRmsValue();
    //    dsbVoltage_9->setValue(rms);

    //    iterator.next();
    //    int i = iterator.key() - 1;
    //    m_listDsbVoltage[i]->setValue(iterator.value().Value1);
    //    m_listDsbCurrent[i]->setValue(iterator.value().Value2);
    dsbVoltage_1->setValue(static_cast<double>(list.ch1));
    dsbVoltage_2->setValue(static_cast<double>(list.ch2));
    dsbVoltage_3->setValue(static_cast<double>(list.ch3));
    m_series[0]->append(m_keyX.toMSecsSinceEpoch(), list.ch1);
    m_series[1]->append(m_keyX.toMSecsSinceEpoch(), list.ch2);
    m_series[2]->append(m_keyX.toMSecsSinceEpoch(), list.ch3);

    for (auto var : m_series) {
        if (var->count() > MaxCount)
            var->remove(0);
        if (var->isVisible()) {
            minY = qMin(minY, var->points().last().y());
            maxY = qMax(maxY, var->points().last().y());
            flag = true;
        }
    }

    if (flag) {
        if (m_series[0]->count() == MaxCount)
            m_minX = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(m_series[0]->at(0).x()));

        graphicsView->chart()->axes(Qt::Horizontal).at(0) /*axisX()*/->setRange(m_minX, m_keyX);
        graphicsView->chart()->axes(Qt::Vertical).at(0) /*axisY()*/->setRange(minY, maxY);
    }
    //    qDebug() << "GetMeasuredValueSlot" << timer.elapsed();
    m_keyX = QDateTime::currentDateTime();
    m_semaphore.release();
}

void Measurements::showEvent(QShowEvent* /*event*/)
{
    if (mi::man->isConnected()) {
        MeasuredValue value;
        if (mi::man->getMeasuredValue(value, 0)) {
            m_disableSlots = true;
            dsbVoltage_1->setValue(static_cast<double>(value.ch1));
            dsbVoltage_2->setValue(static_cast<double>(value.ch2));
            dsbVoltage_3->setValue(static_cast<double>(value.ch3));
            dsbSetCurrent_1->setValue(static_cast<double>(value.ch3) * 1000);

            pbCurrent_1->setChecked(value.manState.load);
            pbCurrent_1->setText(value.manState.load ? "Выкл." : "Вкл.");

            pbShort_1->setChecked(value.manState.shortCircuit);
            pbShort_1->setText(value.manState.shortCircuit ? "Выкл." : "Вкл.");

            pbOsc_1->setChecked(value.manState.oscilloscope);
            pbOsc_1->setText(value.manState.oscilloscope ? "Выкл." : "Вкл.");

            m_disableSlots = false;
        }
        connect(this, &Measurements::StartMeasure, mi::man, &MAN2::startMeasure);
        connect(mi::man, &MAN2::measureCompleted, this, &Measurements::GetMeasuredValueSlot);
        setEnabled(true);
        on_cbxValType_currentIndexChanged(0);
        on_pbStart_clicked(true);
        return;
    }
    setEnabled(false);
}

void Measurements::hideEvent(QHideEvent* /*event*/)
{
    on_pbStart_clicked(false);
    disconnect(this, &Measurements::StartMeasure, mi::man, &MAN2::startMeasure);
    disconnect(mi::man, &MAN2::measureCompleted, this, &Measurements::GetMeasuredValueSlot);
}

void Measurements::on_pbClear_clicked()
{
    QMutexLocker locker(&m_mutex);
    m_keyX = m_minX = QDateTime();
    for (QAbstractSeries* ser : graphicsView->chart()->series())
        static_cast<QLineSeries*>(ser)->clear();
    minY = +std::numeric_limits<double>::max();
    maxY = -std::numeric_limits<double>::max();
}

void Measurements::on_cbxValType_currentIndexChanged(int index)
{
    switch (index) {
    case CurrentMeasuredValue:
        dsbVoltage_1->setSuffix(" B");
        dsbVoltage_2->setSuffix(" mA");
        dsbVoltage_3->setSuffix(" mA(уст)");
        break;

    case ValueTripCurrent:
        dsbVoltage_1->setSuffix(" Нет");
        dsbVoltage_2->setSuffix(" Нет");
        dsbVoltage_3->setSuffix(" Нет");
        break;
    case CalibVoltage:
        dsbVoltage_1->setSuffix(" B(1)");
        dsbVoltage_2->setSuffix(" B(2)");
        dsbVoltage_3->setSuffix(" Нет");
        break;
    case CalibCurrent:
        dsbVoltage_1->setSuffix(" mA");
        dsbVoltage_2->setSuffix(" Нет");
        dsbVoltage_3->setSuffix(" Нет");
        break;
    case RawData:
        dsbVoltage_1->setSuffix(" B(1)");
        dsbVoltage_2->setSuffix(" B(2)");
        dsbVoltage_3->setSuffix(" mА");
        break;
    default:
        break;
    }
}
