#include "containerproblemgenerator.h"
#include "Pisinger/testcont.h"

void ContainerProblemGenerator::generate(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes, ContainerProblem &problem)
{
    const int MaxItems = 1000;
    item items[MaxItems];
    item *firstItemPointer = &items[0];
    item *lastItemPointer = &items[MaxItems - 1];
    // call the function to generate the test
    int containerLengthX, containerLengthY, containerLengthZ;
    maketest(firstItemPointer, &lastItemPointer, &containerLengthX, &containerLengthY, &containerLengthZ,
             minLength, maxLength, fillPercentage, maxDifferentBoxes);

    //prepareitems(firstItem, lastItem, Widths, Heights, Depths);

    problem.clear();
    problem.setContainerLengthX(containerLengthX);
    problem.setContainerLengthY(containerLengthY);
    problem.setContainerLengthZ(containerLengthZ);
    int itemCount = lastItemPointer - firstItemPointer + 1;

    for (int index = 0; index < itemCount; ++index)
    {
        QColor randomColor(rand() % 256, rand() % 256, rand() % 256);
        problem.addBox(items[index].dx, items[index].dy, items[index].dz, randomColor, QString("Caja %"));
    }
}
