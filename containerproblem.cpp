#include "containerproblem.h"

ContainerProblem::ContainerProblem(QObject *parent)
    : QObject(parent)
{
    problemData.containerLengthXValue = 0;
    problemData.containerLengthYValue = 0;
    problemData.containerLengthZValue = 0;
}

void ContainerProblem::removeGroup(int index)
{
    emit beforeGroupsCounterChanged();
    problemData.groupLengthXValues.remove(index);
    problemData.groupLengthYValues.remove(index);
    problemData.groupLengthZValues.remove(index);
    problemData.groupBoxesCounterValues.remove(index);
    problemData.groupColorValues.remove(index);
    problemData.groupDescriptionValues.remove(index);
    emit afterGroupsCounterChanged();
}

void ContainerProblem::addGroup(int lengthX, int lengthY, int lengthZ,
                              int quantity, QColor color, QString name)
{
    emit beforeAddGroup();
    emit beforeGroupsCounterChanged();
    problemData.groupLengthXValues.append(lengthX);
    problemData.groupLengthYValues.append(lengthY);
    problemData.groupLengthZValues.append(lengthZ);
    problemData.groupBoxesCounterValues.append(quantity);
    problemData.groupColorValues.append(color);
    problemData.groupDescriptionValues.append(name);
    emit afterGroupsCounterChanged();
    emit afterAddGroup();
}

void ContainerProblem::clear()
{
    if (problemData.groupLengthXValues.size() > 0)
    {
        emit beforeGroupsCounterChanged();
        problemData.groupLengthXValues.clear();
        problemData.groupLengthYValues.clear();
        problemData.groupLengthZValues.clear();
        problemData.groupBoxesCounterValues.clear();
        problemData.groupColorValues.clear();
        problemData.groupDescriptionValues.clear();
        emit afterGroupsCounterChanged();
    }
    setContainerLengthX(0);
    setContainerLengthY(0);
    setContainerLengthZ(0);
}

void ContainerProblem::setGroupsCount(int count)
{
    emit beforeGroupsCounterChanged();
    problemData.groupLengthXValues.resize(count);
    problemData.groupLengthYValues.resize(count);
    problemData.groupLengthZValues.resize(count);
    problemData.groupBoxesCounterValues.resize(count);
    problemData.groupColorValues.resize(count);
    problemData.groupDescriptionValues.resize(count);
    emit afterGroupsCounterChanged();
}

void ContainerProblem::setGroup(int index, int quantity, int lengthX, int lengthY, int lengthZ,
                                QColor color, QString name)
{
    bool emitSignal = false;
    if (problemData.groupBoxesCounterValues[index] != quantity)
    {
        problemData.groupBoxesCounterValues[index] = quantity;
        emitSignal = true;
    }
    if (problemData.groupLengthXValues[index] != lengthX)
    {
        problemData.groupLengthXValues[index] = lengthX;
        emitSignal = true;
    }
    if (problemData.groupLengthYValues[index] != lengthY)
    {
        problemData.groupLengthYValues[index] = lengthY;
        emitSignal = true;
    }
    if (problemData.groupLengthZValues[index] != lengthZ)
    {
        problemData.groupLengthZValues[index] = lengthZ;
        emitSignal = true;
    }
    if (problemData.groupColorValues[index] != color)
    {
        problemData.groupColorValues[index] = color;
        emitSignal = true;
    }
    if (problemData.groupDescriptionValues[index] != name)
    {
        problemData.groupDescriptionValues[index] = name;
        emitSignal = true;
    }
    if (emitSignal)
        emit groupChanged(index);
}

void ContainerProblem::setGroupBoxCount(int boxIndex, int quantity)
{
    if (problemData.groupBoxesCounterValues[boxIndex] != quantity)
    {
        problemData.groupBoxesCounterValues[boxIndex] = quantity;
        emit groupChanged(boxIndex);
    }
}

void ContainerProblem::setContainerLengthX(int value)
{
    if (problemData.containerLengthXValue != value)
    {
        problemData.containerLengthXValue = value;
        emit containerLengthXChanged(value);
    }
}

void ContainerProblem::setContainerLengthY(int value)
{
    if (problemData.containerLengthYValue != value)
    {
        problemData.containerLengthYValue = value;
        emit containerLengthYChanged(value);
    }
}

void ContainerProblem::setContainerLengthZ(int value)
{
    if (problemData.containerLengthZValue != value)
    {
        problemData.containerLengthZValue = value;
        emit containerLengthZChanged(value);
    }
}
