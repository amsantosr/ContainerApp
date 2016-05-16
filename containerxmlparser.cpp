#include "containerxmlparser.h"
#include "containerxmlparserexception.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>

ContainerXmlParser::ContainerXmlParser()
{
}

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
        int boxIndex = containerSolution.packedBoxIndex(index);
        streamWriter.writeStartElement("Box");
        streamWriter.writeAttribute("Index", QString::number(boxIndex));
        streamWriter.writeEmptyElement("Position");
        int posX = containerSolution.packedBoxCoordinateX(index);
        int posY = containerSolution.packedBoxCoordinateY(index);
        int posZ = containerSolution.packedBoxCoordinateZ(index);
        streamWriter.writeAttribute("X", QString::number(posX));
        streamWriter.writeAttribute("Y", QString::number(posY));
        streamWriter.writeAttribute("Z", QString::number(posZ));
        streamWriter.writeEmptyElement("Dimensions");
        int x = containerSolution.packedBoxLengthX(index);
        int y = containerSolution.packedBoxLengthY(index);
        int z = containerSolution.packedBoxLengthZ(index);
        streamWriter.writeAttribute("X", QString::number(x));
        streamWriter.writeAttribute("Y", QString::number(y));
        streamWriter.writeAttribute("Z", QString::number(z));
        streamWriter.writeEndElement(); // Box
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
        int boxIndex;
        int positionX, positionY, positionZ;
        int dimensionX, dimensionY, dimensionZ;

        checkCurrentElement("Box");
        parsePackedBoxAttributes(boxIndex);
        checkNextElement("Position");
        parseBoxPositionAttributes(positionX, positionY, positionZ);
        streamReader.skipCurrentElement();
        checkNextElement("Dimensions");
        parseBoxDimensionsAttributes(dimensionX, dimensionY, dimensionZ);
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

void ContainerXmlParser::parseContainerProblemAttributes(ContainerProblem &containerProblem)
{
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
}

void ContainerXmlParser::parseContainerAttributes(ContainerProblem &containerProblem)
{
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
}

void ContainerXmlParser::parseBoxesAttributes(int &boxCount)
{
    bool flag = false;
    foreach (QXmlStreamAttribute attribute, streamReader.attributes())
    {
        if (attribute.name() == "BoxCount")
        {
            boxCount = checkIntegerRangeAttribute(attribute, 1, 1000);
            flag = true;
        }
        else
        {
            invalidAttribute(attribute);
        }
    }
    checkBoxAttributePresent(flag, "BoxCount");
}

void ContainerXmlParser::parseBoxAttributes(int boxIndex, int &boxDimensionX, int &boxDimensionY, int &boxDimensionZ)
{
    bool flags[4] = { 0 };
    foreach (QXmlStreamAttribute attribute, streamReader.attributes())
    {
        if (attribute.name() == "Index")
        {
            checkIntegerAttribute(attribute, boxIndex);
            flags[0] = true;
        }
        else if (attribute.name() == "LengthX")
        {
            boxDimensionX = checkIntegerRangeAttribute(attribute, 1, 9999);
            flags[1] = true;
        }
        else if (attribute.name() == "LengthY")
        {
            boxDimensionY = checkIntegerRangeAttribute(attribute, 1, 9999);
            flags[2] = true;
        }
        else if (attribute.name() == "LengthZ")
        {
            boxDimensionZ = checkIntegerRangeAttribute(attribute, 1, 9999);
            flags[3] = true;
        }
        else
        {
            invalidAttribute(attribute);
        }
    }
    checkBoxAttributePresent(flags[0], "Index");
    checkBoxAttributePresent(flags[1], "LengthX");
    checkBoxAttributePresent(flags[2], "LengthY");
    checkBoxAttributePresent(flags[3], "LengthZ");
}

void ContainerXmlParser::checkBoxAttributePresent(bool present, QString attributeName)
{
    if (!present)
    {
        throwException(QString("Falta el atributo '%2'").arg(attributeName));
    }
}

