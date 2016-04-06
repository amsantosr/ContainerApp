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
    void setProblem(const ContainerProblem &problem);
    int containerLengthX() const { return containerLengthXvalue; }
    int containerLengthY() const { return containerLengthYvalue; }
    int containerLengthZ() const { return containerLengthZvalue; }
    int boxLengthX(int index) const { return boxLengthXvalues[index]; }
    int boxLengthY(int index) const { return boxLengthYvalues[index]; }
    int boxLengthZ(int index) const { return boxLengthZvalues[index]; }
    int boxCoordinateX(int index) const { return boxCoordinateXvalues[index]; }
    int boxCoordinateY(int index) const { return boxCoordinateYvalues[index]; }
    int boxCoordinateZ(int index) const { return boxCoordinateZvalues[index]; }
    bool isBoxPacked(int index) const { return boxPackedFlagValues[index]; }
    int packedVolume() const { return packedVolumeValue; }
    int boxCount() const { return boxPackedFlagValues.size(); }
    void setSolution(QVector<int> boxLengthsX, QVector<int> boxLengthsY, QVector<int> boxLengthsZ,
                     QVector<int> boxCoordinatesX, QVector<int> boxCoordinatesY, QVector<int> boxCoordinatesZ,
                     QVector<bool> boxPackedFlagsBool, int volume);

private:
    int containerLengthXvalue;
    int containerLengthYvalue;
    int containerLengthZvalue;
    QVector<int> boxLengthXvalues;
    QVector<int> boxLengthYvalues;
    QVector<int> boxLengthZvalues;
    QVector<int> boxCoordinateXvalues;
    QVector<int> boxCoordinateYvalues;
    QVector<int> boxCoordinateZvalues;
    QVector<bool> boxPackedFlagValues;
    int packedVolumeValue;
};

#endif // CONTAINERSOLUTION_H
