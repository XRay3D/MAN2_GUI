#include "man.h"
#include "ui_man.h"
#include <QDebug>
#include <QFontDatabase>

MAN::MAN(QWidget* parent)
    : QWidget(parent)
    , m_channel(0)
{
    setupUi(this);
}

MAN::~MAN()
{
}

void MAN::SetU(float v, int channel)
{
    if (m_channel != channel) {
        return;
    }
    if (v != 0.0) {
        leVoltage->setText(QString().setNum(v, 'g', 5).replace('.', ',').left(6));
    }
    else {
        leVoltage->setText("0,0000");
    }
}

void MAN::SetIM(float i, int channel)
{
    if (m_channel != channel) {
        return;
    }
    qDebug() << i << channel;
    if (i > 0.0001) {
        leCurrent->setText(QString().setNum(i, 'g', 5).replace('.', ',').left(6));
    }
    else {
        leCurrent->setText("0,0000");
    }
}

void MAN::SetIU(float u, int channel)
{
    if (m_channel != channel) {
        return;
    }
    dsbCurrent->setValue(u);
}

void MAN::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    *const_cast<int*>(&m_channel) = objectName().split('_').last().toInt();
    qDebug() << "showEvent" << m_channel;
    groupBox->setTitle(QString("Канал %1").arg(m_channel));
}

void MAN::setupUi(QWidget* MAN2)
{
    if (MAN2->objectName().isEmpty()) {
        MAN2->setObjectName(QStringLiteral("MAN2"));
    }
    int id = QFontDatabase::addApplicationFont("://res/sa-digital-number.ttf"); //путь к шрифту
    QString family = QFontDatabase::applicationFontFamilies(id).at(0); //имя шрифта
    QFont f(family); // QFont c вашим шрифтом
    f.setPixelSize(20);
    verticalLayout = new QVBoxLayout(MAN2);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    groupBox = new QGroupBox(MAN2);
    groupBox->setObjectName(QStringLiteral("groupBox"));
    groupBox->setCheckable(true);
    verticalLayout_2 = new QVBoxLayout(groupBox);
    verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(6, 6, 6, 6);
    label = new QLabel(groupBox);
    label->setObjectName(QStringLiteral("label"));
    verticalLayout_2->addWidget(label);
    leVoltage = new QLineEdit(groupBox);
    leVoltage->setFont(f);
    leVoltage->setObjectName(QStringLiteral("lineEdit"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(leVoltage->sizePolicy().hasHeightForWidth());
    leVoltage->setSizePolicy(sizePolicy);
    QFont font;
    font.setPointSize(15);
    leVoltage->setFont(font);
    leVoltage->setAlignment(Qt::AlignCenter);
    verticalLayout_2->addWidget(leVoltage);
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QStringLiteral("label_2"));
    verticalLayout_2->addWidget(label_2);
    dsbCurrent = new QDoubleSpinBox(groupBox);
    dsbCurrent->setFont(f);
    dsbCurrent->setObjectName(QStringLiteral("doubleSpinBox"));
    dsbCurrent->setFont(font);
    dsbCurrent->setAlignment(Qt::AlignCenter);
    dsbCurrent->setButtonSymbols(QAbstractSpinBox::NoButtons);
    dsbCurrent->setDecimals(3);
    dsbCurrent->setSingleStep(0.1);
    dsbCurrent->setValue(0);
    verticalLayout_2->addWidget(dsbCurrent);
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QStringLiteral("label_3"));
    verticalLayout_2->addWidget(label_3);
    leCurrent = new QLineEdit(groupBox);
    leCurrent->setFont(f);
    leCurrent->setObjectName(QStringLiteral("lineEdit_3"));
    sizePolicy.setHeightForWidth(leCurrent->sizePolicy().hasHeightForWidth());
    leCurrent->setSizePolicy(sizePolicy);
    leCurrent->setFont(font);
    leCurrent->setAlignment(Qt::AlignCenter);
    verticalLayout_2->addWidget(leCurrent);
    pushButton = new QPushButton(groupBox);
    pushButton->setObjectName(QStringLiteral("pushButton"));
    verticalLayout_2->addWidget(pushButton);
    pushButton_2 = new QPushButton(groupBox);
    pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
    verticalLayout_2->addWidget(pushButton_2);
    verticalLayout->addWidget(groupBox);
    retranslateUi(MAN2);
    QMetaObject::connectSlotsByName(MAN2);
}

void MAN::retranslateUi(QWidget* MAN2)
{
    MAN2->setWindowTitle(QApplication::translate("MAN2", "Form", Q_NULLPTR));
    groupBox->setTitle(QApplication::translate("MAN2", "\320\232\320\260\320\275\320\260\320\273", Q_NULLPTR));
    label->setText(QApplication::translate("MAN2", "\320\235\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265, \320\222:", Q_NULLPTR));
    leVoltage->setText(QApplication::translate("MAN2", "__,___", Q_NULLPTR));
    label_2->setText(QApplication::translate("MAN2",
        "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275\320\275\321\213\320\271\n"
        "\321\202\320\276\320\272, \320\220:",
        Q_NULLPTR));
    label_3->setText(QApplication::translate("MAN2",
        "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\275\320\275\321\213\320\271\n"
        "\321\202\320\276\320\272, \320\220:",
        Q_NULLPTR));
    leCurrent->setText(QApplication::translate("MAN2",
        "__,___", Q_NULLPTR));
    pushButton->setText(QApplication::translate("MAN2",
        "\320\232\320\276\321\200\320\276\321\202\320\272\320\276\320\265\n"
        "\320\267\320\260\320\274\321\213\320\272\320\260\320\275\320\270\320\265.",
        Q_NULLPTR));
    pushButton_2->setText(QApplication::translate("MAN2",
        "\320\237\320\276\320\270\321\201\320\272 \321\202\320\276\320\272\320\260\n"
        "\321\201\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\275\320\270\321\217.",
        Q_NULLPTR));
}
