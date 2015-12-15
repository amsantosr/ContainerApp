#ifndef CONTAINERPROBLEMTABLEMODEL_H
#define CONTAINERPROBLEMTABLEMODEL_H

#include <QAbstractTableModel>

class ContainerProblemTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ContainerProblemTableModel(QObject *parent = 0);
    void initialize(QList<int> lengthXValues, QList<int> lengthYValues, QList<int> lengthZValues);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<int> lengthXValues;
    QList<int> lengthYValues;
    QList<int> lengthZValues;
};

#endif // CONTAINERPROBLEMTABLEMODEL_H
