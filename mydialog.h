#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QAxWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHideEvent>
#include <QDebug>

class MyDialog : public QDialog {
    Q_OBJECT
public:
    MyDialog(QWidget* parent = nullptr, QString windowTitle = "");
    virtual ~MyDialog();
    void LoadFile(const QString& file);
    QVBoxLayout* verticalLayout;
    //    SHDocVw::WebBrowser* axWidget;
    QAxWidget* axWidget;
    QPushButton* pushButton;
    void Print(bool b);

protected:
    void hideEvent(QHideEvent* event);
};

#endif // MYDIALOG_H
