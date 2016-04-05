#include "containersolutiontablemodel.h"

ContainerSolutionTableModel::ContainerSolutionTableModel(QObject *parent)
    : QAbstractTableModel(parent), containerSolution(0)
{

}

void ContainerSolutionTableModel::setContainerSolution(ContainerSolution *solution)
{
    if (solution != containerSolution)
    {
        qSwap(containerSolution, solution);
        if (containerSolution != 0)
        {
            connect(containerSolution, SIGNAL(beforeBoxCountChanged()), this, SLOT(slotBeforeBoxCountChanged()));
            connect(containerSolution, SIGNAL(afterBoxCountChanged()), this, SLOT(slotAfterBoxCountChanged()));
        }
        if (solution != 0)
        {
            disconnect(solution, 0, this, 0);
        }
    }
}

int ContainerSolutionTableModel::rowCount(const QModelIndex &) const
{
    return containerSolution != 0 ? containerSolution->boxCount() : 0;
}

int ContainerSolutionTableModel::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant ContainerSolutionTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0: result = containerSolution->boxLengthX(index.row()); break;
        case 1: result = containerSolution->boxLengthY(index.row()); break;
        case 2: result = containerSolution->boxLengthZ(index.row()); break;
        case 3: result = containerSolution->boxCoordinateX(index.row()); break;
        case 4: result = containerSolution->boxCoordinateY(index.row()); break;
        case 5: result = containerSolution->boxCoordinateZ(index.row()); break;
        case 6: result = containerSolution->isBoxPacked(index.row()) ? tr("Yes") : tr("No"); break;
        }
    }
    return result;
}

QVariant ContainerSolutionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return tr("Length X"); break;
            case 1: return tr("Length Y"); break;
            case 2: return tr("Length Z"); break;
            case 3: return tr("X"); break;
            case 4: return tr("Y"); break;
            case 5: return tr("Z"); break;
            case 6: return tr("Is packed"); break;
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void ContainerSolutionTableModel::slotBeforeBoxCountChanged()
{
    beginResetModel();
}

void ContainerSolutionTableModel::slotAfterBoxCountChanged()
{
    endResetModel();
}
