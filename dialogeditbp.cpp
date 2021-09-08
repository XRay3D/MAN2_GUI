#include "dialogeditbp.h"
#include "ui_dialogeditbp.h"

#include "devicemodel.h"

DialogEditBp::DialogEditBp(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogEditBp)
{
    ui->setupUi(this);

    ui->tableView->setModel(DeviceModel::instance());

    ui->tableView->setSpan(0, 0, 1, 2);
    ui->tableView->setSpan(1, 0, 1, 2);
    ui->tableView->setSpan(2, 0, 1, 2);
    ui->tableView->setSpan(3, 0, 1, 2);
    ui->tableView->setSpan(4, 0, 1, 2);
    ui->tableView->setSpan(5, 0, 1, 2);
    ui->tableView->setSpan(6, 0, 1, 2);
    ui->tableView->setSpan(7, 0, 1, 2);
    ui->tableView->setSpan(8, 0, 1, 2);
    ui->tableView->setSpan(12, 0, 1, 2);
    ui->tableView->setSpan(13, 0, 1, 2);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

DialogEditBp::~DialogEditBp()
{
    delete ui;
}
