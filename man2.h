#ifndef MAN2_H
#define MAN2_H

#include <QWidget>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class MAN2 : public QWidget {
    Q_OBJECT

public:
    explicit MAN2(QWidget* parent = 0);
    ~MAN2();
    void SetU(float v, int channel);
    void SetIM(float i, int channel);
    void SetIU(float u, int channel);

    // QWidget interface
protected:
    void showEvent(QShowEvent* event);

private:
    const int m_channel;

    QVBoxLayout* verticalLayout;
    QGroupBox* groupBox;
    QVBoxLayout* verticalLayout_2;
    QLabel* label;
    QLineEdit* leVoltage;
    QLabel* label_2;
    QDoubleSpinBox* dsbCurrent;
    QLabel* label_3;
    QLineEdit* leCurrent;
    QPushButton* pushButton;
    QPushButton* pushButton_2;

    void setupUi(QWidget* MAN2); // setupUi

    void retranslateUi(QWidget* MAN2); // retranslateUi
};

#endif // MAN2_H
