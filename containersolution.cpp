#include "containersolution.h"

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
    emit afterDataChange();
}
