#ifndef GRADUATION_H
#define GRADUATION_H

#include "ui_graduation.h"

class MainWindow;

class Graduation : public QWidget, private Ui::Graduation {
    Q_OBJECT
    friend class MainWindow;

public:
    explicit Graduation(QWidget* parent = nullptr);

    ~Graduation();

private:
    uint8_t m_channel = 0;
    QVector<QCheckBox*> m_chbxs;

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);

private slots:
    void on_pbStartGrad_clicked();
};

#endif // GRADUATION_H
