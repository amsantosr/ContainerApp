#ifndef CONTAINERSOLUTION_H
#define CONTAINERSOLUTION_H

#include <QObject>
#include <QVector>
#include "containerproblem.h"

struct SolutionData {
    QVector<int> boxLengthXValues;
    QVector<int> boxLengthYValues;
    QVector<int> boxLengthZValues;
    QVector<int> boxCoordXValues;
    QVector<int> boxCoordYValues;
    QVector<int> boxCoordZValues;
    QVector<int> boxGroupIndexValues;
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
    int boxLengthX(int index) const { return solutionData.boxLengthXValues[index]; }
    int boxLengthY(int index) const { return solutionData.boxLengthYValues[index]; }
    int boxLengthZ(int index) const { return solutionData.boxLengthZValues[index]; }
    int boxCoordinateX(int index) const { return solutionData.boxCoordXValues[index]; }
    int boxCoordinateY(int index) const { return solutionData.boxCoordYValues[index]; }
    int boxCoordinateZ(int index) const { return solutionData.boxCoordZValues[index]; }
    int boxGroupIndex(int index) const { return solutionData.boxGroupIndexValues[index]; }
    int boxesCount() const { return solutionData.boxGroupIndexValues.size(); }

signals:
    void beforeDataChange();
    void afterDataChange();

private:
    ContainerProblem *containerProblem;
    SolutionData solutionData;
};

#endif // CONTAINERSOLUTION_H
