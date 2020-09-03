#include "testform.h"
#include <QDebug>
#include <QPainter>

TestForm::TestForm(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    comboBox_2->addItems(QString("VALUE_VOLTAGE,VALUE_CURRENT,VALUE_TRIP_CURRENT,VALUE_TEMPERATURE,").split(','));
}

TestForm::~TestForm()
{
}

void TestForm::on_pushButton_1_clicked()
{
    qDebug() << "Ping";
    emit Ping(comboBox->currentText());
}

void TestForm::on_pushButton_2_clicked()
{
    emit GetMeasuredValue(comboBox_2->currentIndex());
}

void TestForm::on_pushButton_3_clicked() {}

void TestForm::on_pushButton_4_clicked() {}

void TestForm::on_pushButton_5_clicked() {}

void TestForm::on_pushButton_6_clicked() {}

void TestForm::on_pushButton_7_clicked() {}

void TestForm::on_pushButton_8_clicked() {}
