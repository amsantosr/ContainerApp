#ifndef CONTAINERPROBLEMSOLVERTHREAD_H
#define CONTAINERPROBLEMSOLVERTHREAD_H

#include <QThread>
#include "containerproblem.h"
#include "containersolution.h"

class ContainerProblemSolverThread : public QThread
{
    Q_OBJECT
public:
    explicit ContainerProblemSolverThread(QObject *parent = 0);
    void setContainerProblem(ProblemData problemData);
    void run();

signals:
    void solutionReady(SolutionData solution);

private:
    ProblemData problemData;
};

#endif // CONTAINERPROBLEMSOLVERTHREAD_H
