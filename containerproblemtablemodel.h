#ifndef CONTAINERPROBLEMTABLEMODEL_H
#define CONTAINERPROBLEMTABLEMODEL_H

#include <QAbstractTableModel>
#include "containerproblem.h"

class ContainerProblemTableModel : public QAbstractTableModel
{
public:
    ContainerProblemTableModel(QObject *parent = 0);
    void setContainerProblem(ContainerProblem *problem);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    ContainerProblem *containerProblem;
};

#endif // CONTAINERPROBLEMTABLEMODEL_H
