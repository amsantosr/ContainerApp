#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QObject>
#include <QVector>
#include <QModelIndexList>
#include <QColor>
#include <algorithm>

class ContainerProblem : public QObject
{
    Q_OBJECT
public:
    ContainerProblem(QObject *parent = 0);
    int containerLengthX() const { return containerLengthXValue; }
    int containerLengthY() const { return containerLengthYValue; }
    int containerLengthZ() const { return containerLengthZValue; }
    int groupsCounter() const { return groupLengthXValues.size(); }
    int groupLengthX(int index) const { return groupLengthXValues[index]; }
    int groupLengthY(int index) const { return groupLengthYValues[index]; }
    int groupLengthZ(int index) const { return groupLengthZValues[index]; }
    int groupBoxesCounter(int index) const { return groupBoxesCounterValues[index]; }
    QColor groupColor(int index) const { return groupColorValues[index]; }
    QString groupName(int index) const { return groupDescriptionValues[index]; }
    void removeGroup(int index);
    int allBoxesQuantity() const { return std::accumulate(groupBoxesCounterValues.begin(), groupBoxesCounterValues.end(), 0); }

signals:
    void containerLengthXChanged(int value);
    void containerLengthYChanged(int value);
    void containerLengthZChanged(int value);
    void beforeAddGroup();
    void afterAddGroup();
    void beforeGroupsCounterChanged();
    void afterGroupsCounterChanged();
    void groupChanged(int index);

public:
    void addGroup(int lengthX, int lengthY, int lengthZ, int quantity, QColor color, QString description);
    void clear();
    void setGroupsCount(int count);
    void setGroup(int index, int quantity, int lengthX, int lengthY, int lengthZ, QColor color, QString description);
    void setGroupBoxCount(int groupIndex, int boxesCounter);
    void setContainerLengthX(int value);
    void setContainerLengthY(int value);
    void setContainerLengthZ(int value);

private:
    int containerLengthXValue;
    int containerLengthYValue;
    int containerLengthZValue;
    QVector<int> groupLengthXValues;
    QVector<int> groupLengthYValues;
    QVector<int> groupLengthZValues;
    QVector<int> groupBoxesCounterValues;
    QVector<QColor> groupColorValues;
    QVector<QString> groupDescriptionValues;
};

#endif // CONTAINERPROBLEM_H
