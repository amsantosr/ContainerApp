#include "containersolutiontablemodel.h"

ContainerSolutionTableModel::ContainerSolutionTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

void ContainerSolutionTableModel::initialize(QVector<int> boxLengthXvalues,
                                             QVector<int> boxLengthYvalues,
                                             QVector<int> boxLengthZvalues,
                                             QVector<int> boxCoordinateXvalues,
                                             QVector<int> boxCoordinateYvalues,
                                             QVector<int> boxCoordinateZvalues,
                                             QVector<bool> boxPackedFlagValues)
{
    beginResetModel();
    this->boxLengthXvalues = boxLengthXvalues;
    this->boxLengthYvalues = boxLengthYvalues;
    this->boxLengthZvalues = boxLengthZvalues;
    this->boxCoordinateXvalues = boxCoordinateXvalues;
    this->boxCoordinateYvalues = boxCoordinateYvalues;
    this->boxCoordinateZvalues = boxCoordinateZvalues;
    this->boxPackedFlagValues = boxPackedFlagValues;
    endResetModel();
}

int ContainerSolutionTableModel::rowCount(const QModelIndex &parent) const
{
    return boxLengthXvalues.size();
}

int ContainerSolutionTableModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant ContainerSolutionTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0: result = boxLengthXvalues[index.row()]; break;
        case 1: result = boxLengthYvalues[index.row()]; break;
        case 2: result = boxLengthZvalues[index.row()]; break;
        case 3: result = boxCoordinateXvalues[index.row()]; break;
        case 4: result = boxCoordinateYvalues[index.row()]; break;
        case 5: result = boxCoordinateZvalues[index.row()]; break;
        case 6: result = boxPackedFlagValues[index.row()] ? tr("Yes") : tr("No"); break;
        }
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
            case 0: return tr("Length X"); break;
            case 1: return tr("Length Y"); break;
            case 2: return tr("Length Z"); break;
            case 3: return tr("X"); break;
            case 4: return tr("Y"); break;
            case 5: return tr("Z"); break;
            case 6: return tr("Is packed"); break;
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
