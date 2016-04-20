#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H

#include <QThread>
#include "containerproblemsolver.h"

class AlgorithmThread : public QThread
{
public:
    AlgorithmThread(QObject *parent = 0);
    void setArguments(ContainerProblemSolver *solver, ContainerProblem *problem, ContainerSolution *solution);
    void run();
    void cancel();

private:
    ContainerProblemSolver *containerProblemSolver;
    ContainerProblem *containerProblem;
    ContainerSolution *containerSolution;
};

#endif // ALGORITHMTHREAD_H
