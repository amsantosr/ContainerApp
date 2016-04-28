#include "containerxmlparser.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>

void ContainerXmlParser::writeProblem(const ContainerProblem &containerProblem, QFile *file)
{
    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("ContainerProblem");
    stream.writeAttribute("UnitLabel", containerProblem.textUnit());

    stream.writeStartElement("Container");
    QString dimensionX = QString::number(containerProblem.containerLengthX());
    QString dimensionY = QString::number(containerProblem.containerLengthY());
    QString dimensionZ = QString::number(containerProblem.containerLengthZ());
    stream.writeAttribute("DimensionX", dimensionX);
    stream.writeAttribute("DimensionY", dimensionY);
    stream.writeAttribute("DimensionZ", dimensionZ);
    stream.writeEndElement();

    stream.writeStartElement("Boxes");
    for (int index = 0; index < containerProblem.boxCount(); ++index)
    {
        stream.writeEmptyElement("Box");
        int lengthX = containerProblem.boxLengthX(index);
        int lengthY = containerProblem.boxLengthY(index);
        int lengthZ = containerProblem.boxLengthZ(index);
        stream.writeAttribute("DimensionX", QString::number(lengthX));
        stream.writeAttribute("DimensionY", QString::number(lengthY));
        stream.writeAttribute("DimensionZ", QString::number(lengthZ));
    }
    stream.writeEndDocument();
}

void ContainerXmlParser::readProblem(QFile *file, ContainerProblem &containerProblem)
{
    containerProblem.clear();
    QXmlStreamReader stream(file);
    while (!stream.atEnd())
    {
        if (stream.readNextStartElement())
        {
            if (stream.name() == "ContainerProblem")
            {
                foreach (QXmlStreamAttribute attribute, stream.attributes())
                {
                    if (attribute.name() == "UnitLabel")
                    {
                        containerProblem.setTextUnit(attribute.value().toString());
                    }
                }
            }
            else if (stream.name() == "Container")
            {
                foreach (QXmlStreamAttribute attribute, stream.attributes())
                {
                    if (attribute.name() == "DimensionX")
                    {
                        int dimensionX = attribute.value().toInt();
                        containerProblem.setContainerLengthX(dimensionX);
                    }
                    else if (attribute.name() == "DimensionY")
                    {
                        int dimensionY = attribute.value().toInt();
                        containerProblem.setContainerLengthY(dimensionY);
                    }
                    else if (attribute.name() == "DimensionZ")
                    {
                        int dimensionZ = attribute.value().toInt();
                        containerProblem.setContainerLengthZ(dimensionZ);
                    }
                }
            }
            else if (stream.name() == "Box")
            {
                int boxDimensionX, boxDimensionY, boxDimensionZ;
                foreach (QXmlStreamAttribute attribute, stream.attributes())
                {
                    if (attribute.name() == "DimensionX")
                    {
                        boxDimensionX = attribute.value().toInt();
                    }
                    else if (attribute.name() == "DimensionY")
                    {
                        boxDimensionY = attribute.value().toInt();
                    }
                    else if (attribute.name() == "DimensionZ")
                    {
                        boxDimensionZ = attribute.value().toInt();
                    }
                }
                containerProblem.addBox(boxDimensionX, boxDimensionY, boxDimensionZ);
            }
        }
    }
    if (stream.hasError())
    {
        // process xml errors here
    }
}

void ContainerXmlParser::writeSolution(const ContainerSolution &containerSolution, QFile *file)
{
    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("ContainerSolution");

    stream.writeStartElement("Boxes");
    for (int index = 0; index < containerSolution.boxCount(); ++index)
    {
        if (containerSolution.isBoxPacked(index))
        {
            stream.writeStartElement("Box");

            stream.writeEmptyElement("Position");
            int posX = containerSolution.boxCoordinateX(index);
            int posY = containerSolution.boxCoordinateY(index);
            int posZ = containerSolution.boxCoordinateZ(index);
            stream.writeAttribute("X", QString::number(posX));
            stream.writeAttribute("Y", QString::number(posY));
            stream.writeAttribute("Z", QString::number(posZ));

            stream.writeEmptyElement("Dimensions");
            int x = containerSolution.boxLengthX(index);
            int y = containerSolution.boxLengthZ(index);
            int z = containerSolution.boxLengthZ(index);
            stream.writeAttribute("X", QString::number(x));
            stream.writeAttribute("Y", QString::number(y));
            stream.writeAttribute("Z", QString::number(z));

            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
    stream.writeEndDocument();
}

void ContainerXmlParser::readSolution(QFile *file, ContainerSolution &containerSolution)
{
    containerSolution.clear();
    QXmlStreamReader stream(file);
    QVector<int> positionsX, positionsY, positionsZ;
    QVector<int> dimensionsX, dimensionsY, dimensionsZ;
    while (!stream.atEnd())
    {
        if (stream.readNextStartElement())
        {
            if (stream.name() == "Box")
            {
                while (stream.readNextStartElement())
                {
                    int positionX = 0, positionY = 0, positionZ = 0;
                    int dimensionX = 0, dimensionY = 0, dimensionZ = 0;
                    if (stream.name() == "Position")
                    {
                        foreach (QXmlStreamAttribute attribute, stream.attributes())
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
                        }
                    }
                    else if (stream.name() == "Dimensions")
                    {
                        foreach (QXmlStreamAttribute attribute, stream.attributes())
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
                        }
                    }
                    positionsX.append(positionX);
                    positionsY.append(positionY);
                    positionsZ.append(positionZ);
                    dimensionsX.append(dimensionX);
                    dimensionsY.append(dimensionY);
                    dimensionsZ.append(dimensionZ);
                }
            }
        }
    }
    if (stream.hasError())
    {
        // process xml errors here
    }
}
