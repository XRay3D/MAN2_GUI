#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include "ui_mainwindow.h"
#include "man2.h"
#include "measuringinterface/measuringinterface.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void writeSettings();
    void readSettings();
    QThread m_manThread;
    int m_statusBarTimer;
    MEASURING_INTERFACE mi;
    QLineEdit* statusBarTime;
    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override;
};

#endif // MAINWINDOW2_H
