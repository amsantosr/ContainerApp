#ifndef BOXESGROUPSTABLEVIEW_H
#define BOXESGROUPSTABLEVIEW_H

#include <QTableView>

class BoxesGroupsTableView : public QTableView
{
public:
    BoxesGroupsTableView(QWidget *parent = 0);
    QModelIndexList selectedIndexes() const
    {
        return QTableView::selectedIndexes();
    }
    QVector<int> selectedRows() const;
};

#endif // BOXESGROUPSTABLEVIEW_H
