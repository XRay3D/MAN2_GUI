#ifndef PREPAREFORM_H
#define PREPAREFORM_H

#include "preparation.h"
#include "ui_prepareform.h"
#include <QWidget>

namespace Ui {
class PrepareForm;
}

class PrepareForm : public QWidget, public Ui::PrepareForm {
    Q_OBJECT
    friend class MainWindow;

public:
    explicit PrepareForm(QWidget* parent = nullptr);
    ~PrepareForm();

signals:
    void CurrentTabIndex(int);
    void ScanSettingsSignal(const ScanSettings*);
    void SerialNumberChanged(const QString&, int);

private slots:
    void statrtMeasure_clicked();
};

#endif // PREPAREFORM_H
