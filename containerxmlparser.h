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
    ContainerXmlParser();
    void writeProblem(const ContainerProblem &containerProblem, QFile *file);
    void readProblem(QFile *file, ContainerProblem &containerProblem);
    void writeSolution(const ContainerSolution &containerSolution, QFile *file);
    void readSolution(QFile *file, ContainerSolution &containerSolution);

private:
    void writeProblemElement(const ContainerProblem &containerProblem);
    void readProblemElement(ContainerProblem &containerProblem);
    void checkNextElement(QString name);
    void checkEndElement(QString name);
    void checkCurrentElement(QString name);
    void invalidAttribute(QXmlStreamAttribute &attribute);
    int parseIntegerAttribute(QXmlStreamAttribute &attribute);
    void checkIntegerAttribute(QXmlStreamAttribute &attribute, int value);
    int checkIntegerRangeAttribute(QXmlStreamAttribute &attribute, int minValue, int maxValue);
    void parseContainerProblemAttributes(ContainerProblem &containerProblem);
    void parseContainerAttributes(ContainerProblem &containerProblem);
    void parseBoxesAttributes(int &boxCount);
    void parseGroupAttributes(int &groupIndex, int &boxCount, int &boxDimensionX, int &boxDimensionY, int &boxDimensionZ, QColor &color, QString &description);
    void checkBoxAttributePresent(bool present, QString attributeName);
    void parsePackedBoxAttributes(int &boxIndex);
    void parseBoxPositionAttributes(int &positionX, int &positionY, int &positionZ);
    void parseBoxOrientationAttributes(int &dimensionX, int &dimensionY, int &dimensionZ);
    void throwException(QString message);

private:
    QXmlStreamReader streamReader;
    QXmlStreamWriter streamWriter;
};

#endif // CONTAINERXMLPARSER_H
