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
        connect(containerProblem, &ContainerProblem::beforeAddBox,
                this, [=]() {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
        });
        connect(containerProblem, &ContainerProblem::afterAddBox,
                this, &ContainerProblemTableModel::endInsertRows);
        connect(containerProblem, &ContainerProblem::beforeBoxCountChanged,
                this, &ContainerProblemTableModel::beginResetModel);
        connect(containerProblem, &ContainerProblem::afterBoxCountChanged,
                this, &ContainerProblemTableModel::endResetModel);
        connect(containerProblem, &ContainerProblem::boxDimensionsChanged, this, [&](int boxIndex)
        {
            emit dataChanged(this->index(boxIndex, 0), this->index(boxIndex, columnCount()));
        });
    }
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
    if (role == Qt::DisplayRole)
    {
        QString string;
        switch (index.column())
        {
        case 0: string = QString::number(containerProblem->boxLengthX(index.row())); break;
        case 1: string = QString::number(containerProblem->boxLengthY(index.row())); break;
        case 2: string = QString::number(containerProblem->boxLengthZ(index.row())); break;
        }
        QString textUnit = containerProblem->textUnit();
        if (!textUnit.isNull())
        {
            string += " ";
            string += containerProblem->textUnit();
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
            case 0: return tr("Dim X");
            case 1: return tr("Dim Y");
            case 2: return tr("Dim Z");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
