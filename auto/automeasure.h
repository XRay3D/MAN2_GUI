#ifndef AUTOMATICMEASUREMENTS_H
#define AUTOMATICMEASUREMENTS_H

#include "hw/man2.h"
#include "scansettings.h"
#include "ui_automeasure.h"
#include <QAxWidget>
#include <QDialog>
#include <QHideEvent>
#include <QTimer>
#include <shdocvw.h>

enum {
    SetInputVoltageUpper,
    SetInputVoltageLower,
    SetInputVoltageNormal,
    NoConnectionWithMan,
    RestoreTheOperationOfChannels,
    PulsationsOnTheChannel_1,
    PulsationsOnTheChannel_2,
    PulsationsOnTheChannel_3,
    PulsationsOnTheChannel_4,
    PulsationsOnTheChannel_5,
    PulsationsOnTheChannel_6,
    PulsationsOnTheChannel_7,
    PulsationsOnTheChannel_8,
    CheckInputVoltage
};

class Worker;
class ManModel;

class AutoMeasure : public QWidget, private Ui::AutoMeasure {
    Q_OBJECT

public:
    explicit AutoMeasure(QWidget* parent = nullptr);
    ~AutoMeasure() override;

signals:
    void SetTabBarEnabled(bool);

private:
    void showMessage(int num);
    void updateProgresBar();
    void endSlot();

    Worker* m_worker = nullptr;
    bool m_doNotSkip[8];

    QVector<QString> m_serNum;

    int m_measureTimerId = 0;
    int m_stage = 0;

    QTimer m_timerRms;

    ManModel* m_model;

private slots:
    void on_pbStartStop_clicked(bool checked);

    // QWidget interface
protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
};

#endif // AUTOMATICMEASUREMENTS_H
