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

    for (int i = 0; i < itemCount; ++i)
    {
        int index;
        for (index = 0; index < problem.boxCount(); ++index)
        {
            if (problem.boxLengthX(index) == items[i].dx &&
                problem.boxLengthY(index) == items[i].dy &&
                problem.boxLengthZ(index) == items[i].dz)
            {
                // increment the counter
                problem.setBoxQuantity(index, problem.boxQuantity(index) + 1);
                break;
            }
        }
        if (index == problem.boxCount())
        {
            QColor randomColor(rand() % 256, rand() % 256, rand() % 256);
            problem.addBox(items[i].dx, items[i].dy, items[i].dz, 1, randomColor, QString("Caja tipo %1").arg(index + 1));
        }
    }
}
