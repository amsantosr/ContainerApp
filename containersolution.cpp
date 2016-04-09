#include "containersolution.h"
#include <QRect>

void ContainerSolution::clear()
{
    if (!boxLengthXvalues.empty())
    {
        emit beforeDataChange();
        containerLengthXvalue = 0;
        containerLengthYvalue = 0;
        containerLengthZvalue = 0;
        boxLengthXvalues.clear();
        boxLengthYvalues.clear();
        boxLengthZvalues.clear();
        boxCoordinateXvalues.clear();
        boxCoordinateYvalues.clear();
        boxCoordinateZvalues.clear();
        boxPackedFlagValues.clear();
        boxOrderIndexes.clear();
        emit afterDataChange();
    }
    packedVolumeValue = 0;
}

void ContainerSolution::setProblem(const ContainerProblem &problem)
{
    emit beforeDataChange();
    containerLengthXvalue = problem.containerLengthX();
    containerLengthYvalue = problem.containerLengthY();
    containerLengthZvalue = problem.containerLengthZ();
    boxLengthXvalues = problem.boxLengthsX();
    boxLengthYvalues = problem.boxLengthsY();
    boxLengthZvalues = problem.boxLengthsZ();
    boxCoordinateXvalues.resize(problem.boxCount());
    boxCoordinateYvalues.resize(problem.boxCount());
    boxCoordinateZvalues.resize(problem.boxCount());
    boxPackedFlagValues.resize(problem.boxCount());
    emit afterDataChange();
}

void ContainerSolution::setSolution(QVector<int> boxLengthsX, QVector<int> boxLengthsY, QVector<int> boxLengthsZ,
                 QVector<int> boxCoordinatesX, QVector<int> boxCoordinatesY, QVector<int> boxCoordinatesZ,
                 QVector<bool> boxPackedFlagsBool, int volume)
{
    emit beforeDataChange();
    boxLengthXvalues = boxLengthsX;
    boxLengthYvalues = boxLengthsY;
    boxLengthZvalues = boxLengthsZ;
    boxCoordinateXvalues = boxCoordinatesX;
    boxCoordinateYvalues = boxCoordinatesY;
    boxCoordinateZvalues = boxCoordinatesZ;
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
    std::sort(boxOrderIndexes.begin(), boxOrderIndexes.end(), [&](int a, int b) -> bool
    {
        if (boxCoordinateZ(a) != boxCoordinateZ(b))
            return boxCoordinateZ(a) < boxCoordinateZ(b);
        if (boxCoordinateY(a) != boxCoordinateY(b))
            return boxCoordinateY(a) < boxCoordinateY(b);
        return boxCoordinateX(a) < boxCoordinateX(b);
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
