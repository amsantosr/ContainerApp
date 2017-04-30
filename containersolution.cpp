#include "containersolution.h"
#include <QRect>

ContainerSolution::ContainerSolution(QObject *parent)
    : QObject(parent),
      containerProblem(0)
{
}

void ContainerSolution::clear()
{
    if (!solutionData.boxLengthXValues.empty())
    {
        emit beforeDataChange();
        solutionData.boxLengthXValues.clear();
        solutionData.boxLengthYValues.clear();
        solutionData.boxLengthZValues.clear();
        solutionData.boxCoordXValues.clear();
        solutionData.boxCoordYValues.clear();
        solutionData.boxCoordZValues.clear();
        solutionData.boxGroupIndexValues.clear();
        emit afterDataChange();
    }
}

void ContainerSolution::setContainerProblem(ContainerProblem *problem)
{
    containerProblem = problem;
}

void ContainerSolution::setSolutionData(const SolutionData &solutionData)
{
    emit beforeDataChange();
    this->solutionData = solutionData;
    emit afterDataChange();
}
