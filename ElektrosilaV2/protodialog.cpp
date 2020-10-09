#include "protodialog.h"

ProtoDialog::ProtoDialog(QWidget* parent, QString windowTitle)
    : QDialog(parent)
{
    if (objectName().isEmpty()) {
        setObjectName(QStringLiteral("MyDialog"));
    }
    resize(1020, 600);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    axWidget = new QAxWidget(this);
    axWidget->setControl(QStringLiteral("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    axWidget->setObjectName(QStringLiteral("axWidget"));
    verticalLayout->addWidget(axWidget);
    pushButton = new QPushButton(this);
    pushButton->setObjectName(QStringLiteral("pushButton"));
    verticalLayout->addWidget(pushButton);
    setWindowTitle(windowTitle);
    pushButton->setText("Печать");
    connect(pushButton, &QPushButton::clicked, this, &ProtoDialog::Print);
    show();
}

ProtoDialog::~ProtoDialog() { }

void ProtoDialog::LoadFile(const QString& file)
{
    QVariantList params;
    params << file; //.replace('/', '\\');
    axWidget->dynamicCall("Navigate(QString, QVariant&, QVariant&, QVariant&, QVariant&)", params);
    //        thread()->sleep(1);
    //        params.clear();
    //        params << 7; //OLECMDID_PRINTPREVIEW
    //        params << 1; //OLECMDEXECOPT_PROMPTUSER
    //        axWidget->dynamicCall("ExecWB(OLECMDID, OLECMDEXECOPT, QVariant&, QVariant&)", params);
    //    QFile file5("axWidget.htm");
    //    qDebug() << file5.open(QFile::WriteOnly);
    //    file5.write(axWidget->generateDocumentation().toLocal8Bit());
    //axWidget->Navigate(file);
    //axWidget->dynamicCall("LoadFile(const QString&)", file);
}

void ProtoDialog::Print(bool b)
{
    Q_UNUSED(b)
    QVariantList params;
    params << 7; //OLECMDID_PRINTPREVIEW
    params << 1; //OLECMDEXECOPT_PROMPTUSER
    axWidget->dynamicCall("ExecWB(OLECMDID, OLECMDEXECOPT, QVariant&, QVariant&)", params);
}

void ProtoDialog::hideEvent(QHideEvent* event)
{
    //verticalLayout->removeWidget(axWidget);
    qDebug() << "hideEvent()";
    event->accept();
    deleteLater();
}
