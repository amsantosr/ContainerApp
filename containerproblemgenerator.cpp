#include "containerproblemgenerator.h"
#include "Pisinger/testcont.h"

void ContainerProblemGenerator::generate(int minimumLength, int maximumLength, int fillPercentage, int maximumDifferentBoxes, ContainerProblem &problem)
{
    const int MaxItems = 1000;
    item items[MaxItems];
    item *firstItemPointer = &items[0];
    item *lastItemPointer = &items[MaxItems - 1];
    // call the function to generate the test
    int containerLengthX, containerLengthY, containerLengthZ;
    maketest(firstItemPointer, &lastItemPointer, &containerLengthX, &containerLengthY, &containerLengthZ,
             minimumLength, maximumLength, fillPercentage, maximumDifferentBoxes);

    //prepareitems(firstItem, lastItem, Widths, Heights, Depths);

    problem.setContainerLengthX(containerLengthX);
    problem.setContainerLengthY(containerLengthY);
    problem.setContainerLengthZ(containerLengthZ);
    int itemCount = lastItemPointer - firstItemPointer + 1;
    problem.setBoxCount(itemCount);

    QVector<int> boxLengthsX(itemCount);
    QVector<int> boxLengthsY(itemCount);
    QVector<int> boxLengthsZ(itemCount);

    for (int index = 0; index < itemCount; ++index)
    {
        boxLengthsX[index] = items[index].dx;
        boxLengthsY[index] = items[index].dy;
        boxLengthsZ[index] = items[index].dz;
    }

    problem.setBoxLengthXValues(boxLengthsX);
    problem.setBoxLengthYValues(boxLengthsY);
    problem.setBoxLengthZValues(boxLengthsZ);
}
