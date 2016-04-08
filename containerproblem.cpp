#include "containerproblem.h"

void ContainerProblem::removeIndexes(QModelIndexList indexes)
{
    if (!indexes.empty())
    {
        emit beforeBoxCountChanged();
        QVector<int> rowIndexes;
        foreach (QModelIndex index, indexes)
        {
            rowIndexes.append(index.row());
        }

        std::sort(rowIndexes.begin(), rowIndexes.end(), std::greater<int>());
        auto last = std::unique(rowIndexes.begin(), rowIndexes.end());
        rowIndexes.resize(last - rowIndexes.begin());

        for (int i = 0; i < rowIndexes.size(); ++i)
        {
            int index = rowIndexes[i];
            boxLengthXValues.remove(index);
            boxLengthYValues.remove(index);
            boxLengthZValues.remove(index);
        }
        emit afterBoxCountChanged();
    }
}

void ContainerProblem::addBox(int xLength, int yLength, int zLength)
{
    emit beforeAddBox();
    emit beforeBoxCountChanged();
    boxLengthXValues.append(xLength);
    boxLengthYValues.append(yLength);
    boxLengthZValues.append(zLength);
    emit afterBoxCountChanged();
    emit afterAddBox();
}

void ContainerProblem::clear()
{
    if (boxLengthXValues.size() > 0)
    {
        emit beforeBoxCountChanged();
        boxLengthXValues.clear();
        boxLengthYValues.clear();
        boxLengthZValues.clear();
        emit afterBoxCountChanged();
    }
    setContainerLengthX(0);
    setContainerLengthY(0);
    setContainerLengthZ(0);
}

void ContainerProblem::setContainerLengthX(int value)
{
    if (containerLengthXValue != value)
    {
        containerLengthXValue = value;
        emit containerLengthX_changed(value);
    }
}

void ContainerProblem::setContainerLengthY(int value)
{
    if (containerLengthYValue != value)
    {
        containerLengthYValue = value;
        emit containerLengthY_changed(value);
    }
}

void ContainerProblem::setContainerLengthZ(int value)
{
    if (containerLengthZValue != value)
    {
        containerLengthZValue = value;
        emit containerLengthZ_changed(value);
    }
}
