#ifndef CONTAINERPROBLEMSOLVERTHREAD_H
#define CONTAINERPROBLEMSOLVERTHREAD_H

#include <QThread>
#include "containerproblem.h"

class ContainerProblemSolverThread : public QThread
{
    Q_OBJECT
public:
    explicit ContainerProblemSolverThread(QObject *parent = 0);
    void setContainerProblem(ContainerProblem *problem);
    void run();

signals:
    void solutionReady(QVector<int> boxLengthsX,
                       QVector<int> boxLengthsY,
                       QVector<int> boxLengthsZ,
                       QVector<int> boxCoordinatesX,
                       QVector<int> boxCoordinatesY,
                       QVector<int> boxCoordinatesZ,
                       QVector<bool> boxPackedFlagsBool, int volume);

private:
    ContainerProblem *containerProblem;
};

#endif // CONTAINERPROBLEMSOLVERTHREAD_H
