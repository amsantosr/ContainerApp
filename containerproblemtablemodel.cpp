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
        connect(containerProblem, &ContainerProblem::beforeAddGroup, [this]()
        {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
        });
        connect(containerProblem, &ContainerProblem::afterAddGroup,
                this, &ContainerProblemTableModel::endInsertRows);
        connect(containerProblem, &ContainerProblem::beforeGroupsCounterChanged,
                this, &ContainerProblemTableModel::beginResetModel);
        connect(containerProblem, &ContainerProblem::afterGroupsCounterChanged,
                this, &ContainerProblemTableModel::endResetModel);
        connect(containerProblem, &ContainerProblem::groupChanged, this, [this](int boxIndex)
        {
            emit dataChanged(this->index(boxIndex, 0), this->index(boxIndex, columnCount()));
        });
    }
}

int ContainerProblemTableModel::rowCount(const QModelIndex &) const
{
    if (!containerProblem)
        return 0;
    return containerProblem->groupsCounter();
}

int ContainerProblemTableModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant ContainerProblemTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        switch (index.column())
        {
        case 0: result = containerProblem->groupLengthX(row); break;
        case 1: result = containerProblem->groupLengthY(row); break;
        case 2: result = containerProblem->groupLengthZ(row); break;
        case 3: result = containerProblem->groupBoxesCounter(row); break;
        case 4: result = containerProblem->groupDescription(index.row()); break;
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        //result = Qt::AlignRight;
    }
    return result;
}

QVariant ContainerProblemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return QString("Dim X");
            case 1: return QString("Dim Y");
            case 2: return QString("Dim Z");
            case 3: return QString("Cantidad");
            case 4: return QString("Descripción");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
