#include "algorithmthread.h"
#include "mainwindow.h"

AlgorithmThread::AlgorithmThread(QObject *parent)
    : QThread(parent), containerProblemSolver(0)
{

}

void AlgorithmThread::setArguments(ContainerProblemSolver *solver,
                                                ContainerProblem *problem,
                                                ContainerSolution *solution)
{
    containerProblemSolver = solver;
    containerProblem = problem;
    containerSolution = solution;
}

void AlgorithmThread::run()
{
    if (containerProblemSolver != 0 && containerProblem != 0 && containerSolution != 0)
        containerProblemSolver->solve(*containerProblem, *containerSolution);
}

void AlgorithmThread::cancel()
{

}
