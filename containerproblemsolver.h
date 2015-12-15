#ifndef CONTAINERPROBLEMSOLVER_H
#define CONTAINERPROBLEMSOLVER_H

#include "containerproblem.h"
#include "containersolution.h"

class ContainerProblemSolver
{
public:
    void solve(const ContainerProblem &problem, ContainerSolution &solution);
};

#endif // CONTAINERPROBLEMSOLVER_H
