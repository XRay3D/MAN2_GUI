#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "ui_communications.h"

class ManInterface;
class MainWindow;
class Communications : public QWidget, private Ui::Communications {
    Q_OBJECT

    friend class MainWindow;

public:
    explicit Communications(QWidget* parent = 0);
    ~Communications();

    void setMan(ManInterface* man);
signals:
    void CurrentTabIndex(int);
    void SetTabBarEnabled(bool);
private slots:
    void on_pbManCheckConnection_clicked();

    void on_pbIrtCheckConnection_clicked();

private:
    ManInterface* m_man;
};

#endif // COMMUNICATIONS_H
