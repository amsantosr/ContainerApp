#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoggenerarinstancia.h"
#include "ui_dialoggenerarinstancia.h"
#include "Pisinger/testcont.h"
#include "glcontainerwidget.h"
#include "containerproblemtablemodel.h"
#include "containersolutiontablemodel.h"
#include "ui_containerproblemform.h"
#include "dialoganadircaja.h"
#include "ui_dialoganadircaja.h"
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTableView>
#include <QList>
#include <QXmlStreamWriter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dialogGenerarInstancia(new DialogGenerarInstancia(this)),
    dialogAnadirCaja(new DialogAnadirCaja(this)),
    containerProblemTableModel(new ContainerProblemTableModel(this)),
    containerSolutionTableModel(new ContainerSolutionTableModel(this))
{
    ui->setupUi(this);
    ui->tableViewCajas->setModel(containerProblemTableModel);
    ui->tableViewSolution->setModel(containerSolutionTableModel);
    containerProblemTableModel->setContainerProblem(&containerProblem);

    // connect the problem to the spinboxes and back
    connect(&containerProblem, SIGNAL(containerLengthX_changed(int)),
            ui->spinBoxContainerDimensionX, SLOT(setValue(int)));
    connect(&containerProblem, SIGNAL(containerLengthY_changed(int)),
            ui->spinBoxContainerDimensionY, SLOT(setValue(int)));
    connect(&containerProblem, SIGNAL(containerLengthZ_changed(int)),
            ui->spinBoxContainerDimensionZ, SLOT(setValue(int)));

    connect(ui->spinBoxContainerDimensionX, SIGNAL(valueChanged(int)),
            &containerProblem, SLOT(setContainerLengthX(int)));
    connect(ui->spinBoxContainerDimensionY, SIGNAL(valueChanged(int)),
            &containerProblem, SLOT(setContainerLengthY(int)));
    connect(ui->spinBoxContainerDimensionZ, SIGNAL(valueChanged(int)),
            &containerProblem, SLOT(setContainerLengthZ(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateTestInstanceTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);

//    QList<int> widths, heights, depths;
//    widths.reserve(containerProblem.boxCount());
//    heights.reserve(containerProblem.boxCount());
//    depths.reserve(containerProblem.boxCount());
//    for (int index = 0; index < containerProblem.boxCount(); ++index)
//    {
//        widths.append(containerProblem.boxLengthX(index));
//        heights.append(containerProblem.boxLengthY(index));
//        depths.append(containerProblem.boxLengthZ(index));
//    }
//    containerProblemTableModel->initialize(widths, heights, depths);
}

void MainWindow::generateInstanceFromDialog()
{
    int minimumDimension = dialogGenerarInstancia->ui->spinBoxMinimumDimension->value();
    int maximumDimension = dialogGenerarInstancia->ui->spinBoxMaximumDimension->value();
    int fillPercentage = dialogGenerarInstancia->ui->spinBoxFillPercentage->value();
    int maximumDifferentBoxes = dialogGenerarInstancia->ui->spinBoxDifferentTypes->value();
    generateTestInstanceTableView(minimumDimension, maximumDimension, fillPercentage, maximumDifferentBoxes);
}

void MainWindow::on_actionGenerarInstanciaDePrueba_triggered()
{
    if (dialogGenerarInstancia->exec() == QDialog::Accepted)
    {
        generateInstanceFromDialog();
    }
}

#ifdef DEBUG_ISSUES
void MainWindow::testGenerateInstance()
{
    dialogGenerarInstancia->ui->spinBoxMinimumDimension->setValue(25);
    dialogGenerarInstancia->ui->spinBoxMaximumDimension->setValue(115);
    dialogGenerarInstancia->ui->spinBoxFillPercentage->setValue(90);
    dialogGenerarInstancia->ui->spinBoxDifferentTypes->setValue(20);
    this->generateInstanceFromDialog();
}
#endif

void MainWindow::on_actionResolverProblema_triggered()
{
    containerProblemSolver.solve(containerProblem, containerSolution);
    containerSolutionTableModel->initialize(
                containerSolution.boxLengthsX(),
                containerSolution.boxLengthsY(),
                containerSolution.boxLengthsZ(),
                containerSolution.boxCoordinatesX(),
                containerSolution.boxCoordinatesY(),
                containerSolution.boxCoordinatesZ(),
                containerSolution.boxPackedFlags());
}

void MainWindow::on_actionVisualizarSolucion_triggered()
{
    ui->openGLWidget->setContainerInfo(containerProblem, containerSolution);
    ui->openGLWidget->update();
}

void MainWindow::on_actionAnadirCaja_triggered()
{
    if (dialogAnadirCaja->exec() == QDialog::Accepted)
    {
        int dimensionX = dialogAnadirCaja->ui->spinBoxDimensionX->value();
        int dimensionY = dialogAnadirCaja->ui->spinBoxDimensionY->value();
        int dimensionZ = dialogAnadirCaja->ui->spinBoxDimensionZ->value();
        int cantidad = dialogAnadirCaja->ui->spinBoxCantidad->value();

        for (int i = 0; i < cantidad; ++i)
        {
            containerProblem.addBox(dimensionX, dimensionY, dimensionZ);
        }
    }
}

void MainWindow::on_actionGuardarDatos_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Guardar archivo"));
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::WriteOnly))
        {
            QXmlStreamWriter stream(&file);
            stream.setAutoFormatting(true);
            stream.writeStartDocument();
            stream.writeStartElement("ContainerProblem");

            stream.writeStartElement("Container");
            QString dimensionX = QString::number(ui->spinBoxContainerDimensionX->value());
            QString dimensionY = QString::number(ui->spinBoxContainerDimensionY->value());
            QString dimensionZ = QString::number(ui->spinBoxContainerDimensionZ->value());
            stream.writeAttribute("DimensionX", dimensionX);
            stream.writeAttribute("DimensionY", dimensionY);
            stream.writeAttribute("DimensionZ", dimensionZ);
            stream.writeEndElement();

            stream.writeStartElement("Boxes");
            for (int index = 0; index < containerProblem.boxCount(); ++index)
            {
                stream.writeEmptyElement("Box");
                int lengthX = containerProblem.boxLengthX(index);
                int lengthY = containerProblem.boxLengthY(index);
                int lengthZ = containerProblem.boxLengthZ(index);
                stream.writeAttribute("DimensionX", QString::number(lengthX));
                stream.writeAttribute("DimensionY", QString::number(lengthY));
                stream.writeAttribute("DimensionZ", QString::number(lengthZ));
            }
            stream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionAbrirProblema_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Abrir archivo"));
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::ReadOnly))
        {
            containerProblem.clear();
            QXmlStreamReader stream(&file);
            while (!stream.atEnd())
            {
                if (stream.readNextStartElement())
                {
                    if (stream.name() == "Container")
                    {
                        foreach (QXmlStreamAttribute attribute, stream.attributes())
                        {
                            if (attribute.name() == "DimensionX")
                            {
                                int dimensionX = attribute.value().toInt();
                                containerProblem.setContainerLengthX(dimensionX);
                            }
                            else if (attribute.name() == "DimensionY")
                            {
                                int dimensionY = attribute.value().toInt();
                                containerProblem.setContainerLengthY(dimensionY);
                            }
                            else if (attribute.name() == "DimensionZ")
                            {
                                int dimensionZ = attribute.value().toInt();
                                containerProblem.setContainerLengthZ(dimensionZ);
                            }
                        }
                    }
                    else if (stream.name() == "Box")
                    {
                        int boxDimensionX, boxDimensionY, boxDimensionZ;
                        foreach (QXmlStreamAttribute attribute, stream.attributes())
                        {
                            if (attribute.name() == "DimensionX")
                            {
                                boxDimensionX = attribute.value().toInt();
                            }
                            else if (attribute.name() == "DimensionY")
                            {
                                boxDimensionY = attribute.value().toInt();
                            }
                            else if (attribute.name() == "DimensionZ")
                            {
                                boxDimensionZ = attribute.value().toInt();
                            }
                        }
                        containerProblem.addBox(boxDimensionX, boxDimensionY, boxDimensionZ);
                    }
                }
            }
            if (stream.hasError())
            {
                // procesar errores aqui
            }
        }
    }
}

