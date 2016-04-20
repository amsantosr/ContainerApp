#ifndef BOXESORDERINGTABLEMODEL_H
#define BOXESORDERINGTABLEMODEL_H

#include <QAbstractTableModel>

class ContainerSolution;

class BoxesOrderingTableModel : public QAbstractTableModel
{
public:
    BoxesOrderingTableModel(QObject *parent = 0);
    void setContainerSolution(ContainerSolution *solution);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    ContainerSolution *containerSolution;
};

#endif // BOXESORDERINGTABLEMODEL_H
