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
            QString text;
            switch (index.column())
            {
            case 0: text = QString::number(containerSolution->boxLengthX(index.row())); break;
            case 1: text = QString::number(containerSolution->boxLengthY(index.row())); break;
            case 2: text = QString::number(containerSolution->boxLengthZ(index.row())); break;
            case 3: text = QString::number(containerSolution->boxCoordinateX(index.row())); break;
            case 4: text = QString::number(containerSolution->boxCoordinateY(index.row())); break;
            case 5: text = QString::number(containerSolution->boxCoordinateZ(index.row())); break;
            case 6: text = tr("Si"); break;
            }
            QString textUnit = containerSolution->textUnit();
            if (!textUnit.isNull() && index.column() < 6)
            {
                text.append(" ").append(textUnit);
            }
            result = QVariant(text);
        }
        else
        {
            switch (index.column())
            {
            case 6: result = tr("No"); break;
            }
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        if (index.column() < 6)
            return Qt::AlignRight;
        else
            return Qt::AlignCenter;
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
