#ifndef DIALOGEDITBP_H
#define DIALOGEDITBP_H

#include <QDialog>

namespace Ui {
class DialogEditBp;
}

class DialogEditBp : public QDialog {
    Q_OBJECT

public:
    explicit DialogEditBp(QWidget* parent = nullptr);
    ~DialogEditBp();

private:
    Ui::DialogEditBp* ui;
};

#endif // DIALOGEDITBP_H
