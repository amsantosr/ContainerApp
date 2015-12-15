#include "containerproblemtablemodel.h"

ContainerProblemTableModel::ContainerProblemTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

void ContainerProblemTableModel::initialize(QList<int> widths, QList<int> heights, QList<int> depths)
{
    beginResetModel();
    this->lengthXValues = widths;
    this->lengthYValues = heights;
    this->lengthZValues = depths;
    endResetModel();
}

int ContainerProblemTableModel::rowCount(const QModelIndex &) const
{
    return lengthXValues.size();
}

int ContainerProblemTableModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant ContainerProblemTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return lengthXValues[index.row()];
        case 1: return lengthYValues[index.row()];
        case 2: return lengthZValues[index.row()];
        }
    }
    return QVariant();
}

QVariant ContainerProblemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return tr("X Length");
            case 1: return tr("Y Length");
            case 2: return tr("Z Length");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
