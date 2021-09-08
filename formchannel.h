#ifndef FORMCHANNEL_H
#define FORMCHANNEL_H

#include "ui_formchannel.h"
#include <QtWidgets>

class FormChannel : public QGroupBox, public Ui::FormChannel {
    Q_OBJECT

public:
    explicit FormChannel(QWidget* parent = nullptr);
    ~FormChannel();
};

#endif // FORMCHANNEL_H
