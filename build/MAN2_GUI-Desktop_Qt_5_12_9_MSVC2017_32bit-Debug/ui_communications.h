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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Communications
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QComboBox *cbManPort;
    QPushButton *pbManCheckConnection;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *Communications)
    {
        if (Communications->objectName().isEmpty())
            Communications->setObjectName(QString::fromUtf8("Communications"));
        Communications->resize(751, 352);
        horizontalLayout = new QHBoxLayout(Communications);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(Communications);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        cbManPort = new QComboBox(groupBox);
        cbManPort->setObjectName(QString::fromUtf8("cbManPort"));

        verticalLayout_2->addWidget(cbManPort);

        pbManCheckConnection = new QPushButton(groupBox);
        pbManCheckConnection->setObjectName(QString::fromUtf8("pbManCheckConnection"));

        verticalLayout_2->addWidget(pbManCheckConnection);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout->addWidget(groupBox);


        retranslateUi(Communications);

        QMetaObject::connectSlotsByName(Communications);
    } // setupUi

    void retranslateUi(QWidget *Communications)
    {
        Communications->setWindowTitle(QApplication::translate("Communications", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("Communications", "\320\237\320\276\321\200\321\202\321\213", nullptr));
        pbManCheckConnection->setText(QApplication::translate("Communications", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Communications: public Ui_Communications {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMUNICATIONS_H
