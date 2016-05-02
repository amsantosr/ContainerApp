#include "glcontainerwidget.h"
#include "containerproblem.h"
#include "containersolution.h"
#include <QMouseEvent>

GLContainerWidget::GLContainerWidget(QWidget *parent)
    : QOpenGLWidget(parent), containerProblem(0), containerSolution(0)
{
    resetView();
    setCursor(Qt::OpenHandCursor);
}

GLContainerWidget::~GLContainerWidget()
{
}

void GLContainerWidget::setContainerProblem(ContainerProblem *problem)
{
    if (containerProblem != problem)
    {
        std::swap(containerProblem, problem);
        if (containerProblem != 0)
        {
            auto functionUpdate = static_cast<void (GLContainerWidget::*)()>(&GLContainerWidget::update);
            connect(containerProblem, &ContainerProblem::containerLengthX_changed, this, functionUpdate);
            connect(containerProblem, &ContainerProblem::containerLengthY_changed, this, functionUpdate);
            connect(containerProblem, &ContainerProblem::containerLengthZ_changed, this, functionUpdate);
        }
        if (problem != 0)
            disconnect(problem, 0, this, 0);
    }
}

void GLContainerWidget::setContainerSolution(ContainerSolution *solution)
{
    if (containerSolution != solution)
    {
        std::swap(containerSolution, solution);
        if (containerSolution != 0)
        {
            auto glContainerWidgetUpdate = static_cast<void (GLContainerWidget::*)()>(&GLContainerWidget::update);
            connect(containerSolution, &ContainerSolution::afterDataChange,
                    this, glContainerWidgetUpdate);
        }
        if (solution != 0)
            disconnect(solution, 0, this, 0);
    }
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
        glRotatef(rotationX, 1.0, 0.0, 0.0);
        glRotatef(rotationY, 0.0, 1.0, 0.0);

        int lengthX = containerProblem->containerLengthX();
        int lengthY = containerProblem->containerLengthY();
        int lengthZ = containerProblem->containerLengthZ();

        if (lengthX > 0 && lengthY > 0 && lengthZ > 0)
        {
            int midX = lengthX / 2;
            int midY = lengthY / 2;
            int midZ = lengthZ / 2;
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
        mouseLastPosition = event->pos();
    }
}

void GLContainerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int dx = event->x() - mouseLastPosition.x();
        int dy = event->y() - mouseLastPosition.y();
        (rotationX += dy + 360) %= 360;
        (rotationY += dx + 360) %= 360;
        mouseLastPosition = event->pos();
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
    rotationX = rotationZ = 0;
    rotationY = 270;
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
    int x2 = containerProblem->containerLengthX();
    int y2 = containerProblem->containerLengthY();
    int z2 = containerProblem->containerLengthZ();

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
