#ifndef CONTAINERSOLUTIONTABLEMODEL_H
#define CONTAINERSOLUTIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "containersolution.h"

class ContainerSolutionTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ContainerSolutionTableModel(QObject *parent = 0);
    void setContainerSolution(ContainerSolution *solution);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private slots:
    void slotBeforeBoxCountChanged();
    void slotAfterBoxCountChanged();

private:
    ContainerSolution *containerSolution;
};

#endif // CONTAINERSOLUTIONTABLEMODEL_H
