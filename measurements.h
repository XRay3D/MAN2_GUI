#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include "hw/man2.h"
#include "ui_measurements.h"

#include <QDate>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class Measurements : public QWidget, private Ui::Measurements {
    Q_OBJECT

public:
    explicit Measurements(QWidget* parent = 0);

private slots:
    void on_cbOsc_currentIndexChanged(int index);
    void on_pbStart_clicked(bool checked);
    void on_sbTimeout_valueChanged(int arg1);

    void on_dsbSetCurrentAll_valueChanged(double arg1);
    void on_pbCurrentAll_clicked(bool checked);
    void on_pbShortAll_clicked(bool checked);

    void DsbSetCurrent(int channel);
    void PbCurrentClicked(int channel);
    void PbShortClicked(int channel);
    void PbOscClicked(int channel);

    void GetMeasuredValueSlot(const QMap<int, MeasuredValue>& list);

    void GbChanneClicked(int channel);

    void on_pbClear_clicked();

signals:
    void StartMeasure(ValuetypeEnum type = CurrentMeasuredValue, uint8_t channel = 0);
    //public:
    //    bool eventFilter(QObject* watched, QEvent* event);

private:
    QList<QPushButton*> m_listPbCurrent;
    QList<QPushButton*> m_listPbShort;
    QList<QPushButton*> m_listPbOsc;
    QList<QDoubleSpinBox*> m_listDsbVoltage;
    QList<QDoubleSpinBox*> m_listDsbCurrent;
    QList<QDoubleSpinBox*> m_listDsbSetCurrent;
    QList<QGroupBox*> m_listGroupBox;
    QList<QLineSeries*> m_series;
    int m_timerMeasure;
    bool m_disableSlots = false;
    QDateTime m_keyX;
    QDateTime m_minX;
    //    QSignalMapper* smCurrent;
    //    QSignalMapper* smShort;
    //    QSignalMapper* smOsc;
    //    QSignalMapper* smSetCurrent;
    //    QSignalMapper* smGroupBox;

    QSemaphore m_semaphore;
    QMutex m_mutex;

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event);

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
};

#endif // MEASUREMENTS_H
