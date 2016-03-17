#include "glcontainerwidget.h"
#include "containerproblem.h"
#include "containersolution.h"
#include <QMouseEvent>

GLContainerWidget::GLContainerWidget(QWidget *parent)
    : QOpenGLWidget(parent), containerProblem(0), containerSolution(0)
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
    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightpos[] = {.5, 1., 1., 0.};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
}

void GLContainerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    if (containerProblem != 0)
    {
        glRotatef(xRot, 1.0, 0.0, 0.0);
        glRotatef(yRot, 0.0, 1.0, 0.0);

        int midx = containerProblem->containerLengthX() / 2;
        int midy = containerProblem->containerLengthY() / 2;
        int midz = containerProblem->containerLengthZ() / 2;
        glTranslatef(-midx, -midy, -midz);

        if (containerProblem != 0 && containerSolution != 0)
        {
            srand(0);
            drawContainer();
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
        update();
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
    distance -= event->delta() / 10.0f;
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
    update();
}

void GLContainerWidget::drawCube(int x1, int y1, int z1, int x2, int y2, int z2)
{
    GLint color[] = { rand(), rand(), rand(), INT_MAX };
    glMaterialiv(GL_FRONT, GL_DIFFUSE, color);
    glBegin(GL_QUADS);
    glNormal3b(0, 0, -1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);

    glNormal3b(0, 0, 1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);

    glNormal3b(-1, 0, 0);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);

    glNormal3b(1, 0, 0);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y1, z1);

    glNormal3b(0, -1, 0);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y1, z1);

    glNormal3b(0, 1, 0);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glEnd();
}

void GLContainerWidget::drawContainer()
{
    int x1 = 0, y1 = 0, z1 = 0;
    int x2 = containerProblem->containerLengthX();
    int y2 = containerProblem->containerLengthY();
    int z2 = containerProblem->containerLengthZ();
    glColor3i(INT_MAX, INT_MAX, INT_MAX);

    glBegin(GL_LINE_LOOP);
    glVertex3i(x1, y1, z1);
    glVertex3i(x1, y2, z1);
    glVertex3i(x2, y2, z1);
    glVertex3i(x2, y1, z1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3i(x1, y1, z2);
    glVertex3i(x1, y2, z2);
    glVertex3i(x2, y2, z2);
    glVertex3i(x2, y1, z2);
    glEnd();

    glBegin(GL_LINES);
    glVertex3i(x1, y1, z1);
    glVertex3i(x1, y1, z2);
    glVertex3i(x1, y2, z1);
    glVertex3i(x1, y2, z2);
    glVertex3i(x2, y2, z1);
    glVertex3i(x2, y2, z2);
    glVertex3i(x2, y1, z1);
    glVertex3i(x2, y1, z2);
    glEnd();
}
