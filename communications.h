#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "ui_communications.h"

class MainWindow;

class Communications final : public QWidget, private Ui::Communications {
    Q_OBJECT
    friend class MainWindow;

public:
    explicit Communications(QWidget* parent = nullptr);
    ~Communications() = default;

    void checkConnection();

signals:
    void currentTabIndex(int);
    void setTabBarEnabled(bool);
};

#endif // COMMUNICATIONS_H
