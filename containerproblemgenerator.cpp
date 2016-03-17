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

    for (int index = 0; index < itemCount; ++index)
    {
        problem.addBox(items[index].dx, items[index].dy, items[index].dz);
    }
}
