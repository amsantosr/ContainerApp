#ifndef CONTAINERSOLUTION_H
#define CONTAINERSOLUTION_H

#include <QVector>

class ContainerSolution
{
public:
    int boxLengthX(int index) const { return boxLengthXvalues[index]; }
    int boxLengthY(int index) const { return boxLengthYvalues[index]; }
    int boxLengthZ(int index) const { return boxLengthZvalues[index]; }
    int boxCoordinateX(int index) const { return boxCoordinateXvalues[index]; }
    int boxCoordinateY(int index) const { return boxCoordinateYvalues[index]; }
    int boxCoordinateZ(int index) const { return boxCoordinateZvalues[index]; }
    bool isBoxPacked(int index) const { return boxPackedFlagValues[index]; }
    int packedVolume() const { return packedVolumeValue; }

    void setBoxLengthsX(QVector<int> values) { boxLengthXvalues = values; }
    void setBoxLengthsY(QVector<int> values) { boxLengthYvalues = values; }
    void setBoxLengthsZ(QVector<int> values) { boxLengthZvalues = values; }
    void setBoxCoordinatesX(QVector<int> values) { boxCoordinateXvalues = values; }
    void setBoxCoordinatesY(QVector<int> values) { boxCoordinateYvalues = values; }
    void setBoxCoordinatesZ(QVector<int> values) { boxCoordinateZvalues = values; }
    void setBoxPackedFlags(QVector<bool> values) { boxPackedFlagValues = values; }
    void setPackedVolume(int value) { packedVolumeValue = value; }

    QVector<int> &boxLengthsX() { return boxLengthXvalues; }
    QVector<int> &boxLengthsY() { return boxLengthYvalues; }
    QVector<int> &boxLengthsZ() { return boxLengthZvalues; }
    QVector<int> &boxCoordinatesX() { return boxCoordinateXvalues; }
    QVector<int> &boxCoordinatesY() { return boxCoordinateYvalues; }
    QVector<int> &boxCoordinatesZ() { return boxCoordinateZvalues; }
    QVector<bool> &boxPackedFlags() { return boxPackedFlagValues; }

private:
    QVector<int> boxLengthXvalues;
    QVector<int> boxLengthYvalues;
    QVector<int> boxLengthZvalues;
    QVector<int> boxCoordinateXvalues;
    QVector<int> boxCoordinateYvalues;
    QVector<int> boxCoordinateZvalues;
    QVector<bool> boxPackedFlagValues;
    int packedVolumeValue;
};

#endif // CONTAINERSOLUTION_H
