#include "containerproblem.h"

ContainerProblem::ContainerProblem(QObject *parent)
    : QObject(parent),
      containerLengthXValue(0),
      containerLengthYValue(0),
      containerLengthZValue(0)
{
}

void ContainerProblem::removeGroup(int index)
{
    emit beforeGroupsCounterChanged();
    groupLengthXValues.remove(index);
    groupLengthYValues.remove(index);
    groupLengthZValues.remove(index);
    groupBoxesCounterValues.remove(index);
    groupColorValues.remove(index);
    groupDescriptionValues.remove(index);
    emit afterGroupsCounterChanged();
}

void ContainerProblem::addGroup(int lengthX, int lengthY, int lengthZ,
                              int quantity, QColor color, QString description)
{
    emit beforeAddGroup();
    emit beforeGroupsCounterChanged();
    groupLengthXValues.append(lengthX);
    groupLengthYValues.append(lengthY);
    groupLengthZValues.append(lengthZ);
    groupBoxesCounterValues.append(quantity);
    groupColorValues.append(color);
    groupDescriptionValues.append(description);
    emit afterGroupsCounterChanged();
    emit afterAddGroup();
}

void ContainerProblem::clear()
{
    if (groupLengthXValues.size() > 0)
    {
        emit beforeGroupsCounterChanged();
        groupLengthXValues.clear();
        groupLengthYValues.clear();
        groupLengthZValues.clear();
        groupBoxesCounterValues.clear();
        groupColorValues.clear();
        groupDescriptionValues.clear();
        emit afterGroupsCounterChanged();
    }
    setContainerLengthX(0);
    setContainerLengthY(0);
    setContainerLengthZ(0);
}

void ContainerProblem::setGroupsCount(int count)
{
    emit beforeGroupsCounterChanged();
    groupLengthXValues.resize(count);
    groupLengthYValues.resize(count);
    groupLengthZValues.resize(count);
    groupBoxesCounterValues.resize(count);
    groupColorValues.resize(count);
    groupDescriptionValues.resize(count);
    emit afterGroupsCounterChanged();
}

void ContainerProblem::setGroup(int index, int quantity, int lengthX, int lengthY, int lengthZ,
                                QColor color, QString description)
{
    bool emitSignal = false;
    if (groupBoxesCounterValues[index] != quantity)
    {
        groupBoxesCounterValues[index] = quantity;
        emitSignal = true;
    }
    if (groupLengthXValues[index] != lengthX)
    {
        groupLengthXValues[index] = lengthX;
        emitSignal = true;
    }
    if (groupLengthYValues[index] != lengthY)
    {
        groupLengthYValues[index] = lengthY;
        emitSignal = true;
    }
    if (groupLengthZValues[index] != lengthZ)
    {
        groupLengthZValues[index] = lengthZ;
        emitSignal = true;
    }
    if (groupColorValues[index] != color)
    {
        groupColorValues[index] = color;
        emitSignal = true;
    }
    if (groupDescriptionValues[index] != description)
    {
        groupDescriptionValues[index] = description;
        emitSignal = true;
    }
    if (emitSignal)
        emit groupChanged(index);
}

void ContainerProblem::setGroupBoxCount(int boxIndex, int quantity)
{
    if (groupBoxesCounterValues[boxIndex] != quantity)
    {
        groupBoxesCounterValues[boxIndex] = quantity;
        emit groupChanged(boxIndex);
    }
}

void ContainerProblem::setContainerLengthX(int value)
{
    if (containerLengthXValue != value)
    {
        containerLengthXValue = value;
        emit containerLengthXChanged(value);
    }
}

void ContainerProblem::setContainerLengthY(int value)
{
    if (containerLengthYValue != value)
    {
        containerLengthYValue = value;
        emit containerLengthYChanged(value);
    }
}

void ContainerProblem::setContainerLengthZ(int value)
{
    if (containerLengthZValue != value)
    {
        containerLengthZValue = value;
        emit containerLengthZChanged(value);
    }
}
