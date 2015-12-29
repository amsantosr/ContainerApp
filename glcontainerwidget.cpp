#include "glcontainerwidget.h"
#include "containerproblem.h"
#include "containersolution.h"
#include <QMouseEvent>

GLContainerWidget::GLContainerWidget(QWidget *parent)
    : QGLWidget(parent), containerProblem(0), containerSolution(0)
{
    distance = -1000.0f;
    xRot = yRot = zRot = 0;

    setCursor(Qt::OpenHandCursor);
    quadric = gluNewQuadric();
}

GLContainerWidget::~GLContainerWidget()
{
    gluDeleteQuadric(quadric);
}

void GLContainerWidget::setContainerInfo(const ContainerProblem &problem, const ContainerSolution &solution)
{
    containerProblem = &problem;
    containerSolution = &solution;
}

void GLContainerWidget::initializeGL()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
}

void GLContainerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);

    if (containerProblem != 0 && containerSolution != 0)
    {
        srand(0);
        //drawCube(0, 0, 0, containerProblem->containerLengthX(), containerProblem->containerLengthY(), containerProblem->containerLengthZ());
        for (int index = 0; index < containerProblem->boxCount(); ++index)
        {
            if (containerSolution->isBoxPacked(index))
            {
                int x1 = containerSolution->boxCoordinateX(index);
                int y1 = containerSolution->boxCoordinateY(index);
                int z1 = containerSolution->boxCoordinateZ(index);
                int x2 = x1 + containerSolution->boxLengthX(index);
                int y2 = y1 + containerSolution->boxLengthY(index);
                int z2 = z1 + containerSolution->boxLengthZ(index);
                drawCube(x1, y1, z1, x2, y2, z2);
            }
        }
    }

    glPopMatrix();
}

void GLContainerWidget::resizeGL(int width, int height)
{
    double side = qMin(width, height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-width/side, width/side, -height/side, height/side, 5.0, 1200.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLContainerWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        setCursor(Qt::ClosedHandCursor);
        lastPos = event->pos();
    }
}

void GLContainerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();
        (xRot += dy + 360) %= 360;
        (yRot += dx + 360) %= 360;
        lastPos = event->pos();
        updateGL();
    }
}

void GLContainerWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setCursor(Qt::OpenHandCursor);
    }
}

void GLContainerWidget::wheelEvent(QWheelEvent *event)
{
    distance -= event->delta() / 50.0f;
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
    updateGL();
}

void GLContainerWidget::drawCube(int x1, int y1, int z1, int x2, int y2, int z2)
{
//    glDisable(GL_LIGHTING);

//    glColor3ub(rand() % 256, rand() % 256, rand() % 256);
//    glBegin(GL_LINES);
//    glVertex3f(x1, y1, z1);
//    glVertex3f(x1, y2, z1);
//    glVertex3f(x1, y2, z1);
//    glVertex3f(x2, y2, z1);
//    glVertex3f(x2, y2, z1);
//    glVertex3f(x2, y1, z1);
//    glVertex3f(x2, y1, z1);
//    glVertex3f(x1, y1, z1);

//    glVertex3f(x2, y2, z2);
//    glVertex3f(x2, y1, z2);
//    glVertex3f(x2, y1, z2);
//    glVertex3f(x1, y1, z2);
//    glVertex3f(x1, y1, z2);
//    glVertex3f(x1, y2, z2);
//    glVertex3f(x1, y2, z2);
//    glVertex3f(x2, y2, z2);

//    glVertex3f(x1, y1, z1);
//    glVertex3f(x1, y1, z2);
//    glVertex3f(x1, y2, z1);
//    glVertex3f(x1, y2, z2);
//    glVertex3f(x2, y2, z1);
//    glVertex3f(x2, y2, z2);
//    glVertex3f(x2, y1, z1);
//    glVertex3f(x2, y1, z2);
//    glEnd();
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor4ub(rand() % 256, rand() % 256, rand() % 256, 0);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);

    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);

    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);

    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y1, z1);

    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glEnd();
}
