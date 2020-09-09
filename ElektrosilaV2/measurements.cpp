#include "measurements.h"
#include "hw/interface.h"
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QEvent>
#include <QLineSeries>
#include <QSettings>
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
    verticalLayout_11->addWidget(graphicsView, 1);
    gridLayout->setSpacing(6);

    m_listPbCurrent = { pbCurrent_1, pbCurrent_2, pbCurrent_3, pbCurrent_4, pbCurrent_5, pbCurrent_6, pbCurrent_7, pbCurrent_8 };
    m_listPbShort = { pbShort_1, pbShort_2, pbShort_3, pbShort_4, pbShort_5, pbShort_6, pbShort_7, pbShort_8 };
    m_listPbOsc = { pbOsc_1, pbOsc_2, pbOsc_3, pbOsc_4, pbOsc_5, pbOsc_6, pbOsc_7, pbOsc_8 };
    m_listDsbVoltage = { dsbVoltage_1, dsbVoltage_2, dsbVoltage_3, dsbVoltage_4, dsbVoltage_5, dsbVoltage_6, dsbVoltage_7, dsbVoltage_8 };
    m_listDsbCurrent = { dsbCurrent_1, dsbCurrent_2, dsbCurrent_3, dsbCurrent_4, dsbCurrent_5, dsbCurrent_6, dsbCurrent_7, dsbCurrent_8 };
    m_listDsbSetCurrent = { dsbSetCurrent_1, dsbSetCurrent_2, dsbSetCurrent_3, dsbSetCurrent_4, dsbSetCurrent_5, dsbSetCurrent_6, dsbSetCurrent_7, dsbSetCurrent_8 };
    m_listGroupBox = { gbChannel_1, gbChannel_2, gbChannel_3, gbChannel_4, gbChannel_5, gbChannel_6, gbChannel_7, gbChannel_8 };

    for (QPushButton* pbtn : m_listPbCurrent)
        connect(pbtn, &QPushButton::clicked, [=] { bbCurrentClicked(m_listPbCurrent.indexOf(pbtn)); });

    for (QPushButton* pbtn : m_listPbShort)
        connect(pbtn, &QPushButton::clicked, [=] { pbShortClicked(m_listPbShort.indexOf(pbtn)); });

    for (QPushButton* pbtn : m_listPbOsc)
        connect(pbtn, &QPushButton::clicked, [=]() { obOscClicked(m_listPbOsc.indexOf(pbtn)); });

    for (QDoubleSpinBox* dsbx : m_listDsbSetCurrent)
        connect(dsbx, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=] { dsbSetCurrent(m_listDsbSetCurrent.indexOf(dsbx)); });

    for (QGroupBox* grbx : m_listGroupBox)
        connect(grbx, &QGroupBox::toggled, [=]() { gbChanneClicked(m_listGroupBox.indexOf(grbx)); });

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
    for (int i = 0; i < 8; ++i) {
        QLineSeries* series = new QLineSeries(chart);
        m_series.append(series);
        QPen pen(QBrush(), 2.0);
        pen.setColor(QColor::fromHsv((360.0 / 8.0) * i, 255, 255));
        //        QPen pen(QBrush(color[i]), 0.0);
        series->setPen(pen);
        series->setName(QString("Канал %1, В.").arg(i + 1));
        chart->addSeries(series);
        series->attachAxis(axisY);
        series->attachAxis(axisX);
    }
    //    chart->createDefaultAxes();
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setMargins(QMargins(3, 3, 3, 3));
    chart->setBackgroundBrush(Qt::darkGray);

    graphicsView->setChart(chart);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setPalette(QPalette(Qt::white));

    QSettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("Measurements");
    gbChannel_1->setChecked(settings.value("gbChannel_1", true).toBool());
    gbChannel_2->setChecked(settings.value("gbChannel_2", true).toBool());
    gbChannel_3->setChecked(settings.value("gbChannel_3", true).toBool());
    gbChannel_4->setChecked(settings.value("gbChannel_4", true).toBool());
    gbChannel_5->setChecked(settings.value("gbChannel_5", true).toBool());
    gbChannel_6->setChecked(settings.value("gbChannel_6", true).toBool());
    gbChannel_7->setChecked(settings.value("gbChannel_7", true).toBool());
    gbChannel_8->setChecked(settings.value("gbChannel_8", true).toBool());
}

