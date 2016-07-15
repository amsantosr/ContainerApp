#ifndef CONTAINERGROUPSTABLEMODEL_H
#define CONTAINERGROUPSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "containersolution.h"

class ContainerGroupsTableModel : public QAbstractTableModel
{
public:
    ContainerGroupsTableModel(QObject *parent = 0);
    void setContainerSolution(ContainerSolution *solution);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void updateCounters();

private:
    ContainerSolution *containerSolution;
    ContainerProblem *containerProblem;
    QVector<int> packedBoxesCounters;
    QVector<int> unpackedBoxesCounters;
};

#endif // CONTAINERGROUPSTABLEMODEL_H
