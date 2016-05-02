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
        QString string;
        switch (index.column())
        {
        case 0: string = tr("Caja %1").arg(boxIndex + 1); break;
        case 1: string = QString::number(containerSolution->boxLengthX(boxIndex)); break;
        case 2: string = QString::number(containerSolution->boxLengthY(boxIndex)); break;
        case 3: string = QString::number(containerSolution->boxLengthZ(boxIndex)); break;
        case 4: string = QString::number(containerSolution->boxCoordinateX(boxIndex)); break;
        case 5: string = QString::number(containerSolution->boxCoordinateY(boxIndex)); break;
        case 6: string = QString::number(containerSolution->boxCoordinateZ(boxIndex)); break;
        }
        QString textUnit = containerSolution->textUnit();
        if (!textUnit.isNull() && 0 < index.column())
            string += " " + containerSolution->textUnit();
        result = string;
    }
    else if (role == Qt::TextAlignmentRole)
    {
        if (0 < index.column())
            return Qt::AlignRight;
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
