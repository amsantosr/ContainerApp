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

void GLContainerWidget::setContainerSolution(ContainerSolution *solution)
{
    auto functionUpdate = static_cast<void (GLContainerWidget::*)()>(&GLContainerWidget::update);
    if (containerSolution != solution)
    {
        std::swap(containerSolution, solution);
        if (containerSolution != 0)
        {
            connect(containerSolution, &ContainerSolution::afterDataChange, this, functionUpdate);
        }
        if (solution != 0)
            disconnect(solution, 0, this, 0);
    }
    ContainerProblem *problem = containerSolution->getContainerProblem();
    if (containerProblem != problem)
    {
        std::swap(containerProblem, problem);
        containerProblem = containerSolution->getContainerProblem();
        if (containerProblem != 0)
        {
            connect(containerProblem, &ContainerProblem::containerLengthXChanged, this, functionUpdate);
            connect(containerProblem, &ContainerProblem::containerLengthYChanged, this, functionUpdate);
            connect(containerProblem, &ContainerProblem::containerLengthZChanged, this, functionUpdate);
        }
        if (problem != 0)
            disconnect(problem, 0, this, 0);
    }
}

void GLContainerWidget::initializeGL()
{
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    // Enable light and set up 2 light sources (GL_LIGHT0 and GL_LIGHT1)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    // We're setting up two light sources. One of them is located
    // on the left side of the model (x = -1.5f) and emits white light. The
    // second light source is located on the right side of the model (x = 1.5f)
    // emitting red light.

    // GL_LIGHT0: the white light emitting light source
    // Create light components for GL_LIGHT0
    float ambientLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float diffuseLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float specularLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float position0[] = { -1500.0f, 1.0f, -4.0f, 1.0f };
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);

    // GL_LIGHT1: the red light emitting light source
    // Create light components for GL_LIGHT1
    float ambientLight1[] = { 1.0f, 0.5f, 0.5f, 1.0f };
    float diffuseLight1[] = { 1.0f, 0.5f, 0.5f, 1.0f };
    float specularLight1[] = { 1.0f, 0.5f, 0.5f, 1.0f };
    float position1[] = { 1500, 1.0f, -4.0f, 1.0f };
    // Assign created components to GL_LIGHT1
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);

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
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);

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
            int groupIndex = containerSolution->packedBoxGroupIndex(count);
            int x1 = containerSolution->packedBoxCoordinateX(count);
            int y1 = containerSolution->packedBoxCoordinateY(count);
            int z1 = containerSolution->packedBoxCoordinateZ(count);
            int x2 = x1 + containerSolution->packedBoxLengthX(count);
            int y2 = y1 + containerSolution->packedBoxLengthY(count);
            int z2 = z1 + containerSolution->packedBoxLengthZ(count);
            QColor color = containerProblem->groupColor(groupIndex);
            drawBox(x1, y1, z1, x2, y2, z2, color);
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
    rotationY = 0;
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

void GLContainerWidget::drawBox(int x1, int y1, int z1, int x2, int y2, int z2, QColor color)
{
    GLfloat faceColor[] = { float(color.redF()), float(color.greenF()), float(color.blueF()), 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColor);
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

    GLint lineColor[] = { 0, 0, 0, INT_MAX };
    glMaterialiv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, lineColor);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glEnd();
}

void GLContainerWidget::drawContainer()
{
    int x1 = 0, y1 = 0, z1 = 0;
    int x2 = containerProblem->containerLengthX();
    int y2 = containerProblem->containerLengthY();
    int z2 = containerProblem->containerLengthZ();

    GLint color[] = { 0, INT_MAX, 0, INT_MAX };
    glMaterialiv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
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
