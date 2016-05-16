#include "containerproblemsolver.h"

extern "C" {
void contload(int n, int W, int H, int D,
              int *w, int *h, int *d,
              int *x, int *y, int *z, int *k,
              int *vol);
}

void ContainerProblemSolver::solve(const ContainerProblem &problem, ContainerSolution &solution)
{
    int volume = 0;
    QVector<int> boxLengthsX = problem.boxLengthsX();
    QVector<int> boxLengthsY = problem.boxLengthsY();
    QVector<int> boxLengthsZ = problem.boxLengthsZ();
    QVector<int> boxCoordinatesX(problem.boxCount());
    QVector<int> boxCoordinatesY(problem.boxCount());
    QVector<int> boxCoordinatesZ(problem.boxCount());
    QVector<int> boxPackedFlagsInt(problem.boxCount());
    QVector<int> packedBoxesIndexes;

    // llamar al procedimiento en C
    contload(problem.boxCount(),
             problem.containerLengthX(), problem.containerLengthY(), problem.containerLengthZ(),
             boxLengthsX.data(), boxLengthsY.data(), boxLengthsZ.data(),
             boxCoordinatesX.data(), boxCoordinatesY.data(), boxCoordinatesZ.data(),
             boxPackedFlagsInt.data(), &volume);

    int packedBoxesCount = 0;
    for (int index = 0; index < boxPackedFlagsInt.size(); ++index)
    {
        if (boxPackedFlagsInt[index])
        {
            boxLengthsX[packedBoxesCount] = boxLengthsX[index];
            boxLengthsY[packedBoxesCount] = boxLengthsY[index];
            boxLengthsZ[packedBoxesCount] = boxLengthsZ[index];
            boxCoordinatesX[packedBoxesCount] = boxCoordinatesX[index];
            boxCoordinatesY[packedBoxesCount] = boxCoordinatesY[index];
            boxCoordinatesZ[packedBoxesCount] = boxCoordinatesZ[index];
            packedBoxesIndexes.append(index);
            ++packedBoxesCount;
        }
    }
    boxLengthsX.resize(packedBoxesCount);
    boxLengthsY.resize(packedBoxesCount);
    boxLengthsZ.resize(packedBoxesCount);
    boxCoordinatesX.resize(packedBoxesCount);
    boxCoordinatesY.resize(packedBoxesCount);
    boxCoordinatesZ.resize(packedBoxesCount);

    solution.setPackedBoxes(boxLengthsX, boxLengthsY, boxLengthsZ,
                            boxCoordinatesX, boxCoordinatesY, boxCoordinatesZ,
                            packedBoxesIndexes);
}
