#include "containerxmlparser.h"
#include "containerxmlparserexception.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>

void ContainerXmlParser::writeProblem(const ContainerProblem &containerProblem, QFile *file)
{
    streamWriter.setDevice(file);
    streamWriter.setAutoFormatting(true);
    streamWriter.writeStartDocument();
    writeProblemElement(containerProblem);
    streamWriter.writeEndDocument();
}

void ContainerXmlParser::readProblem(QFile *file, ContainerProblem &containerProblem)
{
    containerProblem.clear();
    streamReader.setDevice(file);
    readProblemElement(containerProblem);
}

void ContainerXmlParser::writeSolution(const ContainerSolution &containerSolution, QFile *file)
{
    streamWriter.setDevice(file);
    streamWriter.setAutoFormatting(true);

    streamWriter.writeStartDocument();
    streamWriter.writeStartElement("ContainerSolution");
    writeProblemElement(*containerSolution.getContainerProblem());
    streamWriter.writeStartElement("PackedBoxes");
    for (int index = 0; index < containerSolution.packedBoxesCount(); ++index)
    {
        streamWriter.writeStartElement("Box");
        streamWriter.writeAttribute("Index", QString::number(containerSolution.boxOrderIndex(index)));
        streamWriter.writeEmptyElement("Position");
        int posX = containerSolution.boxCoordinateX(index);
        int posY = containerSolution.boxCoordinateY(index);
        int posZ = containerSolution.boxCoordinateZ(index);
        streamWriter.writeAttribute("X", QString::number(posX));
        streamWriter.writeAttribute("Y", QString::number(posY));
        streamWriter.writeAttribute("Z", QString::number(posZ));

        streamWriter.writeEmptyElement("Dimensions");
        int x = containerSolution.boxLengthX(index);
        int y = containerSolution.boxLengthY(index);
        int z = containerSolution.boxLengthZ(index);
        streamWriter.writeAttribute("X", QString::number(x));
        streamWriter.writeAttribute("Y", QString::number(y));
        streamWriter.writeAttribute("Z", QString::number(z));

        streamWriter.writeEndElement();
    }
    streamWriter.writeEndElement(); // PackedBoxes
    streamWriter.writeEndElement(); // ContainerSolution
    streamWriter.writeEndDocument();
}

void ContainerXmlParser::readSolution(QFile *file, ContainerSolution &containerSolution)
{
    containerSolution.clear();
    streamReader.setDevice(file);

    checkNextElement("ContainerSolution");
    readProblemElement(*containerSolution.getContainerProblem());
    streamReader.skipCurrentElement();

    checkNextElement("PackedBoxes");
    QVector<int> packedBoxesIndexes;
    QVector<int> vectorPositionX;
    QVector<int> vectorPositionY;
    QVector<int> vectorPositionZ;
    QVector<int> vectorDimensionX;
    QVector<int> vectorDimensionY;
    QVector<int> vectorDimensionZ;

    while (streamReader.readNextStartElement())
    {
        checkCurrentElement("Box");
        int boxIndex;
        int positionX, positionY, positionZ;
        int dimensionX, dimensionY, dimensionZ;
        foreach (QXmlStreamAttribute attribute, streamReader.attributes())
        {
            if (attribute.name() == "Index")
            {
                boxIndex = attribute.value().toInt();
            }
            else
            {
                invalidAttribute(attribute);
            }
        }

        checkNextElement("Position");
        foreach (QXmlStreamAttribute attribute, streamReader.attributes())
        {
            if (attribute.name() == "X")
            {
                positionX = attribute.value().toInt();
            }
            else if (attribute.name() == "Y")
            {
                positionY = attribute.value().toInt();
            }
            else if (attribute.name() == "Z")
            {
                positionZ = attribute.value().toInt();
            }
            else
            {
                invalidAttribute(attribute);
            }
        }
        streamReader.skipCurrentElement();

        checkNextElement("Dimensions");
        foreach (QXmlStreamAttribute attribute, streamReader.attributes())
        {
            if (attribute.name() == "X")
            {
                dimensionX = attribute.value().toInt();
            }
            else if (attribute.name() == "Y")
            {
                dimensionY = attribute.value().toInt();
            }
            else if (attribute.name() == "Z")
            {
                dimensionZ = attribute.value().toInt();
            }
            else
            {
                invalidAttribute(attribute);
            }
        }
        streamReader.skipCurrentElement();

        vectorPositionX.append(positionX);
        vectorPositionY.append(positionY);
        vectorPositionZ.append(positionZ);
        vectorDimensionX.append(dimensionX);
        vectorDimensionY.append(dimensionY);
        vectorDimensionZ.append(dimensionZ);
        packedBoxesIndexes.append(boxIndex);
        streamReader.skipCurrentElement();
    }
    containerSolution.setPackedBoxes(vectorDimensionX, vectorDimensionY, vectorDimensionZ,
                                     vectorPositionX, vectorPositionY, vectorPositionZ,
                                     packedBoxesIndexes);
}

