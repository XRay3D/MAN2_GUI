#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include "ui_mainwindow.h"
#include "hw/interface.h"

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
private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW2_H
