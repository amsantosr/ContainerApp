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
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        QString textUnit = containerProblem->textUnit();
        QString string;
        switch (index.column())
        {
        case 0: string = QString("%1 %2").arg(containerProblem->groupLengthX(row)).arg(textUnit); break;
        case 1: string = QString("%1 %2").arg(containerProblem->groupLengthY(row)).arg(textUnit); break;
        case 2: string = QString("%1 %2").arg(containerProblem->groupLengthZ(row)).arg(textUnit); break;
        case 3: string = QString::number(containerProblem->groupBoxesCounter(row)); break;
        case 4: string = containerProblem->groupDescription(index.row()); break;
        }
        return string;
    }
    else if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignRight;
    }
    return QVariant();
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
