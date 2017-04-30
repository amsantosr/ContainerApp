#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QObject>
#include <QVector>
#include <QModelIndexList>
#include <QColor>
#include <algorithm>

struct ProblemData {
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

class ContainerProblem : public QObject
{
    Q_OBJECT
public:
    ContainerProblem(QObject *parent = 0);
    const ProblemData &data() const { return problemData; }
    int containerLengthX() const { return problemData.containerLengthXValue; }
    int containerLengthY() const { return problemData.containerLengthYValue; }
    int containerLengthZ() const { return problemData.containerLengthZValue; }
    int groupsCounter() const { return problemData.groupLengthXValues.size(); }
    int groupLengthX(int index) const { return problemData.groupLengthXValues[index]; }
    int groupLengthY(int index) const { return problemData.groupLengthYValues[index]; }
    int groupLengthZ(int index) const { return problemData.groupLengthZValues[index]; }
    int groupBoxesCounter(int index) const { return problemData.groupBoxesCounterValues[index]; }
    QColor groupColor(int index) const { return problemData.groupColorValues[index]; }
    QString groupName(int index) const { return problemData.groupDescriptionValues[index]; }
    void removeGroup(int index);
    int allBoxesQuantity() const { return std::accumulate(problemData.groupBoxesCounterValues.begin(),
                                                          problemData.groupBoxesCounterValues.end(), 0); }

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
    void clear();
    void addGroup(int lengthX, int lengthY, int lengthZ, int quantity, QColor color, QString name);
    void setGroupsCount(int count);
    void setGroup(int index, int quantity, int lengthX, int lengthY, int lengthZ, QColor color, QString name);
    void setGroupBoxCount(int groupIndex, int boxesCounter);
    void setContainerLengthX(int value);
    void setContainerLengthY(int value);
    void setContainerLengthZ(int value);

private:
    ProblemData problemData;
};

#endif // CONTAINERPROBLEM_H
