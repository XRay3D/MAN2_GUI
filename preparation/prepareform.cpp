#include "prepareform.h"
#include "devicemodel.h"
#include "sernummodel.h"

#include <QMessageBox>
int id = qRegisterMetaType<ScanSettings>("ScanSettings_t");

PrepareForm::PrepareForm(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    tvParams->setModel(new DeviceModel(this));
    tvParams->setSpan(0, 0, 1, 2);
    tvParams->setSpan(1, 0, 1, 2);
    tvParams->setSpan(2, 0, 1, 2);
    tvParams->setSpan(3, 0, 1, 2);
    tvParams->setSpan(4, 0, 1, 2);
    tvParams->setSpan(5, 0, 1, 2);
    tvParams->setSpan(6, 0, 1, 2);
    tvParams->setSpan(7, 0, 1, 2);
    tvParams->setSpan(8, 0, 1, 2);
    tvParams->setSpan(9, 0, 1, 2);
    tvParams->setSpan(10, 0, 1, 2);
    tvParams->setSpan(13, 0, 1, 2);
    tvParams->setSpan(14, 0, 1, 2);
    tvParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvParams->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    cbxDevice->addItems(DeviceModel::self->cbxData());

    tvSerNum->setModel(SerNumModel::self);
    tvSerNum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvSerNum->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(pbClearSerNum, &QPushButton::clicked, SerNumModel::self, &SerNumModel::clear);
    connect(pbStatrtMeasure, &QPushButton::clicked, this, &PrepareForm::statrtMeasure_clicked);
    //connect(cbxDevice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), DeviceModel::self, &DeviceModel::setIndex);
    connect(cbxDevice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [](int index) {
        DeviceModel::self->setIndex(index);
        SerNumModel::self->setCount(8 / DeviceModel::self->scanSettings().NumberOfChannels);
    });
}

PrepareForm::~PrepareForm()
{
}

void PrepareForm::statrtMeasure_clicked()
{
    if (SerNumModel::self->isEmpty())
        QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
    else
        emit CurrentTabIndex(1);
}
