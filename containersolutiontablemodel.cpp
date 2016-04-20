#include "containersolutiontablemodel.h"

ContainerSolutionTableModel::ContainerSolutionTableModel(QObject *parent)
    : QAbstractTableModel(parent), containerSolution(0)
{

}

void ContainerSolutionTableModel::setContainerSolution(ContainerSolution *solution)
{
    if (solution != containerSolution)
    {
        std::swap(containerSolution, solution);
        if (containerSolution != 0)
        {
            connect(containerSolution, &ContainerSolution::beforeDataChange,
                    this, &ContainerSolutionTableModel::beginResetModel);
            connect(containerSolution, &ContainerSolution::afterDataChange,
                    this, &ContainerSolutionTableModel::endResetModel);
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
        if (containerSolution->isBoxPacked(index.row()))
        {
            switch (index.column())
            {
            case 0: result = containerSolution->boxLengthX(index.row()); break;
            case 1: result = containerSolution->boxLengthY(index.row()); break;
            case 2: result = containerSolution->boxLengthZ(index.row()); break;
            case 3: result = containerSolution->boxCoordinateX(index.row()); break;
            case 4: result = containerSolution->boxCoordinateY(index.row()); break;
            case 5: result = containerSolution->boxCoordinateZ(index.row()); break;
            case 6: result = tr("Si"); break;
            }
        }
        else
        {
            switch (index.column())
            {
            case 6: result = tr("No");
            }
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
            case 0: return tr("Dim X"); break;
            case 1: return tr("Dim Y"); break;
            case 2: return tr("Dim Z"); break;
            case 3: return tr("Pos X"); break;
            case 4: return tr("Pos Y"); break;
            case 5: return tr("Pos Z"); break;
            case 6: return tr("Colocada"); break;
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
