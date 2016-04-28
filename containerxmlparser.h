#ifndef CONTAINERXMLPARSER_H
#define CONTAINERXMLPARSER_H

#include <QFile>
#include "containerproblem.h"
#include "containersolution.h"

class ContainerXmlParser
{
public:
    void writeProblem(const ContainerProblem &containerProblem, QFile *file);
    void readProblem(QFile *file, ContainerProblem &containerProblem);
    void writeSolution(const ContainerSolution &containerSolution, QFile *file);
    void readSolution(QFile *file, ContainerSolution &containerSolution);

private:
};

#endif // CONTAINERXMLPARSER_H
