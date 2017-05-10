#include "containersolution.h"
#include <QRect>

ContainerSolution::ContainerSolution(QObject *parent)
    : QObject(parent),
      containerProblem(0)
{
}

void ContainerSolution::clear()
{
    if (!solutionData.boxLengthXVector.empty())
    {
        emit beforeDataChange();
        solutionData.boxLengthXVector.clear();
        solutionData.boxLengthYVector.clear();
        solutionData.boxLengthZVector.clear();
        solutionData.boxCoordXVector.clear();
        solutionData.boxCoordYVector.clear();
        solutionData.boxCoordZVector.clear();
        solutionData.boxGroupIndexVector.clear();
        emit afterDataChange();
    }
}

void ContainerSolution::setContainerProblem(ContainerProblem *problem)
{
    std::swap(containerProblem, problem);
}

void ContainerSolution::setSolutionData(const SolutionData &solutionData)
{
    emit beforeDataChange();
    this->solutionData = solutionData;
    emit afterDataChange();
}
