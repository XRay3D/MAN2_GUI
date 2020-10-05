#include "lintest.h"
#include "ui_lintest.h"

#include <QThread>
#include <hw/interface.h>

LinTest::LinTest(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LinTest)
{
    ui->setupUi(this);
    cv1 = new QChartView(this);
    cv2 = new QChartView(this);
    ui->gridLayout->addWidget(cv1, 3, 0, 1, 0);
    ui->gridLayout->addWidget(cv2, 4, 0, 1, 0);

    setup(cv1, m_series, { "Setted", "Man", "Scpi" });
    setup(cv2, m_series2, { "", "Man", "Scpi" });

    connect(&timer, &QTimer::timeout, this, &LinTest::measure);
    connect(ui->dsbxCurrent, qOverload<double>(&QDoubleSpinBox::valueChanged), [](double val) {
        mi::man->setCurrent(val);
    });
}

LinTest::~LinTest()
{
    delete ui;
}

void LinTest::measure()
{
    if (m_mutex.tryLock())
        m_mutex.unlock();
    else
        return;

    QMutexLocker locker(&m_mutex);
    enum { MaxCount = 5000 };
    if (m_minX == m_keyX)
        m_minX = m_keyX = QDateTime::currentDateTime();

    double minY = +std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    double minY2 = +std::numeric_limits<double>::max();
    double maxY2 = -std::numeric_limits<double>::max();

    bool flag = false;

    MeasuredValue list;
    mi::man->getMeasuredValue(list, mi::man->address());

    //    double rms = 0.0;
    //    rms = mi::man->getRmsValue();
    //    dsbVoltage_9->setValue(rms);

    //    iterator.next();
    //    int i = iterator.key() - 1;
    //    m_listDsbVoltage[i]->setValue(iterator.value().Value1);
    //    m_listDsbCurrent[i]->setValue(iterator.value().Value2);

    ui->dsbxMeasMan->setValue(list.ch2);
    ui->dsbxMeasScpi->setValue(mi::scpi->GetDcCurrent());

    //    dsbVoltage_1->setValue(static_cast<double>(list.Value1));
    //    dsbVoltage_2->setValue(static_cast<double>(list.Value2));
    //    dsbVoltage_3->setValue(static_cast<double>(list.Value3));

    m_series[0]->append(m_keyX.toMSecsSinceEpoch(), list.ch3 * 1000 /*ui->dsbxCurrent->value()*/);
    m_series[1]->append(m_keyX.toMSecsSinceEpoch(), ui->dsbxMeasMan->value() * 1000);
    m_series[2]->append(m_keyX.toMSecsSinceEpoch(), ui->dsbxMeasScpi->value() * 1000);

    m_series2[1]->append(m_keyX.toMSecsSinceEpoch(), ui->dsbxMeasMan->value() * 1000 - list.ch3 * 1000);
    m_series2[2]->append(m_keyX.toMSecsSinceEpoch(), ui->dsbxMeasScpi->value() * 1000 - list.ch3 * 1000);
    //    m_series2[2]->append(m_keyX.toMSecsSinceEpoch(), ui->dsbxMeasScpi->value());

    for (auto var : m_series) {
        if (var->count() > MaxCount)
            var->remove(0);

        if (var->isVisible()) {
            for (QPointF& p : var->points()) {
                minY = qMin(minY, p.y());
                maxY = qMax(maxY, p.y());
            }
            flag = true;
        }
    }

    for (auto var : m_series2) {
        if (var->count() > MaxCount)
            var->remove(0);
        if (var->isVisible()) {
            for (QPointF& p : var->points()) {
                minY2 = qMin(minY2, p.y());
                maxY2 = qMax(maxY2, p.y());
            }
            flag = true;
        }
    }

    if (flag) {
        if (m_series[0]->count() == MaxCount)
            m_minX = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(m_series[0]->at(0).x()));

        cv1->chart()->axes(Qt::Horizontal).at(0) /*axisX()*/->setRange(m_minX, m_keyX);
        cv1->chart()->axes(Qt::Vertical).at(0) /*axisY()*/->setRange(minY, maxY);

        cv2->chart()->axes(Qt::Horizontal).at(0) /*axisX()*/->setRange(m_minX, m_keyX);
        cv2->chart()->axes(Qt::Vertical).at(0) /*axisY()*/->setRange(minY2, maxY2);
    }

    //    qDebug() << "GetMeasuredValueSlot" << timer.elapsed();
    m_keyX = QDateTime::currentDateTime();
    ui->dsbxCurrent->setValue(ui->dsbxCurrent->value() + ui->dsbxStep->value());
}

void LinTest::setup(QChartView* cv, QList<QLineSeries*>& series, const QList<QString>& names)
{

    QChart* chart = new QChart();
    QValueAxis* axisY = new QValueAxis;
    QDateTimeAxis* axisX = new QDateTimeAxis;

    { // axisY
        QPen pen(axisY->gridLinePen().color(), 0.0, Qt::DotLine);
        //        axisY->setMinorGridLinePen(pen);
        //        axisY->setMinorTickCount(4);
        chart->addAxis(axisY, Qt::AlignLeft);
    }

    { // axisX
        axisX->setFormat("h:mm:ss");
        axisX->setTickCount(7);
        chart->addAxis(axisX, Qt::AlignBottom);
    }

    for (int i = 0; i < 3; ++i) {
        series.append(new QLineSeries(chart));
        QPen pen(Qt::black, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen.setColor(QColor::fromHsv((360.0 / 3.0) * i, 255, 255));
        series.last()->setPen(pen);
        series.last()->setName(names.value(i) /*QString("Канал %1, В.").arg(i + 1)*/);
        chart->addSeries(series.last());
        series.last()->attachAxis(axisY);
        series.last()->attachAxis(axisX);
    }

    //    chart->createDefaultAxes();
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundBrush(Qt::white);

    cv->setChart(chart);
    cv->setRenderHint(QPainter::Antialiasing);
    cv->setPalette(QPalette(Qt::white));
}

void LinTest::on_pushButton_clicked(bool checked)
{
    if (checked) {
        for (auto& var : m_series)
            var->clear();
        for (auto& var : m_series2)
            var->clear();
        m_minX = m_keyX;
        ui->dsbxCurrent->setValue(0);
        mi::man->switchCurrent(1);
        mi::man->setCurrent(0);
        mi::man->oscilloscope(mi::man->address());
        timer.start(2000);
    } else {
        mi::man->switchCurrent(0);
        mi::man->oscilloscope(0);
        timer.stop();
    }
}
