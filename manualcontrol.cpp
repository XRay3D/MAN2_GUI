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

    gridLayout->setSpacing(6);
    for (int i = 0; i < ManCount; ++i) {
        auto grbxChannel = grbxChannels[i] = new FormChannel(this);
        grbxChannel->setObjectName("grbxChannel_" + QString::number(i + 1));
        grbxChannel->setTitle(grbxChannel->title() + " " + QString::number(i + 1));
        for (auto obj : grbxChannel->children())
            obj->setObjectName(obj->objectName() + QString::number(i + 1));

        connect(grbxChannel, &QGroupBox::toggled, [ch = i, this] { gbChanneClicked(ch); });
        connect(grbxChannel->dsbSetCurrent_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [ch = i, this] { dsbSetCurrent(ch); });
        connect(grbxChannel->pbCurrent_, &QPushButton::clicked, [ch = i, this] { bbCurrentClicked(ch); });
        connect(grbxChannel->pbOsc_, &QPushButton::clicked, [ch = i, this] { obOscClicked(ch); });
        connect(grbxChannel->pbShort_, &QPushButton::clicked, [ch = i, this] { pbShortClicked(ch); });

        gridLayout->addWidget(grbxChannel, 0, 2 + i);
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

        series->setPen(pen);
        series->setName(QString("Канал %1, В.").arg(i + 1));
        chart->addSeries(series);
        series->attachAxis(axisY);
        series->attachAxis(axisX);
    }

    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setMargins(QMargins(3, 3, 3, 3));
    chart->setBackgroundBrush(Qt::darkGray);

    graphicsView = new QChartView(this);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    graphicsView->setChart(chart);
    graphicsView->setPalette(QPalette(Qt::white));
    graphicsView->setRenderHint(QPainter::Antialiasing);
    verticalLayout_11->addWidget(graphicsView, 1);

    MySettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("ManualControl");
    for (auto groupBox : grbxChannels)
        settings.getValue(groupBox, true);
    settings.endGroup();

    connect(&m_timerMeasure, &QTimer::timeout, [this] {
        startMeasure();
        //        }
    });
}

ManualControl::~ManualControl()
{
    MySettings settings;
    settings.setIniCodec("UTF-8");
    settings.beginGroup("ManualControl");
    for (auto groupBox : grbxChannels)
        settings.setValue(groupBox);
    settings.endGroup();
}

void ManualControl::on_cbOsc_currentIndexChanged(int index)
{

    for (int i = 0; i < grbxChannels.size(); ++i) {
        auto ch = grbxChannels[i];
        if (i == index - 1) {
            ch->pbOsc_->setChecked(true);
            ch->pbOsc_->setText("Выкл.");
        } else {
            ch->pbOsc_->setChecked(false);
            ch->pbOsc_->setText("Вкл.");
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
    for (auto* grbx : grbxChannels)
        grbx->dsbSetCurrent_->setValue(arg1);
    mi::man()->setCurrent(arg1);
    m_disableSlots = false;
}

void ManualControl::on_pbCurrentAll_clicked(bool checked)
{
    for (auto groupBox : grbxChannels) {
        groupBox->pbCurrent_->setChecked(checked);
        groupBox->pbCurrent_->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man()->switchCurrent(checked);
}

void ManualControl::on_pbShortAll_clicked(bool checked)
{
    for (auto groupBox : grbxChannels) {
        groupBox->pbShort_->setChecked(checked);
        groupBox->pbShort_->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man()->switchShortCircuit(checked ? ScShunt : ScOff);
}

void ManualControl::bbCurrentClicked(int channel)
{
    QPushButton* btn = grbxChannels[channel]->pbCurrent_;
    bool checked = btn->isChecked();
    btn->setText(checked ? "Выкл." : "Вкл.");
    mi::man()->switchCurrent(checked, channel + 1);
}

void ManualControl::dsbSetCurrent(int channel)
{
    if (m_disableSlots)
        return;
    double value = grbxChannels[channel]->dsbSetCurrent_->value();
    mi::man()->setCurrent(value, channel + 1);
}

void ManualControl::pbShortClicked(int channel)
{
    QPushButton* btn = grbxChannels[channel]->pbShort_;
    bool checked = btn->isChecked();
    btn->setText(checked ? "Выкл." : "Вкл.");
    mi::man()->shortCircuitTest(checked ? ScShunt : ScOff, channel + 1);
}

void ManualControl::obOscClicked(int channel)
{
    QPushButton* btn = grbxChannels[channel]->pbOsc_;
    cbOsc->setCurrentIndex(btn->isChecked() ? channel + 1 : 0);
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

        auto ch = grbxChannels[i];
        ch->dsbVoltage_->setValue(value.valCh1);
        ch->dsbCurrent_->setValue(value.valCh2);

        m_series[i]->append(rangeX.max.toMSecsSinceEpoch(), value.valCh1);
        rangeY[i] = value.valCh1;
        if (ch->isChecked()) {
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
                auto ch = grbxChannels[i];
                ch->dsbVoltage_->setValue(static_cast<double>(mv.valCh1));
                ch->dsbCurrent_->setValue(static_cast<double>(mv.valCh2));
                ch->dsbSetCurrent_->setValue(static_cast<double>(mv.valCh3));

                ch->pbCurrent_->setChecked(mv.manState.load);
                ch->pbCurrent_->setText(mv.manState.load ? "Выкл." : "Вкл.");

                ch->pbShort_->setChecked(mv.manState.shortCircuit);
                ch->pbShort_->setText(mv.manState.shortCircuit ? "Выкл." : "Вкл.");

                ch->pbOsc_->setChecked(mv.manState.oscilloscope);
                ch->pbOsc_->setText(mv.manState.oscilloscope ? "Выкл." : "Вкл.");
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
    graphicsView->chart()->series()[channel]->setVisible(grbxChannels[channel]->isChecked());
    for (auto w : grbxChannels[channel]->findChildren<QWidget*>())
        w->setEnabled(true);

    grbxChannels[channel]->setEnabled(true);
}

void ManualControl::on_pbClear_clicked()
{
    rangeX.reset();
    for (auto& range : rangeY)
        range.reset();
    for (QAbstractSeries* ser : graphicsView->chart()->series())
        static_cast<QLineSeries*>(ser)->clear();
}