Measurements::~Measurements()
{
    QSettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("Measurements");
    settings.setValue("gbChannel_1", gbChannel_1->isChecked());
    settings.setValue("gbChannel_2", gbChannel_2->isChecked());
    settings.setValue("gbChannel_3", gbChannel_3->isChecked());
    settings.setValue("gbChannel_4", gbChannel_4->isChecked());
    settings.setValue("gbChannel_5", gbChannel_5->isChecked());
    settings.setValue("gbChannel_6", gbChannel_6->isChecked());
    settings.setValue("gbChannel_7", gbChannel_7->isChecked());
    settings.setValue("gbChannel_8", gbChannel_8->isChecked());
}

void Measurements::on_cbOsc_currentIndexChanged(int index)
{
    //    cbOsc->setCurrentIndex(index);
    for (int i = 0; i < m_listPbOsc.size(); ++i) {
        if (i == index - 1) {
            m_listPbOsc[i]->setChecked(true);
            m_listPbOsc[i]->setText("Выкл.");
        } else {
            m_listPbOsc[i]->setChecked(false);
            m_listPbOsc[i]->setText("Вкл.");
        }
    }
    mi::man->oscilloscope(index);
}

void Measurements::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerMeasure) {
        QMutexLocker locker(&m_mutex);
        if (m_semaphore.tryAcquire()) {
            m_semaphore.acquire(m_semaphore.available());
            startMeasure();
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

void Measurements::on_dsbSetCurrentAll_valueChanged(double arg1)
{
    m_disableSlots = true;
    for (QDoubleSpinBox* spinBox : m_listDsbSetCurrent)
        spinBox->setValue(arg1);
    mi::man->setCurrent(arg1);
    m_disableSlots = false;
}

void Measurements::on_pbCurrentAll_clicked(bool checked)
{
    foreach (QPushButton* button, m_listPbCurrent) {
        button->setChecked(checked);
        button->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man->switchCurrent(checked);
}

void Measurements::on_pbShortAll_clicked(bool checked)
{
    foreach (QPushButton* button, m_listPbShort) {
        button->setChecked(checked);
        button->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man->shortCircuitTest(checked);
}

void Measurements::bbCurrentClicked(int channel)
{
    QPushButton* btn = m_listPbCurrent[channel];
    bool checked = btn->isChecked();
    if (checked)
        btn->setText("Выкл.");
    else
        btn->setText("Вкл.");

    mi::man->switchCurrent(checked, channel + 1);
}

void Measurements::dsbSetCurrent(int channel)
{
    if (m_disableSlots)
        return;
    double value = m_listDsbSetCurrent[channel]->value();
    mi::man->setCurrent(value, channel + 1);
}

void Measurements::pbShortClicked(int channel)
{
    QPushButton* btn = m_listPbShort[channel];
    bool checked = btn->isChecked();
    if (checked)
        btn->setText("Выкл.");
    else
        btn->setText("Вкл.");

    mi::man->shortCircuitTest(checked, channel + 1);
}

void Measurements::obOscClicked(int channel)
{
    QPushButton* btn = m_listPbOsc[channel];
    if (btn->isChecked())
        cbOsc->setCurrentIndex(channel + 1); //   on_cbOsc_currentIndexChanged(channel + 1);
    else
        cbOsc->setCurrentIndex(0); //        on_cbOsc_currentIndexChanged(0);
}

void Measurements::measuredValueSlot(const QMap<int, MeasuredValue>& list)
{
    QMutexLocker locker(&m_mutex);
    qDebug() << list;
    enum { MaxCount = 500 };
    if (m_minX == m_keyX)
        m_minX = m_keyX = QDateTime::currentDateTime();

    //    QElapsedTimer timer;
    //    timer.start();

    QMapIterator<int, MeasuredValue> iterator(list);
    double minY = +std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    bool flag = false;

    //    double rms = mi::man->getRmsValue();
    //    dsbVoltage_9->setValue(rms);
    while (iterator.hasNext()) {
        iterator.next();
        int i = iterator.key() - 1;
        if (i > 7) {
            continue;
            ; ///////////////
        }
        m_listDsbVoltage[i]->setValue(iterator.value().valCh1);
        m_listDsbCurrent[i]->setValue(iterator.value().valCh2);

        m_series[i]->append(m_keyX.toMSecsSinceEpoch(), iterator.value().valCh1);

        if (m_series[i]->count() > MaxCount)
            m_series[i]->remove(0);

        if (m_series[i]->isVisible()) {
            for (QPointF& p : m_series[i]->points()) {
                minY = qMin(minY, p.y());
                maxY = qMax(maxY, p.y());
            }
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

void Measurements::measureCompletedSlot(const MeasuredValue&)
{
    //    QMutexLocker locker(&m_mutex);
    //    enum { MaxCount = 500 };
    //    if (m_minX == m_keyX)
    //        m_minX = m_keyX = QDateTime::currentDateTime();

    //    //    QElapsedTimer timer;
    //    //    timer.start();

    //    QMapIterator<int, MeasuredValue> iterator(list);
    //    double minY = +std::numeric_limits<double>::max();
    //    double maxY = -std::numeric_limits<double>::max();
    //    bool flag = false;

    //    double rms = 0.0;
    //    rms = mi::man->getRmsValue();
    //    dsbVoltage_9->setValue(rms);
    //    while (iterator.hasNext()) {
    //        iterator.next();
    //        int i = iterator.key() - 1;
    //        if (i > 7) {
    //            continue;; ///////////////
    //        }
    //        m_listDsbVoltage[i]->setValue(iterator.value().valCh1);
    //        m_listDsbCurrent[i]->setValue(iterator.value().valCh2);

    //        m_series[i]->append(m_keyX.toMSecsSinceEpoch(), iterator.value().valCh1);

    //        if (m_series[i]->count() > MaxCount)
    //            m_series[i]->remove(0);

    //        if (m_series[i]->isVisible()) {
    //            for (QPointF& p : m_series[i]->points()) {
    //                minY = qMin(minY, p.y());
    //                maxY = qMax(maxY, p.y());
    //            }
    //            flag = true;
    //        }
    //    }
    //    if (flag) {
    //        if (m_series[0]->count() == MaxCount)
    //            m_minX = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(m_series[0]->at(0).x()));

    //        graphicsView->chart()->axes(Qt::Horizontal).at(0) /*axisX()*/->setRange(m_minX, m_keyX);
    //        graphicsView->chart()->axes(Qt::Vertical).at(0) /*axisY()*/->setRange(minY, maxY);
    //    }
    //    //    qDebug() << "GetMeasuredValueSlot" << timer.elapsed();
    //    m_keyX = QDateTime::currentDateTime();
    //    m_semaphore.release();
}

void Measurements::showEvent(QShowEvent* /*event*/)
{
    if (mi::man->isConnected()) {
        QMap<int, MeasuredValue> list;
        qWarning("Fix i");
        if (mi::man->getMeasuredValue(list)) {
            m_disableSlots = true;
            for (int i = 0; i < 8; ++i) {
                const auto& mv = list[i];
                m_listDsbVoltage[i]->setValue(static_cast<double>(mv.valCh1));
                m_listDsbCurrent[i]->setValue(static_cast<double>(mv.valCh2));
                m_listDsbSetCurrent[i]->setValue(static_cast<double>(mv.valCh3));

                m_listPbCurrent[i]->setChecked(mv.manState.load);
                m_listPbCurrent[i]->setText(mv.manState.load ? "Выкл." : "Вкл.");

                m_listPbShort[i]->setChecked(mv.manState.shortCircuit);
                m_listPbShort[i]->setText(mv.manState.shortCircuit ? "Выкл." : "Вкл.");

                m_listPbOsc[i]->setChecked(mv.manState.oscilloscope);
                m_listPbOsc[i]->setText(mv.manState.oscilloscope ? "Выкл." : "Вкл.");
            }
            m_disableSlots = false;
        }
        connect(this, &Measurements::startMeasure, mi::man, &MAN2::startMeasure);
        connect(mi::man, &MAN2::measuresCompleted, this, &Measurements::measuredValueSlot);
        return;
    }
    setEnabled(false);
}

void Measurements::hideEvent(QHideEvent* /*event*/)
{
    on_pbStart_clicked(false);
    disconnect(this, &Measurements::startMeasure, mi::man, &MAN2::startMeasure);
    disconnect(mi::man, &MAN2::measuresCompleted, this, &Measurements::measuredValueSlot);
}

void Measurements::gbChanneClicked(int channel)
{
    graphicsView->chart()->series()[channel]->setVisible(m_listGroupBox[channel]->isChecked());
    m_listPbCurrent[channel]->setEnabled(true);
    m_listPbShort[channel]->setEnabled(true);
    m_listPbOsc[channel]->setEnabled(true);
    m_listDsbVoltage[channel]->setEnabled(true);
    m_listDsbCurrent[channel]->setEnabled(true);
    m_listDsbSetCurrent[channel]->setEnabled(true);
    m_listGroupBox[channel]->setEnabled(true);
}

void Measurements::on_pbClear_clicked()
{
    QMutexLocker locker(&m_mutex);
    m_keyX = m_minX = QDateTime();
    for (QAbstractSeries* ser : graphicsView->chart()->series())
        static_cast<QLineSeries*>(ser)->clear();
}
