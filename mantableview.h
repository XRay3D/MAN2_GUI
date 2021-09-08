#ifndef MANTABLEVIEW_H
#define MANTABLEVIEW_H

#include <QTableView>

class ManTableView : public QTableView {
    Q_OBJECT
public:
    explicit ManTableView(QWidget* parent = nullptr);

protected:
    // QWidget interface
    void showEvent(QShowEvent* event) override;
};

#endif // MANTABLEVIEW_H
