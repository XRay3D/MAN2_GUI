#include "mantableview.h"

#include "manmodel.h"

#include <QDebug>
#include <QtWidgets>

ManTableView::ManTableView(QWidget* parent)
    : QTableView(parent)
{
    setModel(new ManModel(this));
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    verticalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    setSpan(2, 0, 1, 8);

    auto verticalLayout = new QVBoxLayout(findChild<QAbstractButton*>());
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(6, 6, 6, 6);
    auto label = new QLabel("Канал ->", this);
    label->setAlignment(Qt::AlignCenter);
    verticalLayout->addWidget(label);
}

void ManTableView::showEvent(QShowEvent* event)
{
    QTableView::showEvent(event);
    setMinimumHeight(rowHeight(0) + rowHeight(1) + rowHeight(2) + horizontalHeader()->height() + 20);
}
