#include "boxesgroupstableview.h"

BoxesGroupsTableView::BoxesGroupsTableView(QWidget *parent)
    : QTableView(parent)
{

}

QVector<int> BoxesGroupsTableView::selectedRows() const
{
    // returns a vector of the rows selected in the table view
    QVector<int> rows;
    foreach (QModelIndex index, selectedIndexes())
    {
        rows.append(index.row());
    }
    std::sort(rows.begin(), rows.end());
    auto last = std::unique(rows.begin(), rows.end());
    int count = last - rows.begin();
    rows.resize(count);
    return rows;
}
