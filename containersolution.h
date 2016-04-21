#ifndef CONTAINERSOLUTION_H
#define CONTAINERSOLUTION_H

#include <QObject>
#include <QVector>
#include "containerproblem.h"

class ContainerSolution : public QObject
{
    Q_OBJECT
signals:
    void beforeDataChange();
    void afterDataChange();

public:
    void clear();
    int containerLengthX() const { return containerLengthXvalue; }
    int containerLengthY() const { return containerLengthYvalue; }
    int containerLengthZ() const { return containerLengthZvalue; }
    int boxLengthX(int index) const { return boxLengthXValues[index]; }
    int boxLengthY(int index) const { return boxLengthYValues[index]; }
    int boxLengthZ(int index) const { return boxLengthZValues[index]; }
    int boxCoordinateX(int index) const { return boxCoordinateXValues[index]; }
    int boxCoordinateY(int index) const { return boxCoordinateYValues[index]; }
    int boxCoordinateZ(int index) const { return boxCoordinateZValues[index]; }
    bool isBoxPacked(int index) const { return boxPackedFlagValues[index]; }
    int packedVolume() const { return packedVolumeValue; }
    int boxCount() const { return boxPackedFlagValues.size(); }

    void setSolution(int containerLengthX, int containerLengthY, int containerLengthZ,
                     QVector<int> boxLengthsX, QVector<int> boxLengthsY, QVector<int> boxLengthsZ,
                     QVector<int> boxCoordinatesX, QVector<int> boxCoordinatesY, QVector<int> boxCoordinatesZ,
                     QVector<bool> boxPackedFlagsBool, int volume);
    int packedBoxesCount() const { return boxOrderIndexes.size(); }
    int boxOrderIndex(int i) const { return boxOrderIndexes[i]; }
    void setContainerLengthX(int value);
    void setContainerLengthY(int value);
    void setContainerLengthZ(int value);

private:
    int containerLengthXvalue;
    int containerLengthYvalue;
    int containerLengthZvalue;
    QVector<int> boxLengthXValues;
    QVector<int> boxLengthYValues;
    QVector<int> boxLengthZValues;
    QVector<int> boxCoordinateXValues;
    QVector<int> boxCoordinateYValues;
    QVector<int> boxCoordinateZValues;
    QVector<bool> boxPackedFlagValues;
    QVector<int> boxOrderIndexes;
    int packedVolumeValue;
};

#endif // CONTAINERSOLUTION_H
