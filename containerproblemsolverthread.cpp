#include "containerproblemsolverthread.h"

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
    QVector<int> boxLengthsX = containerProblem->boxLengthsX();
    QVector<int> boxLengthsY = containerProblem->boxLengthsY();
    QVector<int> boxLengthsZ = containerProblem->boxLengthsZ();
    QVector<int> boxCoordinatesX(containerProblem->boxCount());
    QVector<int> boxCoordinatesY(containerProblem->boxCount());
    QVector<int> boxCoordinatesZ(containerProblem->boxCount());
    QVector<int> boxPackedFlagsInt(containerProblem->boxCount());
    QVector<bool> boxPackedFlagsBool(containerProblem->boxCount());

    // llamar al procedimiento en C
    contload(containerProblem->boxCount(),
             containerProblem->containerLengthX(),
             containerProblem->containerLengthY(),
             containerProblem->containerLengthZ(),
             boxLengthsX.data(), boxLengthsY.data(), boxLengthsZ.data(),
             boxCoordinatesX.data(), boxCoordinatesY.data(), boxCoordinatesZ.data(),
             boxPackedFlagsInt.data(), &volume);

    for (int index = 0; index < boxPackedFlagsInt.size(); ++index)
    {
        boxPackedFlagsBool[index] = (boxPackedFlagsInt[index] != 0);
    }

    emit solutionReady(boxLengthsX, boxLengthsY, boxLengthsZ,
                       boxCoordinatesX, boxCoordinatesY, boxCoordinatesZ,
                       boxPackedFlagsBool, volume);
    // TODO tratar de eliminar esta linea de codigo
    msleep(10);
}
