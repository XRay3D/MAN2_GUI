#ifndef TRIPTEST_H
#define TRIPTEST_H

#include <QDateTime>
#include <QWidget>
#include <QtCharts>
#include <hw/man2.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class TripTest;
}

class TripTestForm : public QWidget {
    Q_OBJECT

public:
    explicit TripTestForm(QWidget* parent = nullptr);
    ~TripTestForm();

signals:
    void startTest(float start, float stop, float step, uint8_t channel);

private slots:
    void on_pushButton_clicked();

private:
    Ui::TripTest* ui;
    QChartView* chvw;
    QList<QLineSeries*> series;
    QMutex m_mutex;
    QDateTime m_keyX;
    QDateTime m_minX;

    void setup(QChartView* cv, QList<QLineSeries*>& series, const QList<QString>& names);
    void measCompleted(const MeasuredValue& list);
};

#endif // TRIPTEST_H
