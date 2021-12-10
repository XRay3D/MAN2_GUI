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
        for (auto obj : ch->children())
            obj->setObjectName(objectName() + QString::number(i + 1));

        connect(ch->dsbSetCurrent_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [ch = i, this] { dsbSetCurrent(ch); });
        connect(ch, &QGroupBox::toggled, [ch = i, this] { gbChanneClicked(ch); });
        connect(ch->pbCurrent_, &QPushButton::clicked, [ch = i, this] { bbCurrentClicked(ch); });
        connect(ch->pbOsc_, &QPushButton::clicked, [ch = i, this] { obOscClicked(ch); });
        connect(ch->pbShort_, &QPushButton::clicked, [ch = i, this] { pbShortClicked(ch); });
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

    for (int i = 0; i < m_listGroupBox.size(); ++i) {
        if (i == index - 1) {
            m_listGroupBox[i]->pbOsc_->setChecked(true);
            m_listGroupBox[i]->pbOsc_->setText("Выкл.");
        } else {
            m_listGroupBox[i]->pbOsc_->setChecked(false);
            m_listGroupBox[i]->pbOsc_->setText("Вкл.");
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
    for (auto* grbx : m_listGroupBox)
        grbx->dsbSetCurrent_->setValue(arg1);
    mi::man()->setCurrent(arg1);
    m_disableSlots = false;
}

void ManualControl::on_pbCurrentAll_clicked(bool checked)
{
    for (auto groupBox : m_listGroupBox) {
        groupBox->pbCurrent_->setChecked(checked);
        groupBox->pbCurrent_->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man()->switchCurrent(checked);
}

void ManualControl::on_pbShortAll_clicked(bool checked)
{
    for (auto groupBox : m_listGroupBox) {
        groupBox->pbShort_->setChecked(checked);
        groupBox->pbShort_->setText(checked ? "Выкл." : "Вкл.");
    }
    mi::man()->switchShortCircuit(checked ? ScShunt : ScOff);
}

void ManualControl::bbCurrentClicked(int channel)
{
    QPushButton* btn = m_listGroupBox[channel]->pbCurrent_;
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
    double value = m_listGroupBox[channel]->dsbSetCurrent_->value();
    mi::man()->setCurrent(value, channel + 1);
}

void ManualControl::pbShortClicked(int channel)
{
    QPushButton* btn = m_listGroupBox[channel]->pbShort_;
    bool checked = btn->isChecked();
    if (checked)
        btn->setText("Выкл.");
    else
        btn->setText("Вкл.");

    mi::man()->shortCircuitTest(checked ? ScShunt : ScOff, channel + 1);
}

void ManualControl::obOscClicked(int channel)
{
    QPushButton* btn = m_listGroupBox[channel]->pbOsc_;
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

        m_listGroupBox[i]->dsbVoltage_->setValue(value.valCh1);
        m_listGroupBox[i]->dsbCurrent_->setValue(value.valCh2);

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
                m_listGroupBox[i]->dsbVoltage_->setValue(static_cast<double>(mv.valCh1));
                m_listGroupBox[i]->dsbCurrent_->setValue(static_cast<double>(mv.valCh2));
                m_listGroupBox[i]->dsbSetCurrent_->setValue(static_cast<double>(mv.valCh3));

                m_listGroupBox[i]->pbCurrent_->setChecked(mv.manState.load);
                m_listGroupBox[i]->pbCurrent_->setText(mv.manState.load ? "Выкл." : "Вкл.");

                m_listGroupBox[i]->pbShort_->setChecked(mv.manState.shortCircuit);
                m_listGroupBox[i]->pbShort_->setText(mv.manState.shortCircuit ? "Выкл." : "Вкл.");

                m_listGroupBox[i]->pbOsc_->setChecked(mv.manState.oscilloscope);
                m_listGroupBox[i]->pbOsc_->setText(mv.manState.oscilloscope ? "Выкл." : "Вкл.");
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
    for (auto w : m_listGroupBox[channel]->findChildren<QWidget*>())
        w->setEnabled(true);

    m_listGroupBox[channel]->setEnabled(true);
}

void ManualControl::on_pbClear_clicked()
{
    rangeX.reset();
    for (QAbstractSeries* ser : graphicsView->chart()->series())
        static_cast<QLineSeries*>(ser)->clear();
}
