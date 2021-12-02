#ifndef PREPAREFORM_H
#define PREPAREFORM_H

#include "hw/interface.h"
#include "scansettings.h"
#include "ui_autoform.h"
#include <QWidget>

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

class Tester;
namespace Ui {
class PrepareForm;
}

class PrepareForm : public QWidget, public Ui::PrepareForm {
    Q_OBJECT
    friend class MainWindow;

    int measureTimerId = 0;
    Tester* tester_ = nullptr;
    bool m_doNotSkip[8];

public:
    explicit PrepareForm(QWidget* parent = nullptr);
    ~PrepareForm();

    void showMessage(int num);
    void updateProgresBar();

    void endSlot();

    void pbStartStopClicked(bool checked);

signals:
    void currentTabIndex(int);
    void ScanSettingsSignal(const ScanSettings*);
    void SerialNumberChanged(const QString&, int);
    void startMeasure(ValueType type = CurrentMeasuredValue, uint8_t channel = 0);

private slots:

    void on_pbProto_clicked();

protected:
    // QWidget interface
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

    // QObject interface
    void timerEvent(QTimerEvent* event) override;
};

#endif // PREPAREFORM_H
