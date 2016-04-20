#include "boxesorderingtablemodel.h"
#include "containersolution.h"

BoxesOrderingTableModel::BoxesOrderingTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      containerSolution(0)
{

}

void BoxesOrderingTableModel::setContainerSolution(ContainerSolution *solution)
{
    if (solution != containerSolution)
    {
        std::swap(containerSolution, solution);
        if (containerSolution != 0)
        {
            connect(containerSolution, &ContainerSolution::beforeDataChange,
                    this, &BoxesOrderingTableModel::beginResetModel);
            connect(containerSolution, &ContainerSolution::afterDataChange,
                    this, &BoxesOrderingTableModel::endResetModel);
        }
        if (solution != 0)
        {
            disconnect(solution, 0, this, 0);
        }
    }
}

int BoxesOrderingTableModel::rowCount(const QModelIndex &) const
{
    return containerSolution != 0 ? containerSolution->packedBoxesCount() : 0;
}

int BoxesOrderingTableModel::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant BoxesOrderingTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        int boxIndex = containerSolution->boxOrderIndex(index.row());
        switch (index.column())
        {
        case 0: result = tr("Caja %1").arg(boxIndex + 1); break;
        case 1: result = containerSolution->boxLengthX(boxIndex); break;
        case 2: result = containerSolution->boxLengthY(boxIndex); break;
        case 3: result = containerSolution->boxLengthZ(boxIndex); break;
        case 4: result = containerSolution->boxCoordinateX(boxIndex); break;
        case 5: result = containerSolution->boxCoordinateY(boxIndex); break;
        case 6: result = containerSolution->boxCoordinateZ(boxIndex); break;
        }
    }
    return result;
}

QVariant BoxesOrderingTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return tr("ID"); break;
            case 1: return tr("Dim X"); break;
            case 2: return tr("Dim Y"); break;
            case 3: return tr("Dim Z"); break;
            case 4: return tr("Pos X"); break;
            case 5: return tr("Pos Y"); break;
            case 6: return tr("Pos Z"); break;
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
