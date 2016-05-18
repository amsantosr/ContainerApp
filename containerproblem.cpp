#include "containerproblem.h"

ContainerProblem::ContainerProblem(QObject *parent)
    : QObject(parent),
      containerLengthXValue(0),
      containerLengthYValue(0),
      containerLengthZValue(0),
      textUnitValue("cm.")
{
}

void ContainerProblem::removeBoxes(QModelIndexList indexes)
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
            boxQuantityValues.remove(index);
            boxColorValues.remove(index);
            boxDescriptionValues.remove(index);
        }
        emit afterBoxCountChanged();
    }
}

void ContainerProblem::addBox(int lengthX, int lengthY, int lengthZ,
                              int quantity, QColor color, QString description)
{
    emit beforeAddBox();
    emit beforeBoxCountChanged();
    boxLengthXValues.append(lengthX);
    boxLengthYValues.append(lengthY);
    boxLengthZValues.append(lengthZ);
    boxQuantityValues.append(quantity);
    boxColorValues.append(color);
    boxDescriptionValues.append(description);
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
        boxQuantityValues.clear();
        boxColorValues.clear();
        boxDescriptionValues.clear();
        emit afterBoxCountChanged();
    }
    setContainerLengthX(0);
    setContainerLengthY(0);
    setContainerLengthZ(0);
}

void ContainerProblem::setBoxCount(int count)
{
    emit beforeBoxCountChanged();
    boxLengthXValues.resize(count);
    boxLengthYValues.resize(count);
    boxLengthZValues.resize(count);
    boxColorValues.resize(count);
    boxDescriptionValues.resize(count);
    emit afterBoxCountChanged();
}

void ContainerProblem::setBox(int boxIndex, int lengthX, int lengthY, int lengthZ,
                              int quantity, QColor color, QString description)
{
    boxLengthXValues[boxIndex] = lengthX;
    boxLengthYValues[boxIndex] = lengthY;
    boxLengthZValues[boxIndex] = lengthZ;
    boxQuantityValues[boxIndex] = quantity;
    boxColorValues[boxIndex] = color;
    boxDescriptionValues[boxIndex] = description;
    emit boxValuesChanged(boxIndex);
}

void ContainerProblem::setBoxQuantity(int boxIndex, int quantity)
{
    if (boxQuantityValues[boxIndex] != quantity)
    {
        boxQuantityValues[boxIndex] = quantity;
        emit boxValuesChanged(boxIndex);
    }
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

void ContainerProblem::setTextUnit(QString value)
{
    if (textUnitValue != value)
    {
        textUnitValue = value;
        emit textUnit_changed(value);
    }
}
