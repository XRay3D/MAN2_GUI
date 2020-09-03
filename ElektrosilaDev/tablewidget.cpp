#include "tablewidget.h"

TableWidget::TableWidget(QWidget* parent)
    : QTableWidget(8, 7, parent)
{
    for (int col = 0; col < colorCount(); ++col) {
        for (int row = 0; row < rowCount(); ++row) {
            QTableWidgetItem* tableWidgetItem = new QTableWidgetItem(QString("%1:%2").arg(row).arg(col));
            tableWidgetItem->setFlags(Qt::ItemIsEnabled);
            setItem(row, col, tableWidgetItem);
        }
    }
}
