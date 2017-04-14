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
        int boxIndex = containerSolution.packedBoxGroupIndex(index);
        streamWriter.writeStartElement("Box");
        streamWriter.writeAttribute("GroupIndex", QString::number(boxIndex));
        streamWriter.writeEmptyElement("Position");
        int posX = containerSolution.packedBoxCoordinateX(index);
        int posY = containerSolution.packedBoxCoordinateY(index);
        int posZ = containerSolution.packedBoxCoordinateZ(index);
        streamWriter.writeAttribute("X", QString::number(posX));
        streamWriter.writeAttribute("Y", QString::number(posY));
        streamWriter.writeAttribute("Z", QString::number(posZ));
        streamWriter.writeEmptyElement("Orientation");
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
        int boxGroupIndex;
        int positionX, positionY, positionZ;
        int dimensionX, dimensionY, dimensionZ;

        checkCurrentElement("Box");
        parsePackedBoxAttributes(boxGroupIndex);
        checkNextElement("Position");
        parseBoxPositionAttributes(positionX, positionY, positionZ);
        streamReader.skipCurrentElement();
        checkNextElement("Orientation");
        parseBoxOrientationAttributes(dimensionX, dimensionY, dimensionZ);
        streamReader.skipCurrentElement();

        vectorPositionX.append(positionX);
        vectorPositionY.append(positionY);
        vectorPositionZ.append(positionZ);
        vectorDimensionX.append(dimensionX);
        vectorDimensionY.append(dimensionY);
        vectorDimensionZ.append(dimensionZ);
        packedBoxesIndexes.append(boxGroupIndex);
        streamReader.skipCurrentElement();
    }
    containerSolution.setPackedBoxes(vectorDimensionX, vectorDimensionY, vectorDimensionZ,
                                     vectorPositionX, vectorPositionY, vectorPositionZ,
                                     packedBoxesIndexes);
}

void ContainerXmlParser::writeProblemElement(const ContainerProblem &containerProblem)
{
    streamWriter.writeStartElement("ContainerProblem");

    streamWriter.writeEmptyElement("Container");
    QString dimensionX = QString::number(containerProblem.containerLengthX());
    QString dimensionY = QString::number(containerProblem.containerLengthY());
    QString dimensionZ = QString::number(containerProblem.containerLengthZ());
    streamWriter.writeAttribute("LengthX", dimensionX);
    streamWriter.writeAttribute("LengthY", dimensionY);
    streamWriter.writeAttribute("LengthZ", dimensionZ);

    streamWriter.writeStartElement("Groups");
    streamWriter.writeAttribute("Count", QString::number(containerProblem.groupsCounter()));
    for (int index = 0; index < containerProblem.groupsCounter(); ++index)
    {
        streamWriter.writeEmptyElement("Group");
        int lengthX = containerProblem.groupLengthX(index);
        int lengthY = containerProblem.groupLengthY(index);
        int lengthZ = containerProblem.groupLengthZ(index);
        int counter = containerProblem.groupBoxesCounter(index);
        streamWriter.writeAttribute("Index", QString::number(index));
        streamWriter.writeAttribute("BoxCount", QString::number(counter));
        streamWriter.writeAttribute("LengthX", QString::number(lengthX));
        streamWriter.writeAttribute("LengthY", QString::number(lengthY));
        streamWriter.writeAttribute("LengthZ", QString::number(lengthZ));
        streamWriter.writeAttribute("Color", containerProblem.groupColor(index).name());
        streamWriter.writeAttribute("Description", containerProblem.groupName(index));
    }
    streamWriter.writeEndElement(); // Groups
    streamWriter.writeEndElement(); // ContainerProblem
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
        if (attribute.name() == "Count")
        {
            boxCount = checkIntegerRangeAttribute(attribute, 1, 1000);
            flag = true;
        }
        else
        {
            invalidAttribute(attribute);
        }
    }
    checkBoxAttributePresent(flag, "Count");
}

void ContainerXmlParser::parseGroupAttributes(int &groupIndex, int &boxCount,
                                              int &boxDimensionX, int &boxDimensionY, int &boxDimensionZ,
                                              QColor &color, QString &description)
{
    bool flags[7] = { 0 };
    foreach (QXmlStreamAttribute attribute, streamReader.attributes())
    {
        if (attribute.name() == "Index")
        {
            groupIndex = parseIntegerAttribute(attribute);
            flags[0] = true;
        }
        else if (attribute.name() == "BoxCount")
        {
            boxCount = parseIntegerAttribute(attribute);
            flags[1] = true;
        }
        else if (attribute.name() == "LengthX")
        {
            boxDimensionX = checkIntegerRangeAttribute(attribute, 1, 9999);
            flags[2] = true;
        }
        else if (attribute.name() == "LengthY")
        {
            boxDimensionY = checkIntegerRangeAttribute(attribute, 1, 9999);
            flags[3] = true;
        }
        else if (attribute.name() == "LengthZ")
        {
            boxDimensionZ = checkIntegerRangeAttribute(attribute, 1, 9999);
            flags[4] = true;
        }
        else if (attribute.name() == "Color")
        {
            color = QColor(attribute.value().toString());
            if (!color.isValid())
                throwException(QString("Unable to parse color '%1'").arg(attribute.value().toString()));
            flags[5] = true;
        }
        else if (attribute.name() == "Description")
        {
            description = attribute.value().toString();
            flags[6] = true;
        }
        else
        {
            invalidAttribute(attribute);
        }
    }
    checkBoxAttributePresent(flags[0], "Index");
    checkBoxAttributePresent(flags[1], "BoxCount");
    checkBoxAttributePresent(flags[2], "LengthX");
    checkBoxAttributePresent(flags[3], "LengthY");
    checkBoxAttributePresent(flags[4], "LengthZ");
    checkBoxAttributePresent(flags[5], "Color");
    checkBoxAttributePresent(flags[6], "Description");
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
    checkNextElement("Container");
    parseContainerAttributes(containerProblem);
    streamReader.skipCurrentElement();
    checkNextElement("Groups");
    int groupCount;
    parseBoxesAttributes(groupCount);
    containerProblem.setGroupsCount(groupCount);

    for (int i = 0; i < groupCount; ++i)
    {
        checkNextElement("Group");
        int groupIndex, boxCount, lengthX, lengthY, lengthZ;
        QColor color;
        QString description;
        parseGroupAttributes(groupIndex, boxCount, lengthX, lengthY, lengthZ, color, description);
        containerProblem.setGroup(groupIndex, boxCount, lengthX, lengthY, lengthZ, color, description);
        streamReader.skipCurrentElement();
    }
    checkEndElement("Groups");
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
        if (attribute.name() == "GroupIndex")
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

void ContainerXmlParser::parseBoxOrientationAttributes(int &dimensionX, int &dimensionY, int &dimensionZ)
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
