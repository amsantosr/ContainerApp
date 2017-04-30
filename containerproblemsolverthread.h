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
    void setContainerProblem(ContainerProblem *problem);
    void run();

signals:
    void solutionReady(SolutionData solution);

private:
    ContainerProblem *containerProblem;
};

#endif // CONTAINERPROBLEMSOLVERTHREAD_H
