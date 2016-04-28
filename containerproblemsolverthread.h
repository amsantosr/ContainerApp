#ifndef CONTAINERPROBLEMSOLVERTHREAD_H
#define CONTAINERPROBLEMSOLVERTHREAD_H

#include <QThread>
#include "containerproblemsolver.h"

class ContainerProblemSolverThread : public QThread
{
    Q_OBJECT
public:
    explicit ContainerProblemSolverThread(QObject *parent = 0);
    void setParameters(ContainerProblem *problem, ContainerSolution *solution);
    void run();

signals:

public slots:

private:
    ContainerProblemSolver containerProblemSolver;
    ContainerProblem *containerProblem;
    ContainerSolution *containerSolution;
};

#endif // CONTAINERPROBLEMSOLVERTHREAD_H
