#ifndef WORKERCONTAINERPROBLEMSOLVER_H
#define WORKERCONTAINERPROBLEMSOLVER_H

#include "containerproblemsolver.h"

class WorkerContainerProblemSolver : public QObject, protected ContainerProblemSolver
{
    Q_OBJECT
public:
    WorkerContainerProblemSolver(QObject *parent = 0);

public slots:
    void solveAsync(ContainerProblem *problem, ContainerSolution *solution);

signals:
    void executionStart();
    void executionEnd();
};

#endif // WORKERCONTAINERPROBLEMSOLVER_H
