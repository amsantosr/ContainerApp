#ifndef CONTAINERPROBLEMGENERATOR_H
#define CONTAINERPROBLEMGENERATOR_H

#include "containerproblem.h"

class ContainerProblemGenerator
{
public:
    void generate(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes, ContainerProblem &problem);
};

#endif // CONTAINERPROBLEMGENERATOR_H
