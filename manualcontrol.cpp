#include "manualcontrol.h"
#include "formchannel.h"
#include "hw/interface.h"
#include "settings.h"
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QEvent>
#include <QLineSeries>
#include <QValueAxis>
#include <limits>

QT_CHARTS_USE_NAMESPACE

ManualControl::ManualControl(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    graphicsView = new QChartView(this);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    verticalLayout_11->addWidget(graphicsView, 1);
    gridLayout->setSpacing(6);
    for (int i = 0; i < ManCount; ++i) {
        auto ch = new FormChannel(this);
        m_listGroupBox[i] = ch;
        gridLayout->addWidget(ch, 0, 2 + i);
        ch->setObjectName("gbChannel_" + QString::number(i + 1));
        ch->setTitle(ch->title() + " " + QString::number(i + 1));
        for (auto obj : ch->children()) {
            obj->setObjectName(objectName() + QString::number(i + 1));
        }
        m_listDsbCurrent[i] = ch->dsbCurrent_;
        m_listDsbSetCurrent[i] = ch->dsbSetCurrent_;
        m_listDsbVoltage[i] = ch->dsbVoltage_;
        m_listPbCurrent[i] = ch->pbCurrent_;
        m_listPbOsc[i] = ch->pbOsc_;
        m_listPbShort[i] = ch->pbShort_;

        connect(m_listDsbSetCurrent[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), [ch = i, this] { dsbSetCurrent(ch); });
        connect(m_listGroupBox[i], &QGroupBox::toggled, [ch = i, this] { gbChanneClicked(ch); });
        connect(m_listPbCurrent[i], &QPushButton::clicked, [ch = i, this] { bbCurrentClicked(ch); });
        connect(m_listPbOsc[i], &QPushButton::clicked, [ch = i, this] { obOscClicked(ch); });
        connect(m_listPbShort[i], &QPushButton::clicked, [ch = i, this] { pbShortClicked(ch); });
    }

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
    for (int i = 0; i < ManCount; ++i) {
        QLineSeries* series = new QLineSeries(chart);
        m_series[i] = series;
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

    MySettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("ManualControl");
    for (auto groupBox : m_listGroupBox)
        settings.getValue(groupBox, true);
    settings.endGroup();

    connect(&m_timerMeasure, &QTimer::timeout, [this] {
        //        if (m_semaphore.tryAcquire()) {
        //            m_semaphore.acquire(m_semaphore.available());
        startMeasure();
        //        }
    });
}

ManualControl::~ManualControl()
{
    MySettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("ManualControl");
    for (auto groupBox : m_listGroupBox)
        settings.setValue(groupBox);
    settings.endGroup();
}

void ManualControl::on_cbOsc_currentIndexChanged(int index)
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
    mi::man()->oscilloscope(index);
}

void ManualControl::on_pbStart_clicked(bool checked)
{
    pbStart->setChecked(checked);
    if (checked) {
        m_timerMeasure.start(sbTimeout->value());
        pbStart->setText("Стоп");
        //        m_semaphore.release();
        rangeX.reset();
        for (auto& range : rangeY)
            range.reset();
    } else {
        m_timerMeasure.stop();
        pbStart->setText("Старт");
    }
}

void ManualControl::on_sbTimeout_valueChanged(int arg1)
{
    m_timerMeasure.setInterval(arg1);
}

void ManualControl::on_dsbSetCurrentAll_valueChanged(double arg1)
{
    m_disableSlots = true;
    for (QDoubleSpinBox* spinBox : m_listDsbSetCurrent)
        spinBox->setValue(arg1);
    mi::man()->setCurrent(arg1);
    m_disableSlots = false;
}

void ManualControl::on_pbCurrentAll_clicked(bool checked)
{
    for (QPushButton* button : m_listPbCurrent) {
        button->setChecked(checked);
        button->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man()->switchCurrent(checked);
}

void ManualControl::on_pbShortAll_clicked(bool checked)
{
    for (QPushButton* button : m_listPbShort) {
        button->setChecked(checked);
        button->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man()->switchShortCircuit(checked ? ScShunt : ScOff);
}

void ManualControl::bbCurrentClicked(int channel)
{
    QPushButton* btn = m_listPbCurrent[channel];
    bool checked = btn->isChecked();
    if (checked)
        btn->setText("Выкл.");
    else
        btn->setText("Вкл.");

    mi::man()->switchCurrent(checked, channel + 1);
}

void ManualControl::dsbSetCurrent(int channel)
{
    if (m_disableSlots)
        return;
    double value = m_listDsbSetCurrent[channel]->value();
    mi::man()->setCurrent(value, channel + 1);
}

void ManualControl::pbShortClicked(int channel)
{
    QPushButton* btn = m_listPbShort[channel];
    bool checked = btn->isChecked();
    if (checked)
        btn->setText("Выкл.");
    else
        btn->setText("Вкл.");

    mi::man()->shortCircuitTest(checked ? ScShunt : ScOff, channel + 1);
}

void ManualControl::obOscClicked(int channel)
{
    QPushButton* btn = m_listPbOsc[channel];
    if (btn->isChecked())
        cbOsc->setCurrentIndex(channel + 1);
    else
        cbOsc->setCurrentIndex(0);
}

void ManualControl::measuredValueSlot(const MeasureMap& valMap)
{
    enum { MaxCount = 500 };
    rangeX = QDateTime::currentDateTime();

    MinMax<float> range;

    for (auto [key, value] : valMap) {
        if (key == 10) {
            dsbVoltage_9->setValue(value.valCh1);
            continue;
        }
        if (key > ManCount) {
            continue;
        }
        int i = key - 1;

        m_listDsbVoltage[i]->setValue(value.valCh1);
        m_listDsbCurrent[i]->setValue(value.valCh2);

        m_series[i]->append(rangeX.max.toMSecsSinceEpoch(), value.valCh1);
        rangeY[i] = value.valCh1;
        if (m_listGroupBox[i]->isChecked()) {
            range = rangeY[i].min;
            range = rangeY[i].max;
        }
    }

    graphicsView->chart()->axes(Qt::Horizontal).at(0) /*axisX()*/->setRange(rangeX.min, rangeX.max);
    graphicsView->chart()->axes(Qt::Vertical).at(0) /*axisY()*/->setRange(range.min, range.max);
}

void ManualControl::showEvent(QShowEvent* /*event*/)
{
    if (mi::man()->isConnected()) {
        MeasureMap list;
        qWarning("Fix i");
        if (mi::man()->getMeasuredValue(list)) {
            m_disableSlots = true;
            for (int i = 0; i < ManCount; ++i) {
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
            dsbVoltage_9->setValue(list[9].valCh1);
            m_disableSlots = false;
        }
        connect(this, &ManualControl::startMeasure, mi::man(), &MAN2::startMeasure);
        connect(mi::man(), &MAN2::measuresCompleted, this, &ManualControl::measuredValueSlot);
        on_pbStart_clicked(true);
        return;
    }
    setEnabled(false);
}

void ManualControl::hideEvent(QHideEvent* /*event*/)
{
    on_pbStart_clicked(false);
    disconnect(this, &ManualControl::startMeasure, mi::man(), &MAN2::startMeasure);
    disconnect(mi::man(), &MAN2::measuresCompleted, this, &ManualControl::measuredValueSlot);
}

void ManualControl::gbChanneClicked(int channel)
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

void ManualControl::on_pbClear_clicked()
{
    rangeX.reset();
    for (QAbstractSeries* ser : graphicsView->chart()->series())
        static_cast<QLineSeries*>(ser)->clear();
}
