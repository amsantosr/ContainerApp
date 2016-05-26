#include "containergroupstablemodel.h"

ContainerGroupsTableModel::ContainerGroupsTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      containerSolution(0),
      containerProblem(0)
{

}

void ContainerGroupsTableModel::setContainerSolution(ContainerSolution *solution)
{
    if (solution != containerSolution)
    {
        std::swap(containerSolution, solution);
        if (containerSolution != 0)
        {
            connect(containerSolution, &ContainerSolution::beforeDataChange,
                    this, &ContainerGroupsTableModel::beginResetModel);
            connect(containerSolution, &ContainerSolution::afterDataChange,
                    this, &ContainerGroupsTableModel::endResetModel);
            connect(containerSolution, &ContainerSolution::afterDataChange,
                    this, &ContainerGroupsTableModel::updateCounters);
        }
        if (solution != 0)
        {
            disconnect(solution, 0, this, 0);
        }
    }
    ContainerProblem *problem = containerSolution->getContainerProblem();
    if (containerProblem != problem)
    {
        std::swap(containerProblem, problem);
        containerProblem = containerSolution->getContainerProblem();
        if (containerProblem != 0)
        {
            connect(containerProblem, &ContainerProblem::beforeGroupsCounterChanged,
                    this, &ContainerGroupsTableModel::beginResetModel);
            connect(containerProblem, &ContainerProblem::afterGroupsCounterChanged,
                    this, &ContainerGroupsTableModel::endResetModel);
            connect(containerProblem, &ContainerProblem::afterGroupsCounterChanged,
                    this, &ContainerGroupsTableModel::updateCounters);
        }
        if (problem != 0)
            disconnect(problem, 0, this, 0);
    }
}

int ContainerGroupsTableModel::rowCount(const QModelIndex &parent) const
{
    return containerSolution != 0 ? containerSolution->getContainerProblem()->groupsCounter() : 0;
}

int ContainerGroupsTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant ContainerGroupsTableModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        switch (index.column())
        {
        case 0: result = QString::number(row + 1); break;
        case 1: result = containerProblem->groupDescription(index.row()); break;
        case 2: result = packedBoxesCounters[index.row()]; break;
        case 3: result = unpackedBoxesCounters[index.row()]; break;
        }
    }
    return result;
}

QVariant ContainerGroupsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: result = tr("Grupo"); break;
        case 1: result = tr("Descripción"); break;
        case 2: result = tr("Cajas empacadas"); break;
        case 3: result = tr("Cajas sin empacar"); break;
        }
    }
    else
        result = QAbstractTableModel::headerData(section, orientation, role);
    return result;
}

void ContainerGroupsTableModel::updateCounters()
{
    int size = containerProblem->groupsCounter();
    packedBoxesCounters.fill(0, size);
    unpackedBoxesCounters.fill(0, size);
    for (int index = 0; index < containerSolution->packedBoxesCount(); ++index)
    {
        int groupIndex = containerSolution->packedBoxGroupIndex(index);
        packedBoxesCounters[groupIndex]++;
    }
    for (int groupIndex = 0; groupIndex < containerProblem->groupsCounter(); ++groupIndex)
    {
        unpackedBoxesCounters[groupIndex] =
                containerProblem->groupBoxesCounter(groupIndex) - packedBoxesCounters[groupIndex];
    }
}
