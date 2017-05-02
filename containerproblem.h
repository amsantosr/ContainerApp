#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QObject>
#include <QVector>
#include <QModelIndexList>
#include <QColor>
#include <algorithm>

struct ProblemData {
    int containerLengthX;
    int containerLengthY;
    int containerLengthZ;
    QVector<int> groupLengthXVector;
    QVector<int> groupLengthYVector;
    QVector<int> groupLengthZVector;
    QVector<int> groupBoxesCountersVector;
    QVector<QColor> groupColorVector;
    QVector<QString> groupNamesVector;
};

class ContainerProblem : public QObject
{
    Q_OBJECT
public:
    ContainerProblem(QObject *parent = 0);
    const ProblemData &data() const { return problemData; }
    int containerLengthX() const { return problemData.containerLengthX; }
    int containerLengthY() const { return problemData.containerLengthY; }
    int containerLengthZ() const { return problemData.containerLengthZ; }
    int groupsCounter() const { return problemData.groupBoxesCountersVector.size(); }
    int groupLengthX(int index) const { return problemData.groupLengthXVector[index]; }
    int groupLengthY(int index) const { return problemData.groupLengthYVector[index]; }
    int groupLengthZ(int index) const { return problemData.groupLengthZVector[index]; }
    int groupBoxesCounter(int index) const { return problemData.groupBoxesCountersVector[index]; }
    QColor groupColor(int index) const { return problemData.groupColorVector[index]; }
    QString groupName(int index) const { return problemData.groupNamesVector[index]; }
    void removeGroup(int index);
    int allBoxesQuantity() const { return std::accumulate(problemData.groupBoxesCountersVector.begin(),
                                                          problemData.groupBoxesCountersVector.end(), 0); }

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
    void setGroup(int index, int lengthX, int lengthY, int lengthZ, int quantity, QColor color, QString name);
    void setGroupBoxCount(int groupIndex, int boxesCounter);
    void setContainerLengthX(int value);
    void setContainerLengthY(int value);
    void setContainerLengthZ(int value);

private:
    ProblemData problemData;
};

#endif // CONTAINERPROBLEM_H
