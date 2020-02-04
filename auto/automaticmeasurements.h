#ifndef AUTOMATICMEASUREMENTS_H
#define AUTOMATICMEASUREMENTS_H

#include "hw/man2.h"
#include "preparation/preparation.h"
#include "ui_automaticmeasurements.h"
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

class AutomaticMeasurements : public QWidget, private Ui::AutomaticMeasurements {
    Q_OBJECT

public:
    explicit AutomaticMeasurements(QWidget* parent = nullptr);
    ~AutomaticMeasurements();

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
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
};

#endif // AUTOMATICMEASUREMENTS_H
