#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QVector>

class ContainerProblem
{
public:
    int containerLengthX() const { return containerLengthXValue; }
    int containerLengthY() const { return containerLengthYValue; }
    int containerLengthZ() const { return containerLengthZValue; }
    int boxCount() const { return boxCountValue; }
    int boxLengthX(int index) const { return boxLengthXValues[index]; }
    int boxLengthY(int index) const { return boxLengthYValues[index]; }
    int boxLengthZ(int index) const { return boxLengthZValues[index]; }
    const QVector<int> &boxLengthsX() const { return boxLengthXValues; }
    const QVector<int> &boxLengthsY() const { return boxLengthYValues; }
    const QVector<int> &boxLengthsZ() const { return boxLengthZValues; }

    void setContainerLengthX(int value) { containerLengthXValue = value; }
    void setContainerLengthY(int value) { containerLengthYValue = value; }
    void setContainerLengthZ(int value) { containerLengthZValue = value; }
    void setBoxCount(int value) { boxCountValue = value; }
    void setBoxLengthXValues(QVector<int> values) { boxLengthXValues = values; }
    void setBoxLengthYValues(QVector<int> values) { boxLengthYValues = values; }
    void setBoxLengthZValues(QVector<int> values) { boxLengthZValues = values; }

private:
    int containerLengthXValue;
    int containerLengthYValue;
    int containerLengthZValue;
    int boxCountValue;
    QVector<int> boxLengthXValues;
    QVector<int> boxLengthYValues;
    QVector<int> boxLengthZValues;
};

#endif // CONTAINERPROBLEM_H
