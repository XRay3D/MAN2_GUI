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
    SET_INPUT_VOLTAGE_0,
    SET_INPUT_VOLTAGE_1,
    SET_INPUT_VOLTAGE_2,
    NO_CONNECTION_WITH_MAN,
    RESTORE_THE_OPERATION_OF_CHANNELS,
    PULSATIONS_ON_THE_CHANNEL_1,
    PULSATIONS_ON_THE_CHANNEL_2,
    PULSATIONS_ON_THE_CHANNEL_3,
    PULSATIONS_ON_THE_CHANNEL_4,
    PULSATIONS_ON_THE_CHANNEL_5,
    PULSATIONS_ON_THE_CHANNEL_6,
    PULSATIONS_ON_THE_CHANNEL_7,
    PULSATIONS_ON_THE_CHANNEL_8,
    TEST_1,
    TEST_2,
    TEST_3,
    TEST_4,
    TEST_5,
    TEST_6,
    TEST_7,
    CHECK_INPUT_VOLTAGE
};

class Worker;
class ManDataModel;

class AutomaticMeasurements : public QWidget, private Ui::AutomaticMeasurements {
    Q_OBJECT

public:
    explicit AutomaticMeasurements(QWidget* parent = 0);
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

    ManDataModel* m_model;

private slots:
    void on_pbStartStop_clicked(bool checked);

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
};

#endif // AUTOMATICMEASUREMENTS_H