void ContainerXmlParser::readProblemElement(ContainerProblem &containerProblem)
{
    containerProblem.clear();

    checkNextElement("ContainerProblem");
    parseContainerProblemAttributes(containerProblem);
    checkNextElement("Container");
    parseContainerAttributes(containerProblem);
    streamReader.skipCurrentElement();
    checkNextElement("Boxes");
    int boxCount;
    parseBoxesAttributes(boxCount);
    containerProblem.setBoxCount(boxCount);

    for (int boxIndex = 0; boxIndex < boxCount; ++boxIndex)
    {
        checkNextElement("Box");
        int boxDimensionX, boxDimensionY, boxDimensionZ;
        parseBoxAttributes(boxIndex, boxDimensionX, boxDimensionY, boxDimensionZ);
        containerProblem.setBoxDimensions(boxIndex, boxDimensionX, boxDimensionY, boxDimensionZ);
        streamReader.skipCurrentElement();
    }
    checkEndElement("Boxes");
    checkEndElement("ContainerProblem");
}

void ContainerXmlParser::checkNextElement(QString name)
{
    if (!streamReader.readNextStartElement())
    {
        if (streamReader.hasError())
            throwException(streamReader.errorString());
        else
            throwException(QString("Falta el elemento '%1'.").arg(name));
    }
    checkCurrentElement(name);
}

void ContainerXmlParser::checkEndElement(QString name)
{
    if (streamReader.readNextStartElement())
        throwException(QString("Se esperaba el elemento de cierre </%1>.").arg(name));
    if (streamReader.hasError())
        throwException(streamReader.errorString());
}

void ContainerXmlParser::checkCurrentElement(QString name)
{
    if (streamReader.name() != name)
    {
        throwException(QString("Element desconocido '%1'.").arg(streamReader.name().toString()));
    }
}

void ContainerXmlParser::invalidAttribute(QXmlStreamAttribute &attribute)
{
    throwException(QString("Atributo '%1' inválido para el elemento '%2'.")
                   .arg(attribute.name().toString())
                   .arg(streamReader.name().toString()));
}

int ContainerXmlParser::parseIntegerAttribute(QXmlStreamAttribute &attribute)
{
    bool ok;
    int integer = attribute.value().toInt(&ok);
    if (!ok)
        throwException(QString("No se puede leer un valor entero en el atributo '%1'.").arg(attribute.name().toString()));
    return integer;
}

void ContainerXmlParser::checkIntegerAttribute(QXmlStreamAttribute &attribute, int value)
{
    int integer = parseIntegerAttribute(attribute);
    if (integer != value)
        throwException(QString("Valor inválido para el atributo '%1' (valor esperado '%2')")
                       .arg(attribute.name().toString())
                       .arg(value));
}

int ContainerXmlParser::checkIntegerRangeAttribute(QXmlStreamAttribute &attribute, int minValue, int maxValue)
{
    int value = parseIntegerAttribute(attribute);
    if (!(minValue <= value && value <= maxValue))
        throwException(QString("Valor inválido para el atributo '%1' (rango permitido [%2 %3])")
                       .arg(attribute.name().toString())
                       .arg(minValue)
                       .arg(maxValue));
    return value;
}

void ContainerXmlParser::parsePackedBoxAttributes(int &boxIndex)
{
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
}

void ContainerXmlParser::parseBoxPositionAttributes(int &positionX, int &positionY, int &positionZ)
{
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
}

void ContainerXmlParser::parseBoxDimensionsAttributes(int &dimensionX, int &dimensionY, int &dimensionZ)
{
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
}

void ContainerXmlParser::throwException(QString message)
{
    throw ContainerXmlParserException(QString("Línea %1, columna %2: %3")
                                      .arg(streamReader.lineNumber())
                                      .arg(streamReader.columnNumber())
                                      .arg(message));
}
