#ifndef GRADUATION_H
#define GRADUATION_H

#include "hw/man2.h"
#include "ui_graduation.h"

class MainWindow;

class Graduation : public QWidget, private Ui::Graduation {
    Q_OBJECT
    friend class MainWindow;

public:
    explicit Graduation(QWidget* parent = nullptr);

    ~Graduation();

private:
    uint8_t m_channel = 0;
    QVector<QCheckBox*> m_chbxs;

    MeasuredValue value;
    GradCoeff gradCoeff;
    const int count = 11;

    void gradI();
    void gradU();
    void gradUAdc();
    void setGradDsbxs();

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);

private slots:
    void on_pbStartGrad_clicked();
    void on_pbGet_clicked();
    void on_pbSet_clicked();
    void on_pbSetDefault_clicked();
};

#endif // GRADUATION_H
