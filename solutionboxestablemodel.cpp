#include "solutionboxestablemodel.h"
#include "containersolution.h"

SolutionBoxesTableModel::SolutionBoxesTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      containerSolution(0)
{

}

void SolutionBoxesTableModel::setContainerSolution(ContainerSolution *solution)
{
    if (solution != containerSolution)
    {
        std::swap(containerSolution, solution);
        if (containerSolution != 0)
        {
            connect(containerSolution, &ContainerSolution::beforeDataChange,
                    this, &SolutionBoxesTableModel::beginResetModel);
            connect(containerSolution, &ContainerSolution::afterDataChange,
                    this, &SolutionBoxesTableModel::endResetModel);
        }
        if (solution != 0)
        {
            disconnect(solution, 0, this, 0);
        }
    }
}

int SolutionBoxesTableModel::rowCount(const QModelIndex &) const
{
    return containerSolution != 0 ? containerSolution->boxesCount() : 0;
}

int SolutionBoxesTableModel::columnCount(const QModelIndex &) const
{
    return 8;
}

QVariant SolutionBoxesTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        int boxTypeIndex = containerSolution->boxGroupIndex(index.row());
        QString string;
        switch (index.column())
        {
        case 0: string = QString::number(boxTypeIndex + 1); break;
        case 1: string = containerSolution->getContainerProblem()->groupName(boxTypeIndex); break;
        case 2: string = QString::number(containerSolution->boxLengthX(index.row())); break;
        case 3: string = QString::number(containerSolution->boxLengthY(index.row())); break;
        case 4: string = QString::number(containerSolution->boxLengthZ(index.row())); break;
        case 5: string = QString::number(containerSolution->boxCoordinateX(index.row())); break;
        case 6: string = QString::number(containerSolution->boxCoordinateY(index.row())); break;
        case 7: string = QString::number(containerSolution->boxCoordinateZ(index.row())); break;
        }
        result = string;
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        // align right only for columns Tipo, Dim X, Dim Y, Dim Z, Pos X, Pos Y, Pos Z
        case 0: case 2: case 3: case 4: case 5: case 6: case 7:
            result = int(Qt::AlignRight | Qt::AlignVCenter);
            break;
        }
    }
    return result;
}

QVariant SolutionBoxesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return tr("ID de grupo"); break;
            case 1: return tr("Nombre de grupo"); break;
            case 2: return tr("Dim X"); break;
            case 3: return tr("Dim Y"); break;
            case 4: return tr("Dim Z"); break;
            case 5: return tr("Pos X"); break;
            case 6: return tr("Pos Y"); break;
            case 7: return tr("Pos Z"); break;
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
