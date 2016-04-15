#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoggenerateproblem.h"
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
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dialogGenerarInstancia(new DialogGenerateProblem(this)),
    dialogAnadirCaja(new DialogAnadirCaja(this)),
    containerProblemTableModel(new ContainerProblemTableModel(this)),
    containerSolutionTableModel(new ContainerSolutionTableModel(this)),
    algorithmThread(this),
    algorithmExecutionDialog(this)
{
    ui->setupUi(this);
    ui->splitterHorizontal->setStretchFactor(0, 0);
    ui->splitterHorizontal->setStretchFactor(1, 1);
    ui->tableViewCajas->setModel(containerProblemTableModel);
    ui->tableViewSolution->setModel(containerSolutionTableModel);
    ui->openGLWidget->setContainerSolution(containerSolution);
    containerProblemTableModel->setContainerProblem(&containerProblem);
    containerSolutionTableModel->setContainerSolution(&containerSolution);

    // connect the problem to the spinboxes and back
    connect(&containerProblem, &ContainerProblem::containerLengthX_changed,
            ui->spinBoxContainerDimensionX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::setValue));
    connect(&containerProblem, &ContainerProblem::containerLengthY_changed,
            ui->spinBoxContainerDimensionY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::setValue));
    connect(&containerProblem, &ContainerProblem::containerLengthZ_changed,
            ui->spinBoxContainerDimensionZ, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::setValue));

    connect(ui->spinBoxContainerDimensionX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            &containerProblem, &ContainerProblem::setContainerLengthX);
    connect(ui->spinBoxContainerDimensionY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            &containerProblem, &ContainerProblem::setContainerLengthY);
    connect(ui->spinBoxContainerDimensionZ, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            &containerProblem, &ContainerProblem::setContainerLengthZ);

    // connect the slider to the GLContainerWidget
    connect(ui->sliderBoxCount, &QSlider::valueChanged,
            ui->openGLWidget, &GLContainerWidget::setDisplayedBoxesLimit);

    connect(&containerSolution, &ContainerSolution::afterDataChange, this, [&]()
    {
        // update the maximum value in the slider and set the value to the maximum
        ui->sliderBoxCount->setMaximum(containerSolution.packedBoxesCount());
        ui->sliderBoxCount->setValue(containerSolution.packedBoxesCount());
    });

    // connect the signals from the AlgorithmThread thread
    connect(&algorithmThread, &AlgorithmThread::started,
            &algorithmExecutionDialog, &AlgorithmExecutionDialog::show);
    connect(&algorithmThread, &AlgorithmThread::finished,
            &algorithmExecutionDialog, &AlgorithmExecutionDialog::hide);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateTestInstanceTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);
}

void MainWindow::generateInstanceFromDialog()
{
    int minimumDimension = dialogGenerarInstancia->ui->spinBoxMinimumDimension->value();
    int maximumDimension = dialogGenerarInstancia->ui->spinBoxMaximumDimension->value();
    int fillPercentage = dialogGenerarInstancia->ui->spinBoxFillPercentage->value();
    int maximumDifferentBoxes = dialogGenerarInstancia->ui->spinBoxDifferentTypes->value();
    generateTestInstanceTableView(minimumDimension, maximumDimension, fillPercentage, maximumDifferentBoxes);
}

void MainWindow::solveProblem()
{
    containerProblemSolver.solve(containerProblem, containerSolution);
}

void MainWindow::on_actionGenerateProblem_triggered()
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

void MainWindow::on_actionSolveProblem_triggered()
{
    algorithmThread.start();
}

void MainWindow::on_actionAddBox_triggered()
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

void MainWindow::on_actionSaveProblem_triggered()
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

void MainWindow::on_actionOpenProblem_triggered()
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

void MainWindow::on_actionSaveSolution_triggered()
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

void MainWindow::on_actionNewProblem_triggered()
{
    containerProblem.clear();
    containerSolution.clear();
    ui->openGLWidget->resetView();
}

void MainWindow::on_actionDeleteBox_triggered()
{
    containerProblem.removeIndexes(ui->tableViewCajas->selectedIndexes());
}
