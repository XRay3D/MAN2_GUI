#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>
#include <QtSerialPort>
#include "ui_testform.h"

class TestForm : public QWidget, public Ui::TestForm {
    Q_OBJECT

public:
    explicit TestForm(QWidget* parent = 0);
    ~TestForm();

signals:
    void U(int ch, float v);
    void Ping(QString portName);
    void GetMeasuredValue(int type);

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
};

#endif // TESTFORM_H
