#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoggenerateproblem.h"
#include "ui_dialoggenerateproblem.h"
#include "Pisinger/testcont.h"
#include "glcontainerwidget.h"
#include "containerproblemtablemodel.h"
#include "containersolutiontablemodel.h"
#include "boxesorderingtablemodel.h"
#include "ui_containerproblemform.h"
#include "ui_dialogabout.h"
#include "ui_dialogaddbox.h"
#include "ui_dialogalgorithmexecution.h"
#include "workercontainerproblemsolver.h"
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
    dialogGenerateProblem(new DialogGenerateProblem(this)),
    dialogAddBox(this),
    containerProblemTableModel(new ContainerProblemTableModel(this)),
    containerSolutionTableModel(new ContainerSolutionTableModel(this)),
    boxesOrderingTableModel(new BoxesOrderingTableModel(this)),
    dialogAlgorithmExecution(this),
    uiAlgorithmExecution(new Ui::DialogAlgorithmExecution)
{
    ui->setupUi(this);
    Ui::DialogAbout uiDialogAbout;
    uiDialogAbout.setupUi(&dialogAbout);
    uiDialogAddBox.setupUi(&dialogAddBox);
    uiAlgorithmExecution->setupUi(&dialogAlgorithmExecution);
    ui->splitterHorizontal->setStretchFactor(0, 0);
    ui->splitterHorizontal->setStretchFactor(1, 1);
    ui->tableViewBoxes->setModel(containerProblemTableModel);
    ui->tableViewSolution->setModel(containerSolutionTableModel);
    ui->tableViewOrdering->setModel(boxesOrderingTableModel);
    ui->openGLWidget->setContainerSolution(containerSolution);
    containerProblemTableModel->setContainerProblem(&containerProblem);
    containerSolutionTableModel->setContainerSolution(&containerSolution);
    boxesOrderingTableModel->setContainerSolution(&containerSolution);

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
    connect(&containerProblem, &ContainerProblem::containerLengthX_changed,
            &containerSolution, &ContainerSolution::setContainerLengthX);
    connect(&containerProblem, &ContainerProblem::containerLengthY_changed,
            &containerSolution, &ContainerSolution::setContainerLengthY);
    connect(&containerProblem, &ContainerProblem::containerLengthZ_changed,
            &containerSolution, &ContainerSolution::setContainerLengthZ);

    // connect the slider to the GLContainerWidget
    connect(ui->sliderDisplayedBoxes, &QSlider::valueChanged, [&](int value)
    {
        ui->openGLWidget->setDisplayedBoxesLimit(value);
        if (value > 0)
        {
            int lastBoxIndex = containerSolution.boxOrderIndex(value - 1);
            ui->labelLastBox->setText(tr("Caja %1").arg(lastBoxIndex));
        }
        else
        {
            ui->labelLastBox->clear();
        }
    });

    connect(&containerSolution, &ContainerSolution::afterDataChange, this, [&]()
    {
        // update the maximum value in the slider and set the value to the maximum
        ui->sliderDisplayedBoxes->setMaximum(containerSolution.packedBoxesCount());
        ui->sliderDisplayedBoxes->setValue(containerSolution.packedBoxesCount());
    });

    WorkerContainerProblemSolver *worker = new WorkerContainerProblemSolver;
    worker->moveToThread(&threadWorker);
    connect(&threadWorker, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::solveProblemAsync, worker, &WorkerContainerProblemSolver::doWork);
    connect(worker, &WorkerContainerProblemSolver::workStarts,
            &dialogAlgorithmExecution, &DialogAlgorithmExecution::show);
    connect(worker, &WorkerContainerProblemSolver::workEnds,
            &dialogAlgorithmExecution, &DialogAlgorithmExecution::hide);
    connect(uiAlgorithmExecution->pushButtonCancel, &QPushButton::clicked, [&]()
    {
        threadWorker.terminate();
        threadWorker.wait();
        dialogAlgorithmExecution.hide();
        threadWorker.start();
    });
    threadWorker.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    threadWorker.quit();
    threadWorker.wait();
}

void MainWindow::generateProblemTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);
}

void MainWindow::generateProblemFromDialog()
{
    int minimumDimension = dialogGenerateProblem->ui->spinBoxMinimumDimension->value();
    int maximumDimension = dialogGenerateProblem->ui->spinBoxMaximumDimension->value();
    int fillPercentage = dialogGenerateProblem->ui->spinBoxFillPercentage->value();
    int maximumDifferentBoxes = dialogGenerateProblem->ui->spinBoxDifferentTypes->value();
    generateProblemTableView(minimumDimension, maximumDimension, fillPercentage, maximumDifferentBoxes);
}

void MainWindow::on_actionGenerateProblem_triggered()
{
    if (dialogGenerateProblem->exec() == QDialog::Accepted)
    {
        generateProblemFromDialog();
    }
}

#ifdef DEBUG_ISSUES
void MainWindow::testGenerateProblem()
{
    dialogGenerateProblem->ui->spinBoxMinimumDimension->setValue(25);
    dialogGenerateProblem->ui->spinBoxMaximumDimension->setValue(115);
    dialogGenerateProblem->ui->spinBoxFillPercentage->setValue(90);
    dialogGenerateProblem->ui->spinBoxDifferentTypes->setValue(20);
    this->generateProblemFromDialog();
}
#endif

void MainWindow::on_actionSolveProblem_triggered()
{
    if (ui->spinBoxContainerDimensionX->value() == 0 ||
            ui->spinBoxContainerDimensionY->value() == 0 ||
            ui->spinBoxContainerDimensionZ->value() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Todas las dimensiones del contenedor deben ser mayor que cero."));
        return;
    }
    if (containerProblem.boxCount() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("No se han ingresado cajas para procesar."));
        return;
    }
    emit solveProblemAsync(&containerProblem, &containerSolution);
}

void MainWindow::on_actionAddBox_triggered()
{
    if (dialogAddBox.exec() == QDialog::Accepted)
    {
        int dimensionX = uiDialogAddBox.spinBoxDimensionX->value();
        int dimensionY = uiDialogAddBox.spinBoxDimensionY->value();
        int dimensionZ = uiDialogAddBox.spinBoxDimensionZ->value();
        int cantidad = uiDialogAddBox.spinBoxCantidad->value();

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
                // process xml errors here
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
    containerProblem.removeBoxes(ui->tableViewBoxes->selectedIndexes());
}

void MainWindow::on_actionAbout_triggered()
{
    dialogAbout.show();
}
