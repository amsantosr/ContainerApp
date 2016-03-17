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

void ContainerProblemTableModel::addBox(int xdim, int ydim, int zdim)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    lengthXValues.append(xdim);
    lengthYValues.append(ydim);
    lengthZValues.append(zdim);
    endInsertRows();
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
            case 0: return tr("X");
            case 1: return tr("Y");
            case 2: return tr("Z");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void ContainerProblemTableModel::clear()
{
    beginResetModel();
    lengthXValues.clear();
    lengthYValues.clear();
    lengthZValues.clear();
    endResetModel();
}
