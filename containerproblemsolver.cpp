#include "containerproblemsolver.h"
#include <QRect>
#include <QApplication>

extern "C" {
void contload(int n, int W, int H, int D,
              int *w, int *h, int *d,
              int *x, int *y, int *z, int *k,
              int *vol);
}

ContainerProblemSolver::ContainerProblemSolver(QObject *parent)
    : QObject(parent)
{
}

void ContainerProblemSolver::solveProblem(ProblemData problemData)
{
    int volume = 0;
    int allBoxesQuantity = std::accumulate(problemData.groupBoxesCountersVector.begin(),
                                           problemData.groupBoxesCountersVector.end(), 0);
    QVector<int> boxLengthsX(allBoxesQuantity);
    QVector<int> boxLengthsY(allBoxesQuantity);
    QVector<int> boxLengthsZ(allBoxesQuantity);
    QVector<int> boxGroupsIndexes(allBoxesQuantity);
    int currentGroupIndex = 0;
    int currentGroupCount = 0;
    for (int index = 0; index < allBoxesQuantity; ++index)
    {
        boxLengthsX[index] = problemData.groupLengthXVector[currentGroupIndex];
        boxLengthsY[index] = problemData.groupLengthYVector[currentGroupIndex];
        boxLengthsZ[index] = problemData.groupLengthZVector[currentGroupIndex];
        boxGroupsIndexes[index] = currentGroupIndex;
        ++currentGroupCount;
        if (currentGroupCount == problemData.groupBoxesCountersVector[currentGroupIndex])
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
             problemData.containerLengthX,
             problemData.containerLengthY,
             problemData.containerLengthZ,
             boxLengthsX.data(), boxLengthsY.data(), boxLengthsZ.data(),
             boxCoordinatesX.data(), boxCoordinatesY.data(), boxCoordinatesZ.data(),
             boxPackedFlagsInt.data(), &volume);

    for (int index = 0; index < boxPackedFlagsInt.size(); ++index)
    {
        if (boxPackedFlagsInt[index])
        {
            packedBoxesIndexes.append(index);
        }
    }

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
    solutionData.boxLengthXVector.resize(packedBoxesCount);
    solutionData.boxLengthYVector.resize(packedBoxesCount);
    solutionData.boxLengthZVector.resize(packedBoxesCount);
    solutionData.boxCoordXVector.resize(packedBoxesCount);
    solutionData.boxCoordYVector.resize(packedBoxesCount);
    solutionData.boxCoordZVector.resize(packedBoxesCount);
    solutionData.boxGroupIndexVector.resize(packedBoxesCount);

    for (int i = 0; i < packedBoxesIndexes.size(); ++i)
    {
        int boxIndex = packedBoxesIndexes[i];
        solutionData.boxLengthXVector[i] = boxLengthsX[boxIndex];
        solutionData.boxLengthYVector[i] = boxLengthsY[boxIndex];
        solutionData.boxLengthZVector[i] = boxLengthsZ[boxIndex];
        solutionData.boxCoordXVector[i] = boxCoordinatesX[boxIndex];
        solutionData.boxCoordYVector[i] = boxCoordinatesY[boxIndex];
        solutionData.boxCoordZVector[i] = boxCoordinatesZ[boxIndex];
        solutionData.boxGroupIndexVector[i] = boxGroupsIndexes[boxIndex];
    }
    emit solutionReady(solutionData);
}
