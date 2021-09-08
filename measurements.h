#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include "hw/man2.h"
#include "ui_measurements.h"
#include <QDate>

namespace QtCharts {
class QChartView;
class QLineSeries;
}

class FormChannel;

class Measurements : public QWidget, private Ui::Measurements {
    Q_OBJECT

public:
    explicit Measurements(QWidget* parent = nullptr);
    ~Measurements();

private slots:
    void on_cbOsc_currentIndexChanged(int index);
    void on_pbStart_clicked(bool checked);
    void on_sbTimeout_valueChanged(int arg1);

    void on_dsbSetCurrentAll_valueChanged(double arg1);
    void on_pbCurrentAll_clicked(bool checked);
    void on_pbShortAll_clicked(bool checked);

    void dsbSetCurrent(int channel);
    void bbCurrentClicked(int channel);
    void pbShortClicked(int channel);
    void obOscClicked(int channel);

    void measuredValueSlot(const QMap<int, MeasuredValue>& valMap);

    void gbChanneClicked(int channel);

    void on_pbClear_clicked();

signals:
    void startMeasure(ValueType type = CurrentMeasuredValue, uint8_t channel = 0);

private:
    QVector<FormChannel*> m_listGroupBox;
    QVector<QDoubleSpinBox*> m_listDsbCurrent;
    QVector<QDoubleSpinBox*> m_listDsbSetCurrent;
    QVector<QDoubleSpinBox*> m_listDsbVoltage;
    QVector<QPushButton*> m_listPbCurrent;
    QVector<QPushButton*> m_listPbOsc;
    QVector<QPushButton*> m_listPbShort;

    QVector<QtCharts::QLineSeries*> m_series;
    int m_timerMeasure;
    bool m_disableSlots = false;

    QDateTime m_keyX;
    QDateTime m_minX;

    QtCharts::QChartView* graphicsView;

    QSemaphore m_semaphore;
    QMutex m_mutex;

protected:
    // QObject interface
    void timerEvent(QTimerEvent* event) override;
    // QWidget interface
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
};

#endif // MEASUREMENTS_H
