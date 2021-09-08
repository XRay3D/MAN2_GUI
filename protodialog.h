#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QAxWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHideEvent>
#include <QDebug>

class ProtoDialog : public QDialog {
    Q_OBJECT
public:
    ProtoDialog(QWidget* parent = nullptr, QString windowTitle = "");
    virtual ~ProtoDialog();
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
