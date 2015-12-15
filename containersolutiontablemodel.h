#ifndef CONTAINERSOLUTIONTABLEMODEL_H
#define CONTAINERSOLUTIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

class ContainerSolutionTableModel : public QAbstractTableModel
{
public:
    ContainerSolutionTableModel(QObject *parent = 0);
    void initialize(QVector<int> boxLengthXvalues,
                    QVector<int> boxLengthYvalues,
                    QVector<int> boxLengthZvalues,
                    QVector<int> boxCoordinateXvalues,
                    QVector<int> boxCoordinateYvalues,
                    QVector<int> boxCoordinateZvalues,
                    QVector<bool> boxPackedFlagValues);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QVector<int> boxLengthXvalues;
    QVector<int> boxLengthYvalues;
    QVector<int> boxLengthZvalues;
    QVector<int> boxCoordinateXvalues;
    QVector<int> boxCoordinateYvalues;
    QVector<int> boxCoordinateZvalues;
    QVector<bool> boxPackedFlagValues;
};

#endif // CONTAINERSOLUTIONTABLEMODEL_H