void MainWindow::on_actionGuardarSolucion_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Guardar solución"));
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::WriteOnly))
        {
            QXmlStreamWriter stream(&file);
            stream.setAutoFormatting(true);
            stream.writeStartDocument();
            stream.writeStartElement("ContainerSolution");

            stream.writeStartElement("Boxes");
            for (int index = 0; index < containerSolution.boxCount(); ++index)
            {
                if (containerSolution.isBoxPacked(index))
                {
                    stream.writeStartElement("Box");

                    stream.writeEmptyElement("Position");
                    int posX = containerSolution.boxCoordinateX(index);
                    int posY = containerSolution.boxCoordinateY(index);
                    int posZ = containerSolution.boxCoordinateZ(index);
                    stream.writeAttribute("X", QString::number(posX));
                    stream.writeAttribute("Y", QString::number(posY));
                    stream.writeAttribute("Z", QString::number(posZ));

                    stream.writeEmptyElement("Dimensions");
                    int x = containerSolution.boxLengthX(index);
                    int y = containerSolution.boxLengthZ(index);
                    int z = containerSolution.boxLengthZ(index);
                    stream.writeAttribute("X", QString::number(x));
                    stream.writeAttribute("Y", QString::number(y));
                    stream.writeAttribute("Z", QString::number(z));

                    stream.writeEndElement();
                }
            }
            stream.writeEndElement();
            stream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionNuevoProblema_triggered()
{
    containerProblem.clear();
}
