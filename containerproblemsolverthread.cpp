#include "containerproblemsolverthread.h"
#include <QRect>

extern "C" {
void contload(int n, int W, int H, int D,
              int *w, int *h, int *d,
              int *x, int *y, int *z, int *k,
              int *vol);
}

ContainerProblemSolverThread::ContainerProblemSolverThread(QObject *parent)
    : QThread(parent)
{
}

void ContainerProblemSolverThread::setContainerProblem(ContainerProblem *problem)
{
    containerProblem = problem;
}

void ContainerProblemSolverThread::run()
{
    int volume = 0;
    int allBoxesQuantity = containerProblem->allBoxesQuantity();
    QVector<int> boxLengthsX(allBoxesQuantity);
    QVector<int> boxLengthsY(allBoxesQuantity);
    QVector<int> boxLengthsZ(allBoxesQuantity);
    QVector<int> boxGroups(allBoxesQuantity);
    int currentGroupIndex = 0;
    int currentGroupCount = 0;
    for (int index = 0; index < allBoxesQuantity; ++index)
    {
        boxLengthsX[index] = containerProblem->groupLengthX(currentGroupIndex);
        boxLengthsY[index] = containerProblem->groupLengthY(currentGroupIndex);
        boxLengthsZ[index] = containerProblem->groupLengthZ(currentGroupIndex);
        boxGroups[index] = currentGroupIndex;
        ++currentGroupCount;
        if (currentGroupCount == containerProblem->groupBoxesCounter(currentGroupIndex))
        {
            ++currentGroupIndex;
            currentGroupCount = 0;
        }
    }

    QVector<int> boxCoordinatesX(allBoxesQuantity);
    QVector<int> boxCoordinatesY(allBoxesQuantity);
    QVector<int> boxCoordinatesZ(allBoxesQuantity);
    QVector<int> boxPackedFlagsInt(allBoxesQuantity);
    QVector<int> packedBoxesIndexes;

    // llamar al procedimiento en C
    contload(allBoxesQuantity,
             containerProblem->containerLengthX(),
             containerProblem->containerLengthY(),
             containerProblem->containerLengthZ(),
             boxLengthsX.data(), boxLengthsY.data(), boxLengthsZ.data(),
             boxCoordinatesX.data(), boxCoordinatesY.data(), boxCoordinatesZ.data(),
             boxPackedFlagsInt.data(), &volume);

    // ordering of the boxes according to the explained algorithm
    std::sort(packedBoxesIndexes.begin(), packedBoxesIndexes.end(), [&](int a, int b) -> bool
    {
        if (boxCoordinatesZ[a] != boxCoordinatesZ[b])
            return boxCoordinatesZ[a] < boxCoordinatesZ[b];
        if (boxCoordinatesX[a] != boxCoordinatesX[b])
            return boxCoordinatesX[a] < boxCoordinatesX[b];
        return boxCoordinatesY[a] < boxCoordinatesY[b];
    });

    for (int j = 1; j < packedBoxesIndexes.size(); ++j)
    {
        int b = packedBoxesIndexes[j];
        for (int i = 0; i < j; ++i)
        {
            int a = packedBoxesIndexes[i];
            QRect rectBoxA(boxCoordinatesX[a], boxCoordinatesZ[a], boxLengthsX[a], boxLengthsZ[a]);
            QRect rectBoxB(boxCoordinatesX[b], boxCoordinatesZ[b], boxLengthsX[b], boxLengthsZ[b]);
            if (rectBoxA.intersects(rectBoxB) && boxCoordinatesY[b] < boxCoordinatesY[a])
            {
                packedBoxesIndexes.remove(j);
                packedBoxesIndexes.insert(i, b);
                break;
            }
        }
    }

    int packedBoxesCount = packedBoxesIndexes.size();
    SolutionData solutionData;
    solutionData.boxLengthXValues.resize(packedBoxesCount);
    solutionData.boxLengthYValues.resize(packedBoxesCount);
    solutionData.boxLengthZValues.resize(packedBoxesCount);
    solutionData.boxCoordXValues.resize(packedBoxesCount);
    solutionData.boxCoordYValues.resize(packedBoxesCount);
    solutionData.boxCoordZValues.resize(packedBoxesCount);
    solutionData.boxGroupIndexValues.resize(packedBoxesCount);

    for (int i = 0; i < packedBoxesIndexes.size(); ++i)
    {
        int boxIndex = packedBoxesIndexes[i];
        solutionData.boxLengthXValues[i] = boxLengthsX[boxIndex];
        solutionData.boxLengthYValues[i] = boxLengthsY[boxIndex];
        solutionData.boxLengthZValues[i] = boxLengthsZ[boxIndex];
        solutionData.boxCoordXValues[i] = boxCoordinatesX[boxIndex];
        solutionData.boxCoordYValues[i] = boxCoordinatesY[boxIndex];
        solutionData.boxCoordZValues[i] = boxCoordinatesZ[boxIndex];
        solutionData.boxGroupIndexValues[i] = boxGroups[boxIndex];
    }

    // TODO tratar de eliminar esta linea de codigo
    msleep(10);
    emit solutionReady(solutionData);
}
