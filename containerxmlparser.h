#ifndef CONTAINERXMLPARSER_H
#define CONTAINERXMLPARSER_H

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
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
    void writeProblemElement(const ContainerProblem &containerProblem);
    void readProblemElement(ContainerProblem &containerProblem);
    void checkNextElement(QString name);
    void checkCurrentElement(QString name);
    void invalidAttribute(QXmlStreamAttribute &attribute);
    int checkIntAttribute(QXmlStreamAttribute &attribute, int minValue, int maxValue);

private:
    QXmlStreamReader streamReader;
    QXmlStreamWriter streamWriter;
};

#endif // CONTAINERXMLPARSER_H
