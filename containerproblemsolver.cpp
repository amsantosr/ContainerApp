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
    solution.setContainerLengthX(problem.containerLengthX());
    solution.setContainerLengthY(problem.containerLengthY());
    solution.setContainerLengthZ(problem.containerLengthZ());
    solution.boxLengthsX() = problem.boxLengthsX();
    solution.boxLengthsY() = problem.boxLengthsY();
    solution.boxLengthsZ() = problem.boxLengthsZ();

    solution.boxCoordinatesX().resize(problem.boxCount());
    solution.boxCoordinatesY().resize(problem.boxCount());
    solution.boxCoordinatesZ().resize(problem.boxCount());
    solution.boxPackedFlags().resize(problem.boxCount());
    QVector<int> boxPackedFlagsInt(problem.boxCount());

    // call the C procedure
    contload(problem.boxCount(),
             problem.containerLengthX(), problem.containerLengthY(), problem.containerLengthZ(),
             solution.boxLengthsX().data(), solution.boxLengthsY().data(), solution.boxLengthsZ().data(),
             solution.boxCoordinatesX().data(), solution.boxCoordinatesY().data(), solution.boxCoordinatesZ().data(),
             boxPackedFlagsInt.data(), &volume);

    for (int index = 0; index < boxPackedFlagsInt.size(); ++index)
    {
        solution.boxPackedFlags()[index] = (boxPackedFlagsInt[index] != 0);
    }

    // update the solutions
    solution.setPackedVolume(volume);
}
