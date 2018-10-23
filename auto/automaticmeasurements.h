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

typedef struct {
    double test1;
    double test2;
    double test3;
    double test4;
    double test5;
    double test6;
    double test7;
} Result_t;

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
    VERIFICATION_IS_COMPLETE,
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

class AutomaticMeasurements : public QWidget, private Ui::AutomaticMeasurements {
    Q_OBJECT

public:
    explicit AutomaticMeasurements(QWidget* parent = 0);
    ~AutomaticMeasurements();
    void ScanSettingsSlot(const ScanSettings* scanSettings);
    void SerialNumberChanged(const QString& serialNumber, int index);

signals:
    void SetTabBarEnabled(bool);

private:
    void ShowMessage(int num);
    void ShowProtocol(int num);
    //    void ItemDoubleClicked(QListWidgetItem* item);
    void UpdateProgresBar();
    void SaveProtokol(const QString& serialNumber, int number);
    void GetMeasuredValueSlot(const QMap<int, MeasuredValue_t>& list);

    Worker* m_worker = nullptr;
    //    QThread m_workerThread;
    bool m_doNotSkip[8];
    static const ScanSettings m_scanSettings;
    //    const QString m_cipher;
    //    const QString m_type;
    Result_t m_result[8];
    QVector<QString> m_paths;
    QVector<QString> m_serNum;
    QList<QCheckBox*> m_listCheckBox;

    int m_measureTimerId = 0;
    int m_stage = 0;

    bool Test1();
    bool Test2();
    bool Test3();
    bool Test4();
    bool Test5();
    bool Test6();
    bool Test7();

    QTimer m_timerRms;

private slots:
    void on_pbStartStop_clicked(bool checked);

    // QWidget interface
protected:
    //    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
};

#endif // AUTOMATICMEASUREMENTS_H
