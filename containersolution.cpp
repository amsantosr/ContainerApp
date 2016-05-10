#include "containersolution.h"
#include <QRect>

ContainerSolution::ContainerSolution(QObject *parent)
    : QObject(parent),
      containerProblem(0),
      packedVolumeValue(0)
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
        boxOrderIndexes.clear();
        emit afterDataChange();
    }
    packedVolumeValue = 0;
}

void ContainerSolution::setContainerProblem(ContainerProblem *problem)
{
    containerProblem = problem;
}

void ContainerSolution::setSolution(QVector<int> boxLengthsX,
                                    QVector<int> boxLengthsY,
                                    QVector<int> boxLengthsZ,
                                    QVector<int> boxCoordinatesX,
                                    QVector<int> boxCoordinatesY,
                                    QVector<int> boxCoordinatesZ,
                                    QVector<bool> boxPackedFlagsBool, int volume)
{
    emit beforeDataChange();
    boxLengthXValues = boxLengthsX;
    boxLengthYValues = boxLengthsY;
    boxLengthZValues = boxLengthsZ;
    boxCoordinateXValues = boxCoordinatesX;
    boxCoordinateYValues = boxCoordinatesY;
    boxCoordinateZValues = boxCoordinatesZ;
    boxPackedFlagValues = boxPackedFlagsBool;
    packedVolumeValue = volume;

    boxOrderIndexes.clear();
    for (int index = 0; index < boxPackedFlagValues.size(); ++index)
    {
        if (isBoxPacked(index))
        {
            boxOrderIndexes.append(index);
        }
    }
    // ordering of the boxes according to the explained algorithm
    std::sort(boxOrderIndexes.begin(), boxOrderIndexes.end(), [&](int a, int b) -> bool
    {
        if (boxCoordinateZ(a) != boxCoordinateZ(b))
            return boxCoordinateZ(a) < boxCoordinateZ(b);
//        int posZa = boxCoordinateZ(a) + boxLengthZ(a);
//        int posZb = boxCoordinateZ(b) + boxLengthZ(b);
//        if (posZa != posZb)
//            return posZa < posZb;
        if (boxCoordinateX(a) != boxCoordinateX(b))
            return boxCoordinateX(a) < boxCoordinateX(b);
        return boxCoordinateY(a) < boxCoordinateY(b);
    });
    for (int j = 1; j < boxOrderIndexes.size(); ++j)
    {
        int b = boxOrderIndexes[j];
        for (int i = 0; i < j; ++i)
        {
            int a = boxOrderIndexes[i];
            QRect rectBoxA(boxCoordinateX(a), boxCoordinateZ(a), boxLengthX(a), boxLengthZ(a));
            QRect rectBoxB(boxCoordinateX(b), boxCoordinateZ(b), boxLengthX(b), boxLengthZ(b));
            if (rectBoxA.intersects(rectBoxB) && boxCoordinateY(b) < boxCoordinateY(a))
            {
                boxOrderIndexes.remove(j);
                boxOrderIndexes.insert(i, b);
                break;
            }
        }
    }
    emit afterDataChange();
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
    //packedVolumeValue = volume;
    boxOrderIndexes = packedBoxesIndexes;
    emit afterDataChange();
}
