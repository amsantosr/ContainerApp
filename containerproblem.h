#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QObject>
#include <QVector>

class ContainerProblem : public QObject
{
    Q_OBJECT
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

signals:
    void containerLengthX_changed(int value);
    void containerLengthY_changed(int value);
    void containerLengthZ_changed(int value);
    void beforeAddBox();
    void afterAddBox();
    void beforeBoxCountChanged();
    void afterBoxCountChanged();

public:
    void addBox(int xLength, int yLength, int zLength)
    {
        emit beforeAddBox();
        emit beforeBoxCountChanged();
        boxLengthXValues.append(xLength);
        boxLengthYValues.append(yLength);
        boxLengthZValues.append(zLength);
        emit afterBoxCountChanged();
        emit afterAddBox();
    }

    void clear()
    {
        if (boxLengthXValues.size() > 0)
        {
            emit beforeBoxCountChanged();
            boxLengthXValues.clear();
            boxLengthYValues.clear();
            boxLengthZValues.clear();
            emit afterBoxCountChanged();
        }
        setContainerLengthX(0);
        setContainerLengthY(0);
        setContainerLengthZ(0);
    }

public slots:
    void setContainerLengthX(int value)
    {
        if (containerLengthXValue != value)
        {
            containerLengthXValue = value;
            emit containerLengthX_changed(value);
        }
    }
    void setContainerLengthY(int value)
    {
        if (containerLengthYValue != value)
        {
            containerLengthYValue = value;
            emit containerLengthY_changed(value);
        }
    }
    void setContainerLengthZ(int value)
    {
        if (containerLengthZValue != value)
        {
            containerLengthZValue = value;
            emit containerLengthZ_changed(value);
        }
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
