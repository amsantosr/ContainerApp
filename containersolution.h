#ifndef CONTAINERSOLUTION_H
#define CONTAINERSOLUTION_H

#include <QObject>
#include <QVector>
#include "containerproblem.h"

struct SolutionData {
    QVector<int> boxLengthXVector;
    QVector<int> boxLengthYVector;
    QVector<int> boxLengthZVector;
    QVector<int> boxCoordXVector;
    QVector<int> boxCoordYVector;
    QVector<int> boxCoordZVector;
    QVector<int> boxGroupIndexVector;
};

class ContainerSolution : public QObject
{
    Q_OBJECT

public:
    ContainerSolution(QObject *parent = 0);
    void setContainerProblem(ContainerProblem *problem);
    const ContainerProblem *getContainerProblem() const { return containerProblem; }
    ContainerProblem *getContainerProblem() { return containerProblem; }
    void setSolutionData(const SolutionData &solutionData);
    void clear();
    int boxLengthX(int index) const { return solutionData.boxLengthXVector[index]; }
    int boxLengthY(int index) const { return solutionData.boxLengthYVector[index]; }
    int boxLengthZ(int index) const { return solutionData.boxLengthZVector[index]; }
    int boxCoordinateX(int index) const { return solutionData.boxCoordXVector[index]; }
    int boxCoordinateY(int index) const { return solutionData.boxCoordYVector[index]; }
    int boxCoordinateZ(int index) const { return solutionData.boxCoordZVector[index]; }
    int boxGroupIndex(int index) const { return solutionData.boxGroupIndexVector[index]; }
    int boxesCount() const { return solutionData.boxGroupIndexVector.size(); }

signals:
    void beforeDataChange();
    void afterDataChange();

private:
    ContainerProblem *containerProblem;
    SolutionData solutionData;
};

#endif // CONTAINERSOLUTION_H
