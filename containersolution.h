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
    int packedBoxLengthX(int index) const { return boxLengthXValues[index]; }
    int packedBoxLengthY(int index) const { return boxLengthYValues[index]; }
    int packedBoxLengthZ(int index) const { return boxLengthZValues[index]; }
    int packedBoxCoordinateX(int index) const { return boxCoordinateXValues[index]; }
    int packedBoxCoordinateY(int index) const { return boxCoordinateYValues[index]; }
    int packedBoxCoordinateZ(int index) const { return boxCoordinateZValues[index]; }
    bool isBoxPacked(int index) const { return boxPackedFlagValues[index]; }
    void setPackedBoxes(QVector<int> boxLengthsX, QVector<int> boxLengthsY, QVector<int> boxLengthsZ,
                        QVector<int> boxCoordinatesX, QVector<int> boxCoordinatesY, QVector<int> boxCoordinatesZ,
                        QVector<int> packedBoxesIndexes);
    int packedBoxesCount() const { return packedBoxesIndexes.size(); }
    int packedBoxIndex(int i) const { return packedBoxesIndexes[i]; }

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
    QVector<int> packedBoxesIndexes;
};

#endif // CONTAINERSOLUTION_H
