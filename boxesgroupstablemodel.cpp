#include "boxesgroupstablemodel.h"

BoxesGroupsTableModel::BoxesGroupsTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      containerProblem(0)
{

}

void BoxesGroupsTableModel::setContainerProblem(ContainerProblem *pointer)
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
                this, &BoxesGroupsTableModel::endInsertRows);
        connect(containerProblem, &ContainerProblem::beforeGroupsCounterChanged,
                this, &BoxesGroupsTableModel::beginResetModel);
        connect(containerProblem, &ContainerProblem::afterGroupsCounterChanged,
                this, &BoxesGroupsTableModel::endResetModel);
        connect(containerProblem, &ContainerProblem::groupChanged, [this](int boxIndex)
        {
            emit dataChanged(this->index(boxIndex, 0), this->index(boxIndex, columnCount()));
        });
    }
}

int BoxesGroupsTableModel::rowCount(const QModelIndex &) const
{
    if (!containerProblem)
        return 0;
    return containerProblem->groupsCounter();
}

int BoxesGroupsTableModel::columnCount(const QModelIndex &) const
{
    return 6;
}

QVariant BoxesGroupsTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        switch (index.column())
        {
        case 0: result = containerProblem->groupName(row); break;
        case 1: result = containerProblem->groupLengthX(row); break;
        case 2: result = containerProblem->groupLengthY(row); break;
        case 3: result = containerProblem->groupLengthZ(row); break;
        case 4: result = containerProblem->groupBoxesCounter(row); break;
        case 5: /* empty string and print the color */ break;
        }
    }
    else if (role == Qt::BackgroundColorRole)
    {
        if (index.column() == 5)
        {
            result = containerProblem->groupColor(index.row());
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case 1: case 2: case 3: case 4:
            result = int(Qt::AlignRight | Qt::AlignVCenter);
            break;
        }
    }
    return result;
}

QVariant BoxesGroupsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result = QAbstractTableModel::headerData(section, orientation, role);
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: result = QString("Nombre"); break;
            case 1: result = QString("Dim X"); break;
            case 2: result = QString("Dim Y"); break;
            case 3: result = QString("Dim Z"); break;
            case 4: result = QString("Cantidad"); break;
            case 5: result = QString("Color"); break;
            }
        }
    }
    return result;
}
