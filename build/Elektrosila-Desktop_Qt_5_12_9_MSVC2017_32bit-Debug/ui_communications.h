/********************************************************************************
** Form generated from reading UI file 'communications.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMUNICATIONS_H
#define UI_COMMUNICATIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Communications
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QComboBox *cbManPort;
    QPushButton *pushButton;
    QLabel *label;
    QComboBox *cbManPortScpi;
    QLabel *label_3;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pbManCheckConnection;
    QSpinBox *spinBox;

    void setupUi(QWidget *Communications)
    {
        if (Communications->objectName().isEmpty())
            Communications->setObjectName(QString::fromUtf8("Communications"));
        Communications->resize(751, 352);
        horizontalLayout = new QHBoxLayout(Communications);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(Communications);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cbManPort = new QComboBox(groupBox);
        cbManPort->setObjectName(QString::fromUtf8("cbManPort"));

        gridLayout->addWidget(cbManPort, 0, 1, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 6, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cbManPortScpi = new QComboBox(groupBox);
        cbManPortScpi->setObjectName(QString::fromUtf8("cbManPortScpi"));

        gridLayout->addWidget(cbManPortScpi, 2, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(false);

        gridLayout->addWidget(comboBox, 4, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 7, 1, 1, 1);

        pbManCheckConnection = new QPushButton(groupBox);
        pbManCheckConnection->setObjectName(QString::fromUtf8("pbManCheckConnection"));

        gridLayout->addWidget(pbManCheckConnection, 3, 1, 1, 1);

        spinBox = new QSpinBox(groupBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(255);

        gridLayout->addWidget(spinBox, 5, 1, 1, 1);


        horizontalLayout->addWidget(groupBox);


        retranslateUi(Communications);

        QMetaObject::connectSlotsByName(Communications);
    } // setupUi

    void retranslateUi(QWidget *Communications)
    {
        Communications->setWindowTitle(QApplication::translate("Communications", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("Communications", "\320\237\320\276\321\200\321\202\321\213", nullptr));
        pushButton->setText(QApplication::translate("Communications", "PushButton", nullptr));
        label->setText(QApplication::translate("Communications", "\320\234\320\220\320\235:", nullptr));
        label_3->setText(QApplication::translate("Communications", "Agient 34401A (SCPI):", nullptr));
        pbManCheckConnection->setText(QApplication::translate("Communications", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Communications: public Ui_Communications {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMUNICATIONS_H
