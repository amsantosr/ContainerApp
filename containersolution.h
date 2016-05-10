#ifndef CONTAINERSOLUTION_H
#define CONTAINERSOLUTION_H

#include <QObject>
#include <QVector>
#include "containerproblem.h"

class ContainerSolution : public QObject
{
    Q_OBJECT

public:
    ContainerSolution(QObject *parent = 0);
    void clear();
    void setContainerProblem(ContainerProblem *problem);
    const ContainerProblem *getContainerProblem() const { return containerProblem; }
    ContainerProblem *getContainerProblem() { return containerProblem; }
    QString textUnit() const { return containerProblem ? containerProblem->textUnit() : QString::null; }
    int boxLengthX(int index) const { return boxLengthXValues[index]; }
    int boxLengthY(int index) const { return boxLengthYValues[index]; }
    int boxLengthZ(int index) const { return boxLengthZValues[index]; }
    int boxCoordinateX(int index) const { return boxCoordinateXValues[index]; }
    int boxCoordinateY(int index) const { return boxCoordinateYValues[index]; }
    int boxCoordinateZ(int index) const { return boxCoordinateZValues[index]; }
    bool isBoxPacked(int index) const { return boxPackedFlagValues[index]; }
    int packedVolume() const { return packedVolumeValue; }
    int boxCount() const { return boxPackedFlagValues.size(); }

    void setSolution(QVector<int> boxLengthsX, QVector<int> boxLengthsY, QVector<int> boxLengthsZ,
                     QVector<int> boxCoordinatesX, QVector<int> boxCoordinatesY, QVector<int> boxCoordinatesZ,
                     QVector<bool> boxPackedFlagsBool, int volume);
    void setPackedBoxes(QVector<int> boxLengthsX, QVector<int> boxLengthsY, QVector<int> boxLengthsZ,
                        QVector<int> boxCoordinatesX, QVector<int> boxCoordinatesY, QVector<int> boxCoordinatesZ,
                        QVector<int> packedBoxesIndexes);
    int packedBoxesCount() const { return boxOrderIndexes.size(); }
    int boxOrderIndex(int i) const { return boxOrderIndexes[i]; }

signals:
    void beforeDataChange();
    void afterDataChange();

private:
    ContainerProblem *containerProblem;
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