void ContainerXmlParser::writeProblemElement(const ContainerProblem &containerProblem)
{
    streamWriter.writeStartElement("ContainerProblem");
    streamWriter.writeAttribute("UnitLabel", containerProblem.textUnit());

    streamWriter.writeEmptyElement("Container");
    QString dimensionX = QString::number(containerProblem.containerLengthX());
    QString dimensionY = QString::number(containerProblem.containerLengthY());
    QString dimensionZ = QString::number(containerProblem.containerLengthZ());
    streamWriter.writeAttribute("LengthX", dimensionX);
    streamWriter.writeAttribute("LengthY", dimensionY);
    streamWriter.writeAttribute("LengthZ", dimensionZ);

    streamWriter.writeStartElement("Boxes");
    streamWriter.writeAttribute("BoxCount", QString::number(containerProblem.boxCount()));
    for (int index = 0; index < containerProblem.boxCount(); ++index)
    {
        streamWriter.writeEmptyElement("Box");
        int lengthX = containerProblem.boxLengthX(index);
        int lengthY = containerProblem.boxLengthY(index);
        int lengthZ = containerProblem.boxLengthZ(index);
        streamWriter.writeAttribute("Index", QString::number(index));
        streamWriter.writeAttribute("LengthX", QString::number(lengthX));
        streamWriter.writeAttribute("LengthY", QString::number(lengthY));
        streamWriter.writeAttribute("LengthZ", QString::number(lengthZ));
    }
    streamWriter.writeEndElement(); // Boxes
    streamWriter.writeEndElement(); // ContainerProblem
}

void ContainerXmlParser::readProblemElement(ContainerProblem &containerProblem)
{
    containerProblem.clear();
    checkNextElement("ContainerProblem");
    foreach (QXmlStreamAttribute attribute, streamReader.attributes())
    {
        if (attribute.name() == "UnitLabel")
        {
            containerProblem.setTextUnit(attribute.value().toString());
        }
        else
        {
            invalidAttribute(attribute);
        }
    }

    checkNextElement("Container");
    foreach (QXmlStreamAttribute attribute, streamReader.attributes())
    {
        if (attribute.name() == "LengthX")
        {
            int dimensionX = attribute.value().toInt();
            containerProblem.setContainerLengthX(dimensionX);
        }
        else if (attribute.name() == "LengthY")
        {
            int dimensionY = attribute.value().toInt();
            containerProblem.setContainerLengthY(dimensionY);
        }
        else if (attribute.name() == "LengthZ")
        {
            int dimensionZ = attribute.value().toInt();
            containerProblem.setContainerLengthZ(dimensionZ);
        }
        else
        {
            invalidAttribute(attribute);
        }
    }
    streamReader.skipCurrentElement();

    checkNextElement("Boxes");
    foreach (QXmlStreamAttribute attribute, streamReader.attributes())
    {
        if (attribute.name() == "BoxCount")
        {
            int boxCount = checkIntAttribute(attribute, 1, 1000);
            containerProblem.setBoxCount(boxCount);
        }
        else
        {
            invalidAttribute(attribute);
        }
    }
    while (streamReader.readNextStartElement())
    {
        checkCurrentElement("Box");
        int boxIndex, boxDimensionX, boxDimensionY, boxDimensionZ;
        foreach (QXmlStreamAttribute attribute, streamReader.attributes())
        {
            if (attribute.name() == "Index")
            {
                boxIndex = attribute.value().toInt();
            }
            else if (attribute.name() == "LengthX")
            {
                boxDimensionX = attribute.value().toInt();
            }
            else if (attribute.name() == "LengthY")
            {
                boxDimensionY = attribute.value().toInt();
            }
            else if (attribute.name() == "LengthZ")
            {
                boxDimensionZ = attribute.value().toInt();
            }
            else
            {
                invalidAttribute(attribute);
            }
        }
        //containerProblem.addBox(boxDimensionX, boxDimensionY, boxDimensionZ);
        containerProblem.setBoxDimensions(boxIndex, boxDimensionX, boxDimensionY, boxDimensionZ);
        streamReader.skipCurrentElement();
    }
    if (streamReader.hasError())
    {
        throw ContainerXmlParserException(streamReader.errorString());
    }
}

void ContainerXmlParser::checkNextElement(QString name)
{
    if (!streamReader.readNextStartElement())
    {
        throw ContainerXmlParserException(QString("Element '%1' expected at line %2")
                                          .arg(name)
                                          .arg(streamReader.lineNumber()));
    }
    checkCurrentElement(name);
}

void ContainerXmlParser::checkCurrentElement(QString name)
{
    if (streamReader.name() != name)
    {
        throw ContainerXmlParserException(QString("Unknown element '%1' at line %2")
                                          .arg(streamReader.name().toString())
                                          .arg(streamReader.lineNumber()));
    }
}

void ContainerXmlParser::invalidAttribute(QXmlStreamAttribute &attribute)
{
    throw ContainerXmlParserException(QString("Unknown attribute '%1' for element '%2' at line %3")
                                      .arg(attribute.name().toString())
                                      .arg(streamReader.name().toString())
                                      .arg(streamReader.lineNumber()));
}

int ContainerXmlParser::checkIntAttribute(QXmlStreamAttribute &attribute, int minValue, int maxValue)
{
    bool ok;
    int value = attribute.value().toInt(&ok);
    if (!ok)
        throw ContainerXmlParserException(QString("Integer value required in attribute '%1' at line '%2'")
                                          .arg(attribute.name().toString())
                                          .arg(streamReader.lineNumber()));
    if (!(minValue <= value && value <= maxValue))
        throw ContainerXmlParserException(QString("Value at attribute '%1' at line '%2'. Range allowed is [%3 %4]")
                                          .arg(attribute.name().toString())
                                          .arg(streamReader.lineNumber())
                                          .arg(minValue)
                                          .arg(maxValue));
    return value;
}
