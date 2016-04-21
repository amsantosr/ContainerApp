#include "workercontainerproblemsolver.h"
#include "mainwindow.h"

WorkerContainerProblemSolver::WorkerContainerProblemSolver(QObject *parent)
    : QObject(parent)
{

}

void WorkerContainerProblemSolver::doWork(ContainerProblem *problem, ContainerSolution *solution)
{
    emit workStarts();
    solve(*problem, *solution);
    emit workEnds();
}
