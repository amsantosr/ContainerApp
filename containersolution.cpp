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
    sortBoxOrderIndexes();
    emit afterDataChange();
}

void ContainerSolution::sortBoxOrderIndexes()
{
    // ordering of the boxes according to the explained algorithm
    sortedBoxesIndexes.resize(packedBoxesIndexes.size());
    for (int i = 0; i < packedBoxesIndexes.size(); ++i)
        sortedBoxesIndexes[i] = i;
    std::sort(sortedBoxesIndexes.begin(), sortedBoxesIndexes.end(), [&](int a, int b) -> bool
    {
        if (packedBoxCoordinateZ(a) != packedBoxCoordinateZ(b))
            return packedBoxCoordinateZ(a) < packedBoxCoordinateZ(b);
        if (packedBoxCoordinateX(a) != packedBoxCoordinateX(b))
            return packedBoxCoordinateX(a) < packedBoxCoordinateX(b);
        return packedBoxCoordinateY(a) < packedBoxCoordinateY(b);
    });
    for (int j = 1; j < sortedBoxesIndexes.size(); ++j)
    {
        int b = sortedBoxesIndexes[j];
        for (int i = 0; i < j; ++i)
        {
            int a = sortedBoxesIndexes[i];
            QRect rectBoxA(packedBoxCoordinateX(a), packedBoxCoordinateZ(a), packedBoxLengthX(a), packedBoxLengthZ(a));
            QRect rectBoxB(packedBoxCoordinateX(b), packedBoxCoordinateZ(b), packedBoxLengthX(b), packedBoxLengthZ(b));
            if (rectBoxA.intersects(rectBoxB) && packedBoxCoordinateY(b) < packedBoxCoordinateY(a))
            {
                packedBoxesIndexes.remove(j);
                packedBoxesIndexes.insert(i, b);
                break;
            }
        }
    }
}
