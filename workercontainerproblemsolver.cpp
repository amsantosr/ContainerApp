#include "workercontainerproblemsolver.h"
#include "mainwindow.h"

WorkerContainerProblemSolver::WorkerContainerProblemSolver(QObject *parent)
    : QObject(parent)
{

}

void WorkerContainerProblemSolver::solveAsync(ContainerProblem *problem, ContainerSolution *solution)
{
    emit executionStart();
    solve(*problem, *solution);
    emit executionEnd();
}
