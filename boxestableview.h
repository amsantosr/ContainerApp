#ifndef BOXESTABLEVIEW_H
#define BOXESTABLEVIEW_H

#include <QTableView>

class BoxesTableView : public QTableView
{
public:
    BoxesTableView(QWidget *parent = 0);
    QModelIndexList selectedIndexes() const
    {
        return QTableView::selectedIndexes();
    }
    QVector<int> selectedRows() const;
};

#endif // BOXESTABLEVIEW_H
