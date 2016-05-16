#include "containersolution.h"
#include <QRect>

ContainerSolution::ContainerSolution(QObject *parent)
    : QObject(parent),
      containerProblem(0)
{
}

void ContainerSolution::clear()
{
    if (!boxLengthXValues.empty())
    {
        emit beforeDataChange();
        boxLengthXValues.clear();
        boxLengthYValues.clear();
        boxLengthZValues.clear();
        boxCoordinateXValues.clear();
        boxCoordinateYValues.clear();
        boxCoordinateZValues.clear();
        boxPackedFlagValues.clear();
        packedBoxesIndexes.clear();
        emit afterDataChange();
    }
}

void ContainerSolution::setContainerProblem(ContainerProblem *problem)
{
    containerProblem = problem;
}

void ContainerSolution::setPackedBoxes(QVector<int> boxLengthsX,
                                       QVector<int> boxLengthsY,
                                       QVector<int> boxLengthsZ,
                                       QVector<int> boxCoordinatesX,
                                       QVector<int> boxCoordinatesY,
                                       QVector<int> boxCoordinatesZ,
                                       QVector<int> packedBoxesIndexes)
{
    emit beforeDataChange();
    boxPackedFlagValues.fill(false, containerProblem->boxCount());
    boxLengthXValues = boxLengthsX;
    boxLengthYValues = boxLengthsY;
    boxLengthZValues = boxLengthsZ;
    boxCoordinateXValues = boxCoordinatesX;
    boxCoordinateYValues = boxCoordinatesY;
    boxCoordinateZValues = boxCoordinatesZ;
    for (int i = 0; i < packedBoxesIndexes.size(); ++i)
    {
        int boxIndex = packedBoxesIndexes[i];
        boxPackedFlagValues[boxIndex] = true;
    }
    this->packedBoxesIndexes = packedBoxesIndexes;
    emit afterDataChange();
}
