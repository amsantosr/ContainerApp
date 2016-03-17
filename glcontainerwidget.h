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
    void setContainerInfo(const ContainerProblem &problem, const ContainerSolution &solution);

public:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void drawCube(int x1, int y1, int z1, int x2, int y2, int z2);
    void drawContainer();

private:
    GLUquadricObj *quadric;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    float distance;

    const ContainerProblem *containerProblem;
    const ContainerSolution *containerSolution;
};

#endif // GLCONTAINERWIDGET_H
