#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include "hw/man2.h"
#include "ui_manualcontrol.h"
#include <QDate>
#include <QTimer>
namespace QtCharts {
class QChartView;
class QLineSeries;
}

class FormChannel;

template <class T>
struct MinMax {
    T min {};
    T max {};
    void reset() { min = max = T {}; }
    MinMax& operator=(const T& val)
    {
        if (min == max && min == T {}) {
            min = max = val;
        } else {
            min = std::min(min, val);
            max = std::max(max, val);
        }
        return *this;
    }
};

class ManualControl : public QWidget, private Ui::ManualControl {
    Q_OBJECT

public:
    explicit ManualControl(QWidget* parent = nullptr);
    ~ManualControl();

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

    void measuredValueSlot(const MeasureMap& valMap);

    void gbChanneClicked(int channel);

    void on_pbClear_clicked();

signals:
    void startMeasure(ValueType type = CurrentMeasuredValue, uint8_t channel = 0);

private:
    std::array<FormChannel*, CHCNT> m_listGroupBox;

    std::array<QtCharts::QLineSeries*, CHCNT> m_series;

    QTimer m_timerMeasure;
    bool m_disableSlots = false;

    MinMax<QDateTime> rangeX;
    MinMax<float> rangeY[CHCNT];

    QtCharts::QChartView* graphicsView;

protected:
    // QWidget interface
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
};

#endif // MEASUREMENTS_H
