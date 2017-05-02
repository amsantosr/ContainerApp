#include "containerproblem.h"

ContainerProblem::ContainerProblem(QObject *parent)
    : QObject(parent)
{
    problemData.containerLengthX = 0;
    problemData.containerLengthY = 0;
    problemData.containerLengthZ = 0;
}

void ContainerProblem::removeGroup(int index)
{
    emit beforeGroupsCounterChanged();
    problemData.groupLengthXVector.remove(index);
    problemData.groupLengthYVector.remove(index);
    problemData.groupLengthZVector.remove(index);
    problemData.groupBoxesCountersVector.remove(index);
    problemData.groupColorVector.remove(index);
    problemData.groupNamesVector.remove(index);
    emit afterGroupsCounterChanged();
}

void ContainerProblem::addGroup(int lengthX, int lengthY, int lengthZ,
                              int quantity, QColor color, QString name)
{
    emit beforeAddGroup();
    emit beforeGroupsCounterChanged();
    problemData.groupLengthXVector.append(lengthX);
    problemData.groupLengthYVector.append(lengthY);
    problemData.groupLengthZVector.append(lengthZ);
    problemData.groupBoxesCountersVector.append(quantity);
    problemData.groupColorVector.append(color);
    problemData.groupNamesVector.append(name);
    emit afterGroupsCounterChanged();
    emit afterAddGroup();
}

void ContainerProblem::clear()
{
    if (problemData.groupLengthXVector.size() > 0)
    {
        emit beforeGroupsCounterChanged();
        problemData.groupLengthXVector.clear();
        problemData.groupLengthYVector.clear();
        problemData.groupLengthZVector.clear();
        problemData.groupBoxesCountersVector.clear();
        problemData.groupColorVector.clear();
        problemData.groupNamesVector.clear();
        emit afterGroupsCounterChanged();
    }
    setContainerLengthX(0);
    setContainerLengthY(0);
    setContainerLengthZ(0);
}

void ContainerProblem::setGroupsCount(int count)
{
    emit beforeGroupsCounterChanged();
    problemData.groupLengthXVector.resize(count);
    problemData.groupLengthYVector.resize(count);
    problemData.groupLengthZVector.resize(count);
    problemData.groupBoxesCountersVector.resize(count);
    problemData.groupColorVector.resize(count);
    problemData.groupNamesVector.resize(count);
    emit afterGroupsCounterChanged();
}

void ContainerProblem::setGroup(int index, int lengthX, int lengthY, int lengthZ, int quantity,
                                QColor color, QString name)
{
    bool emitSignal = false;
    if (problemData.groupBoxesCountersVector[index] != quantity)
    {
        problemData.groupBoxesCountersVector[index] = quantity;
        emitSignal = true;
    }
    if (problemData.groupLengthXVector[index] != lengthX)
    {
        problemData.groupLengthXVector[index] = lengthX;
        emitSignal = true;
    }
    if (problemData.groupLengthYVector[index] != lengthY)
    {
        problemData.groupLengthYVector[index] = lengthY;
        emitSignal = true;
    }
    if (problemData.groupLengthZVector[index] != lengthZ)
    {
        problemData.groupLengthZVector[index] = lengthZ;
        emitSignal = true;
    }
    if (problemData.groupColorVector[index] != color)
    {
        problemData.groupColorVector[index] = color;
        emitSignal = true;
    }
    if (problemData.groupNamesVector[index] != name)
    {
        problemData.groupNamesVector[index] = name;
        emitSignal = true;
    }
    if (emitSignal)
        emit groupChanged(index);
}

void ContainerProblem::setGroupBoxCount(int boxIndex, int quantity)
{
    if (problemData.groupBoxesCountersVector[boxIndex] != quantity)
    {
        problemData.groupBoxesCountersVector[boxIndex] = quantity;
        emit groupChanged(boxIndex);
    }
}

void ContainerProblem::setContainerLengthX(int value)
{
    if (problemData.containerLengthX != value)
    {
        problemData.containerLengthX = value;
        emit containerLengthXChanged(value);
    }
}

void ContainerProblem::setContainerLengthY(int value)
{
    if (problemData.containerLengthY != value)
    {
        problemData.containerLengthY = value;
        emit containerLengthYChanged(value);
    }
}

void ContainerProblem::setContainerLengthZ(int value)
{
    if (problemData.containerLengthZ != value)
    {
        problemData.containerLengthZ = value;
        emit containerLengthZChanged(value);
    }
}
