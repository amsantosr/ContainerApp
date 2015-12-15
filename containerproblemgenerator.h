#ifndef CONTAINERPROBLEMGENERATOR_H
#define CONTAINERPROBLEMGENERATOR_H

#include "containerproblem.h"

class ContainerProblemGenerator
{
public:
    void generate(int minimumLength, int maximumLength, int fillPercentage, int maximumDifferentBoxes, ContainerProblem &problem);
};

#endif // CONTAINERPROBLEMGENERATOR_H
