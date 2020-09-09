#ifndef TECTSTABLEVIEW_H
#define TECTSTABLEVIEW_H

#include <QTableView>

class Header;
class TestModel;
class QCheckBox;
class QAbstractButton;

class TestsTableView : public QTableView {
    Q_OBJECT
public:
    explicit TestsTableView(QWidget* parent = nullptr);
    ~TestsTableView();

    TestModel* model() const;

    void init();
    void initCheckBox();
    void initRadioButton();
    void initCheckBoxRadioButton();

    void setCheckable(bool fl);
    //    QCheckBox* checkBox() const;

private:
    TestModel* m_model;

    void createCornerCheckBox(Header* header);
    QCheckBox* m_checkBox = nullptr;
};

#endif // TECTSTABLEVIEW_H
