#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QVector>

class ContainerProblem
{
public:
    int containerLengthX() const { return containerLengthXValue; }
    int containerLengthY() const { return containerLengthYValue; }
    int containerLengthZ() const { return containerLengthZValue; }
    int boxCount() const { return boxLengthXValues.size(); }
    int boxLengthX(int index) const { return boxLengthXValues[index]; }
    int boxLengthY(int index) const { return boxLengthYValues[index]; }
    int boxLengthZ(int index) const { return boxLengthZValues[index]; }
    const QVector<int> &boxLengthsX() const { return boxLengthXValues; }
    const QVector<int> &boxLengthsY() const { return boxLengthYValues; }
    const QVector<int> &boxLengthsZ() const { return boxLengthZValues; }

    void setContainerLengthX(int value) { containerLengthXValue = value; }
    void setContainerLengthY(int value) { containerLengthYValue = value; }
    void setContainerLengthZ(int value) { containerLengthZValue = value; }
    void addBox(int xLength, int yLength, int zLength)
    {
        boxLengthXValues.append(xLength);
        boxLengthYValues.append(yLength);
        boxLengthZValues.append(zLength);
    }
    void clear()
    {
        boxLengthXValues.clear();
        boxLengthYValues.clear();
        boxLengthZValues.clear();
        containerLengthXValue = 0;
        containerLengthYValue = 0;
        containerLengthZValue = 0;
    }

private:
    int containerLengthXValue;
    int containerLengthYValue;
    int containerLengthZValue;
    QVector<int> boxLengthXValues;
    QVector<int> boxLengthYValues;
    QVector<int> boxLengthZValues;
};

#endif // CONTAINERPROBLEM_H
