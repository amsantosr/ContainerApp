#ifndef GLCONTAINERWIDGET_H
#define GLCONTAINERWIDGET_H

#include <QObject>
#include <QGLWidget>
#include <QOpenGLWidget>
#include <GL/glu.h>

class ContainerProblem;
class ContainerSolution;

class GLContainerWidget : public QOpenGLWidget
{
public:
    GLContainerWidget(QWidget *parent = 0);
    ~GLContainerWidget();
    void setContainerProblem(ContainerProblem *problem);
    void setContainerSolution(ContainerSolution *solution);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resetView();
    void setDisplayedBoxesLimit(int value);

private:
    void drawCube(int x1, int y1, int z1, int x2, int y2, int z2);
    void drawContainer();

private:
    QPoint mouseLastPosition;
    int rotationX;
    int rotationY;
    int rotationZ;
    float distance;
    int displayedBoxesLimit;

    ContainerProblem *containerProblem;
    ContainerSolution *containerSolution;
};

#endif // GLCONTAINERWIDGET_H
