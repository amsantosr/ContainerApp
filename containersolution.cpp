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
        packedBoxesTypesIndexes.clear();
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
                                       QVector<int> packedBoxesTypes)
{
    emit beforeDataChange();
    boxLengthXValues = boxLengthsX;
    boxLengthYValues = boxLengthsY;
    boxLengthZValues = boxLengthsZ;
    boxCoordinateXValues = boxCoordinatesX;
    boxCoordinateYValues = boxCoordinatesY;
    boxCoordinateZValues = boxCoordinatesZ;
    packedBoxesTypesIndexes = packedBoxesTypes;
    emit afterDataChange();
}
