#ifndef CONTAINERPROBLEMSOLVERTHREAD_H
#define CONTAINERPROBLEMSOLVERTHREAD_H

#include "containerproblem.h"
#include "containersolution.h"

class ContainerProblemSolver : public QObject
{
    Q_OBJECT
public:
    explicit ContainerProblemSolver(QObject *parent = 0);
    void solveProblem(const ProblemData &problemData);

signals:
    void solutionReady(SolutionData solution);
};

#endif // CONTAINERPROBLEMSOLVERTHREAD_H
