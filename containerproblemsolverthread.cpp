#include "containerproblemsolverthread.h"

ContainerProblemSolverThread::ContainerProblemSolverThread(QObject *parent)
    : QThread(parent),
      containerProblem(0),
      containerSolution(0)
{

}

void ContainerProblemSolverThread::setParameters(ContainerProblem *problem, ContainerSolution *solution)
{
    containerProblem = problem;
    containerSolution = solution;
}

void ContainerProblemSolverThread::run()
{
    if (containerProblem != 0 && containerSolution != 0)
        containerProblemSolver.solve(*containerProblem, *containerSolution);
}
