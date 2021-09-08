#ifndef TABLE_H
#define TABLE_H

#include <QTableView>

class Header;
class TestModel;
class QCheckBox;
class QAbstractButton;

class TableView : public QTableView {
    Q_OBJECT
public:
    explicit TableView(QWidget* parent = nullptr);
    ~TableView();
    TestModel* model() const;
    void init();
    void initCheckBox();
    void initRadioButton();
    void initCheckBoxRadioButton();

    QCheckBox* checkBox() const;

private:
    TestModel* m_model;
    void createCornerCheckBox(Header* header);
    QCheckBox* m_checkBox = nullptr;
};

#endif // TABLE_H
