#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "ui_communications.h"

class MainWindow;
class Communications : public QWidget, private Ui::Communications {
    Q_OBJECT

    friend class MainWindow;

public:
    explicit Communications(QWidget* parent = 0);
    void CheckConnection();
    ~Communications();

signals:
    void CurrentTabIndex(int);
    void SetTabBarEnabled(bool);

private slots:
    void on_pbManCheckConnection_clicked();
};

#endif // COMMUNICATIONS_H
