#include "triptest.h"
#include "hw/interface.h"
#include "ui_triptest.h"

TripTest::TripTest(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TripTest)
{
    ui->setupUi(this);
    chvw = new QChartView(this);
    ui->verticalLayout->addWidget(chvw);
    setup(chvw, series, { "Setted", "Man", "Scpi" });

    connect(this, &TripTest::startTest, mi::man, &MAN2::startTest);
    connect(mi::man, &MAN2::measCompleted, this, &TripTest::measCompleted);

    QSettings s;
    s.beginGroup("TripTest");
    ui->dsbxStart->setValue(s.value("dsbxStart", 0).toDouble());
    ui->dsbxStop->setValue(s.value("dsbxStop", 1000).toDouble());
    ui->dsbxStep->setValue(s.value("dsbxStep", 10).toDouble());
    s.endGroup();
}

TripTest::~TripTest()
{
    QSettings s;
    s.beginGroup("TripTest");
    s.setValue("dsbxStart", ui->dsbxStart->value());
    s.setValue("dsbxStop", ui->dsbxStop->value());
    s.setValue("dsbxStep", ui->dsbxStep->value());
    s.endGroup();
    delete ui;
}

void TripTest::setup(QChartView* cv, QList<QLineSeries*>& series, const QList<QString>& names)
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

void TripTest::measCompleted(const MeasuredValue& list)
{
    if (!list.manState.TripCurrentTest) {
        return;
    }
    QMutexLocker locker(&m_mutex);
    enum { MaxCount = 5000 };
    if (m_minX == m_keyX)
        m_minX = m_keyX = QDateTime::currentDateTime();

    double minY = +std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();

    bool flag = false;

    series[0]->append(m_keyX.toMSecsSinceEpoch(), list.value1);
    series[1]->append(m_keyX.toMSecsSinceEpoch(), list.value2);
    series[2]->append(m_keyX.toMSecsSinceEpoch(), list.value3);

    for (auto var : series) {
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

    if (flag) {
        if (series[0]->count() == MaxCount)
            m_minX = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(series[0]->at(0).x()));

        chvw->chart()->axes(Qt::Horizontal).at(0) /*axisX()*/->setRange(m_minX, m_keyX);
        chvw->chart()->axes(Qt::Vertical).at(0) /*axisY()*/->setRange(minY, maxY);
    }

    //    qDebug() << "GetMeasuredValueSlot" << timer.elapsed();
    m_keyX = QDateTime::currentDateTime();
    ui->pushButton->setEnabled(list.value3 > ui->dsbxStop->value());
}

void TripTest::on_pushButton_clicked()
{
    m_minX = m_keyX = QDateTime::currentDateTime();
    series[0]->clear();
    series[1]->clear();
    series[2]->clear();

    emit startTest(ui->dsbxStart->value(), ui->dsbxStop->value(), ui->dsbxStep->value(), mi::man->address());
    ui->pushButton->setEnabled(false);
}
