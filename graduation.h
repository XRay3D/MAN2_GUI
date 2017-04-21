#ifndef GRADUATION_H
#define GRADUATION_H

#include "ui_graduation.h"
#include "maninterface.h"
#include "scpi.h"

class Graduation : public QWidget, private Ui::Graduation {
    Q_OBJECT

public:
    explicit Graduation(QWidget* parent = 0);

    ~Graduation();
    void setMan(ManInterface* value);

private:
    ManInterface* m_man;
    SCPI m_scpi;
    int m_channel = 0;

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);

private slots:
    void on_cbTestShortCirc_clicked(bool checked);
    void on_pbOscilloscope_clicked(bool checked);
    void on_pbCurrentEnable_clicked(bool checked);
    void on_pbStartGrad_clicked();
    void on_dsbSetCurrent_valueChanged(double arg1);
    void on_pbMeasure_clicked();
    void on_pbGradRead_clicked();
    void on_pbCheckConnectionScpi_clicked();
    void on_pbSave_clicked();
    void on_pbLoad_clicked();
    void on_cbChannelMan_currentIndexChanged(int index);
};

#endif // GRADUATION_H
