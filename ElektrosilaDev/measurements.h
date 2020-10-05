#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include "hw/man2.h"
#include "ui_measurements.h"

#include <QDate>

namespace QtCharts {
class QLineSeries;
class QChartView;
}

class Measurements : public QWidget, private Ui::Measurements {
    Q_OBJECT

public:
    explicit Measurements(QWidget* parent = 0);

private slots:
    void on_pbStart_clicked(bool checked);
    void on_sbTimeout_valueChanged(int arg1);

    //    void on_dsbSetCurrentAll_valueChanged(double arg1);
    //    void on_pbCurrentAll_clicked(bool checked);
    //    void on_pbShortAll_clicked(bool checked);

    void GetMeasuredValueSlot(const MeasuredValue& list);

    void on_pbClear_clicked();

    void on_cbxValType_currentIndexChanged(int index);

signals:
    void StartMeasure(ValueType type = CurrentMeasuredValue, uint8_t channel = 0);

private:
    QList<QtCharts::QLineSeries*> m_series;
    QtCharts::QChartView* graphicsView;

    int m_timerMeasure;
    bool m_disableSlots = false;
    QDateTime m_keyX;
    QDateTime m_minX;

    QSemaphore m_semaphore;
    QMutex m_mutex;

    double minY = +std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event);

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
};

#endif // MEASUREMENTS_H
