#include "containerproblemtablemodel.h"

ContainerProblemTableModel::ContainerProblemTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      containerProblem(0)
{

}

void ContainerProblemTableModel::setContainerProblem(ContainerProblem *pointer)
{
    if (containerProblem != pointer)
    {
        if (containerProblem != 0)
            disconnect(containerProblem, 0, this, 0);
        containerProblem = pointer;
        connect(containerProblem, SIGNAL(beforeAddBox()), this, SLOT(slotBeforeAddBox()));
        connect(containerProblem, SIGNAL(afterAddBox()), this, SLOT(slotAfterAddBox()));
        connect(containerProblem, SIGNAL(beforeBoxCountChanged()), this, SLOT(slotBeginReset()));
        connect(containerProblem, SIGNAL(afterBoxCountChanged()), this, SLOT(slotEndReset()));
    }
}

void ContainerProblemTableModel::slotBeforeAddBox()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
}

void ContainerProblemTableModel::slotAfterAddBox()
{
    endInsertRows();
}

void ContainerProblemTableModel::slotBeginReset()
{
    beginResetModel();
}

void ContainerProblemTableModel::slotEndReset()
{
    endResetModel();
}

int ContainerProblemTableModel::rowCount(const QModelIndex &) const
{
    if (!containerProblem)
        return 0;
    return containerProblem->boxCount();
}

int ContainerProblemTableModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant ContainerProblemTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return containerProblem->boxLengthX(index.row());
        case 1: return containerProblem->boxLengthY(index.row());
        case 2: return containerProblem->boxLengthZ(index.row());
        }
    }
    return QVariant();
}

QVariant ContainerProblemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return tr("X");
            case 1: return tr("Y");
            case 2: return tr("Z");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
