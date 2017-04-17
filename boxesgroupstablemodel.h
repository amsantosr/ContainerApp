#ifndef CONTAINERPROBLEMTABLEMODEL_H
#define CONTAINERPROBLEMTABLEMODEL_H

#include <QAbstractTableModel>
#include "containerproblem.h"

class BoxesGroupsTableModel : public QAbstractTableModel
{
public:
    BoxesGroupsTableModel(QObject *parent = 0);
    void setContainerProblem(ContainerProblem *problem);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setPackedBoxesCounters(QVector<int> packedBoxesCounters);

private:
    ContainerProblem *containerProblem;
    QVector<int> packedBoxesCounters;
};

#endif // CONTAINERPROBLEMTABLEMODEL_H
