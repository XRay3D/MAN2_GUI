#ifndef LINTEST_H
#define LINTEST_H

#include <QTimer>
#include <QWidget>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class LinTest;
}

class LinTest : public QWidget {
    Q_OBJECT

public:
    explicit LinTest(QWidget* parent = nullptr);
    ~LinTest();

private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::LinTest* ui;

    QChartView* cv1;
    QChartView* cv2;

    QList<QLineSeries*> m_series;
    QList<QLineSeries*> m_series2;

    QMutex m_mutex;
    QTimer timer;

    QDateTime m_keyX;
    QDateTime m_minX;

    void measure();
    void setup(QChartView* cv, QList<QLineSeries*>& series, const QList<QString>& names);
};

#endif // LINTEST_H
