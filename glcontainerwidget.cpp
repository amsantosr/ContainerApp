#include "glcontainerwidget.h"
#include "containerproblem.h"
#include "containersolution.h"
#include <QMouseEvent>

GLContainerWidget::GLContainerWidget(QWidget *parent)
    : QOpenGLWidget(parent), containerSolution(0)
{
    resetView();
    setCursor(Qt::OpenHandCursor);
}

GLContainerWidget::~GLContainerWidget()
{
}

void GLContainerWidget::setContainerSolution(ContainerSolution &solution)
{
    containerSolution = &solution;
    connect(containerSolution, &ContainerSolution::afterDataChange,
            this, static_cast<void (GLContainerWidget::*)()>(&GLContainerWidget::update));
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLContainerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, distance);
    if (containerSolution != 0)
    {
        glRotatef(xRot, 1.0, 0.0, 0.0);
        glRotatef(yRot, 0.0, 1.0, 0.0);

        int lengthX = containerSolution->containerLengthX();
        int lengthY = containerSolution->containerLengthY();
        int lengthZ = containerSolution->containerLengthZ();

        if (lengthX > 0 && lengthY > 0 && lengthZ > 0)
        {
            int midX = containerSolution->containerLengthX() / 2;
            int midY = containerSolution->containerLengthY() / 2;
            int midZ = containerSolution->containerLengthZ() / 2;
            glTranslatef(-midX, -midY, -midZ);
            drawContainer();
        }

        srand(0);
        int displayedBoxes = std::min(displayedBoxesLimit, containerSolution->packedBoxesCount());
        for (int count = 0; count < displayedBoxes; ++count)
        {
            int boxIndex = containerSolution->boxOrderIndex(count);
            int x1 = containerSolution->boxCoordinateX(boxIndex);
            int y1 = containerSolution->boxCoordinateY(boxIndex);
            int z1 = containerSolution->boxCoordinateZ(boxIndex);
            int x2 = x1 + containerSolution->boxLengthX(boxIndex);
            int y2 = y1 + containerSolution->boxLengthY(boxIndex);
            int z2 = z1 + containerSolution->boxLengthZ(boxIndex);
            drawCube(x1, y1, z1, x2, y2, z2);
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
    glFrustum(-width/side, width/side, -height/side, height/side, 5.0, 1500.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLContainerWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        setCursor(Qt::ClosedHandCursor);
        lastPos = event->pos();
    }
}

void GLContainerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
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
    if (event->button() == Qt::LeftButton)
    {
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

void GLContainerWidget::resetView()
{
    distance = -1000.0f;
    xRot = yRot = zRot = 0;
    update();
}

void GLContainerWidget::setDisplayedBoxesLimit(int value)
{
    if (displayedBoxesLimit != value)
    {
        displayedBoxesLimit = value;
        update();
    }
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
    int x2 = containerSolution->containerLengthX();
    int y2 = containerSolution->containerLengthY();
    int z2 = containerSolution->containerLengthZ();

    glColor3i(0, INT_MAX, 0);
    glDisable(GL_LIGHTING);
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
    glEnable(GL_LIGHTING);
}
