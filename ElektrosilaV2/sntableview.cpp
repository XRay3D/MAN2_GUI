#include "sntableview.h"
#include "sernummodel.h"

#include <QHeaderView>

SnTableView::SnTableView(QWidget* parent)
    : QTableView(parent)
{
    setModel(SerNumModel::instance());
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
