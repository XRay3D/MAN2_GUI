#ifndef SNTABLEVIEW_H
#define SNTABLEVIEW_H

#include <QTableView>

class SnTableView : public QTableView {
    Q_OBJECT
public:
    explicit SnTableView(QWidget* parent = nullptr);

signals:
};

#endif // SNTABLEVIEW_H
