#ifndef WORKERCONTAINERPROBLEMSOLVER_H
#define WORKERCONTAINERPROBLEMSOLVER_H

#include "containerproblemsolver.h"

class WorkerContainerProblemSolver : public QObject, protected ContainerProblemSolver
{
    Q_OBJECT
public:
    WorkerContainerProblemSolver(QObject *parent = 0);

public slots:
    void doWork(ContainerProblem *problem, ContainerSolution *solution);

signals:
    void workStarts();
    void workEnds();
};

#endif // WORKERCONTAINERPROBLEMSOLVER_H
