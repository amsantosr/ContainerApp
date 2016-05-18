#ifndef CONTAINERPROBLEM_H
#define CONTAINERPROBLEM_H

#include <QObject>
#include <QVector>
#include <QModelIndexList>
#include <QColor>

class ContainerProblem : public QObject
{
    Q_OBJECT
public:
    ContainerProblem(QObject *parent = 0);
    int containerLengthX() const { return containerLengthXValue; }
    int containerLengthY() const { return containerLengthYValue; }
    int containerLengthZ() const { return containerLengthZValue; }
    QString textUnit() const { return textUnitValue; }
    int boxCount() const { return boxLengthXValues.size(); }
    int boxLengthX(int index) const { return boxLengthXValues[index]; }
    int boxLengthY(int index) const { return boxLengthYValues[index]; }
    int boxLengthZ(int index) const { return boxLengthZValues[index]; }
    int boxQuantity(int index) const { return boxQuantityValues[index]; }
    QColor boxColor(int index) const { return boxColorValues[index]; }
    QString boxDescription(int index) const { return boxDescriptionValues[index]; }
    const QVector<int> &boxLengthsX() const { return boxLengthXValues; }
    const QVector<int> &boxLengthsY() const { return boxLengthYValues; }
    const QVector<int> &boxLengthsZ() const { return boxLengthZValues; }
    void removeBoxes(QModelIndexList indexes);
    int allBoxesQuantity() const { return std::accumulate(boxQuantityValues.begin(), boxQuantityValues.end(), 0); }

signals:
    void containerLengthX_changed(int value);
    void containerLengthY_changed(int value);
    void containerLengthZ_changed(int value);
    void textUnit_changed(QString value);
    void beforeAddBox();
    void afterAddBox();
    void beforeBoxCountChanged();
    void afterBoxCountChanged();
    void boxValuesChanged(int index);

public:
    void addBox(int lengthX, int lengthY, int lengthZ, int quantity, QColor color, QString description);
    void clear();
    void setBoxCount(int count);
    void setBox(int boxIndex, int lengthX, int lengthY, int lengthZ, int quantity, QColor color, QString description);
    void setBoxQuantity(int boxIndex, int quantity);
    void setContainerLengthX(int value);
    void setContainerLengthY(int value);
    void setContainerLengthZ(int value);
    void setTextUnit(QString value);

private:
    int containerLengthXValue;
    int containerLengthYValue;
    int containerLengthZValue;
    QVector<int> boxLengthXValues;
    QVector<int> boxLengthYValues;
    QVector<int> boxLengthZValues;
    QVector<int> boxQuantityValues;
    QVector<QColor> boxColorValues;
    QVector<QString> boxDescriptionValues;
    QString textUnitValue;
};

#endif // CONTAINERPROBLEM_H
