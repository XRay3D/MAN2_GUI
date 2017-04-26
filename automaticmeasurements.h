#ifndef AUTOMATICMEASUREMENTS_H
#define AUTOMATICMEASUREMENTS_H

#include "measuringinterface/man2.h"
#include "ui_automaticmeasurements.h"
#include "inputparameters.h"
#include <QAxWidget>
#include <QDialog>
#include <QHideEvent>
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

class MyDialog : public QDialog {
    Q_OBJECT
public:
    MyDialog(QWidget* parent = 0, QString windowTitle = "")
        : QDialog(parent)
    {

        if (objectName().isEmpty()) {
            setObjectName(QStringLiteral("MyDialog"));
        }
        resize(1020, 600);
        verticalLayout = new QVBoxLayout(this);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        axWidget = new QAxWidget(this);
        axWidget->setControl(QStringLiteral("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
        axWidget->setObjectName(QStringLiteral("axWidget"));
        verticalLayout->addWidget(axWidget);
        pushButton = new QPushButton(this);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        verticalLayout->addWidget(pushButton);
        setWindowTitle(windowTitle);
        pushButton->setText("Печать");
        connect(pushButton, &QPushButton::clicked, this, &MyDialog::Print);
        show();
    }
    virtual ~MyDialog()
    {
        //delete axWidget;
        qDebug() << "~MyDialog()";
    }
    void LoadFile(const QString& file)
    {
        QVariantList params;
        params << file; //.replace('/', '\\');
        axWidget->dynamicCall("Navigate(QString, QVariant&, QVariant&, QVariant&, QVariant&)", params);
        //        thread()->sleep(1);
        //        params.clear();
        //        params << 7; //OLECMDID_PRINTPREVIEW
        //        params << 1; //OLECMDEXECOPT_PROMPTUSER
        //        axWidget->dynamicCall("ExecWB(OLECMDID, OLECMDEXECOPT, QVariant&, QVariant&)", params);
        //    QFile file5("axWidget.htm");
        //    qDebug() << file5.open(QFile::WriteOnly);
        //    file5.write(axWidget->generateDocumentation().toLocal8Bit());
        //axWidget->Navigate(file);
        //axWidget->dynamicCall("LoadFile(const QString&)", file);
    }
    QVBoxLayout* verticalLayout;
    //    SHDocVw::WebBrowser* axWidget;
    QAxWidget* axWidget;
    QPushButton* pushButton;
    void Print(bool b)
    {
        Q_UNUSED(b)
        QVariantList params;
        params << 7; //OLECMDID_PRINTPREVIEW
        params << 1; //OLECMDEXECOPT_PROMPTUSER
        axWidget->dynamicCall("ExecWB(OLECMDID, OLECMDEXECOPT, QVariant&, QVariant&)", params);
    }

protected:
    void hideEvent(QHideEvent* event)
    {
        //verticalLayout->removeWidget(axWidget);
        qDebug() << "hideEvent()";
        event->accept();
        deleteLater();
    }
};

class Worker : public QThread {
    Q_OBJECT
public:
    Worker(bool* doNotSkip, Result_t* result, const ScanSettings* scanSettings, QObject* parent = Q_NULLPTR);
    ~Worker();
    void SetMan();
    void Continue();
    void FinishMeasurements();
    void Quit();
    void Setup();

    void run() override;

signals:
    void ShowMessage(int num);
    void UpdateProgresBar();

private:
    bool Finished();
    bool CheckFinished();
    bool Test1();
    bool Test2();
    bool Test3();
    bool Test4();
    bool Test5();
    bool Test6();
    bool Test7();

    void Msleep(unsigned long time);

    const bool* m_doNotSkip;
    Result_t* m_result;
    const ScanSettings* m_pScanSettings;
    QSemaphore m_semaphore;
    int m_counter;
};

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
    void ItemDoubleClicked(QListWidgetItem* item);
    void UpdateProgresBar();
    void SaveProtokol(const QString& serialNumber, int number);

    Worker* m_worker = nullptr;
    //    QThread m_workerThread;
    bool m_doNotSkip[8];
    static const ScanSettings m_scanSettings;
    //    const QString m_cipher;
    //    const QString m_type;
    Result_t m_result[8];
    QList<QString> m_paths;
    QList<QString> m_serNum;
    QList<QCheckBox*> m_listCheckBox;
    QList<QDoubleSpinBox*> m_listDsbVoltage;
    QList<QDoubleSpinBox*> m_listDsbCurrent;

private slots:
    void on_pbStartStop_clicked(bool checked);

    // QWidget interface
protected:
    //    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
};

#endif // AUTOMATICMEASUREMENTS_H
