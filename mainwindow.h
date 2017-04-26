#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include "ui_mainwindow.h"
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
    QLineEdit* statusBarTime;

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override;
    MI mi;
};

#endif // MAINWINDOW2_H
