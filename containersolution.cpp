#include "containersolution.h"
#include <QRect>

void ContainerSolution::clear()
{
    if (!boxLengthXValues.empty())
    {
        emit beforeDataChange();
        containerLengthXvalue = 0;
        containerLengthYvalue = 0;
        containerLengthZvalue = 0;
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

void ContainerSolution::setContainerLengthX(int value)
{
    if (containerLengthXvalue != value)
    {
        emit beforeDataChange();
        containerLengthXvalue = value;
        emit afterDataChange();
    }
}

void ContainerSolution::setContainerLengthY(int value)
{
    if (containerLengthYvalue != value)
    {
        emit beforeDataChange();
        containerLengthYvalue = value;
        emit afterDataChange();
    }
}

void ContainerSolution::setContainerLengthZ(int value)
{
    if (containerLengthZvalue != value)
    {
        emit beforeDataChange();
        containerLengthZvalue = value;
        emit afterDataChange();
    }
}

void ContainerSolution::setSolution(int containerLengthX,
                                    int containerLengthY,
                                    int containerLengthZ,
                                    QVector<int> boxLengthsX,
                                    QVector<int> boxLengthsY,
                                    QVector<int> boxLengthsZ,
                                    QVector<int> boxCoordinatesX,
                                    QVector<int> boxCoordinatesY,
                                    QVector<int> boxCoordinatesZ,
                                    QVector<bool> boxPackedFlagsBool, int volume)
{
    emit beforeDataChange();
    containerLengthXvalue = containerLengthX;
    containerLengthYvalue = containerLengthY;
    containerLengthZvalue = containerLengthZ;
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
