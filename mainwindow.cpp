#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialoggenerateproblem.h"
#include "ui_dialogabout.h"
#include "Pisinger/testcont.h"
#include "glcontainerwidget.h"
#include "containerproblemtablemodel.h"
#include "containersolutiontablemodel.h"
#include "boxesorderingtablemodel.h"
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
    dialogGenerateProblem(this),
    dialogAddBox(this),
    dialogAlgorithmExecution(this),
    dialogMeasurementSystem(this),
    containerProblemTableModel(new ContainerProblemTableModel(this)),
    containerSolutionTableModel(new ContainerSolutionTableModel(this)),
    boxesOrderingTableModel(new BoxesOrderingTableModel(this))
{
    ui->setupUi(this);
    Ui::DialogAbout uiDialogAbout;
    uiDialogAbout.setupUi(&dialogAbout);
    uiDialogAddBox.setupUi(&dialogAddBox);
    uiDialogAlgorithmExecution.setupUi(&dialogAlgorithmExecution);
    uiDialogGenerateProblem.setupUi(&dialogGenerateProblem);
    uiDialogMeasurementSystem.setupUi(&dialogMeasurementSystem);

    ui->splitterHorizontal->setStretchFactor(0, 0);
    ui->splitterHorizontal->setStretchFactor(1, 1);
    ui->tableViewBoxes->setModel(containerProblemTableModel);
    ui->tableViewSolution->setModel(containerSolutionTableModel);
    ui->tableViewOrdering->setModel(boxesOrderingTableModel);
    ui->openGLWidget->setContainerProblem(&containerProblem);
    ui->openGLWidget->setContainerSolution(&containerSolution);
    containerProblemTableModel->setContainerProblem(&containerProblem);
    containerSolutionTableModel->setContainerSolution(&containerSolution);
    boxesOrderingTableModel->setContainerSolution(&containerSolution);
    connect(uiDialogMeasurementSystem.buttonBox, &QDialogButtonBox::accepted, [&]
    {
        if (uiDialogMeasurementSystem.radioButtonCentimeters->isChecked() ||
                uiDialogMeasurementSystem.radioButtonInches->isChecked())
            dialogMeasurementSystem.accept();
    });

    auto spinBoxSetValue = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::setValue);
    connect(&containerProblem, &ContainerProblem::containerLengthX_changed,
            ui->spinBoxContainerDimensionX, spinBoxSetValue);
    connect(&containerProblem, &ContainerProblem::containerLengthY_changed,
            ui->spinBoxContainerDimensionY, spinBoxSetValue);
    connect(&containerProblem, &ContainerProblem::containerLengthZ_changed,
            ui->spinBoxContainerDimensionZ, spinBoxSetValue);
    connect(&containerProblem, &ContainerProblem::textUnit_changed,
            this, &MainWindow::setTextUnit);
    containerSolution.setContainerProblem(&containerProblem);

    connect(ui->spinBoxContainerDimensionX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            &containerProblem, &ContainerProblem::setContainerLengthX);
    connect(ui->spinBoxContainerDimensionY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            &containerProblem, &ContainerProblem::setContainerLengthY);
    connect(ui->spinBoxContainerDimensionZ, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            &containerProblem, &ContainerProblem::setContainerLengthZ);

    listLabelsUnits << uiDialogAddBox.labelUnit1
                    << uiDialogAddBox.labelUnit2
                    << uiDialogAddBox.labelUnit3
                    << uiDialogGenerateProblem.labelUnit1
                    << uiDialogGenerateProblem.labelUnit2
                    << uiDialogGenerateProblem.labelUnit3
                    << uiDialogGenerateProblem.labelUnit4
                    << ui->labelContainerUnit1
                    << ui->labelContainerUnit2
                    << ui->labelContainerUnit3;

    setTextUnit(containerProblem.textUnit());

    // connect the slider to the GLContainerWidget
    connect(ui->sliderDisplayedBoxes, &QSlider::valueChanged, this, &MainWindow::setMaximumDisplayedBoxes);

    connect(&containerSolution, &ContainerSolution::afterDataChange, this, [&]()
    {
        // update the maximum value in the slider and set the value to the maximum
        ui->sliderDisplayedBoxes->setMaximum(containerSolution.packedBoxesCount());
        ui->sliderDisplayedBoxes->setValue(containerSolution.packedBoxesCount());
    });

    containerProblemSolverThread.setParameters(&containerProblem, &containerSolution);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::started,
            &dialogAlgorithmExecution, &QDialog::exec);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::finished,
            &dialogAlgorithmExecution, &QDialog::hide);
    connect(uiDialogAlgorithmExecution.pushButtonCancel, &QPushButton::clicked,
            this, [&]
    {
        containerProblemSolverThread.terminate();
        containerProblemSolverThread.wait();
        dialogAlgorithmExecution.hide();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateProblemTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);
}

void MainWindow::generateProblemFromDialog()
{
    int minimumDimension = uiDialogGenerateProblem.spinBoxMinimumDimension->value();
    int maximumDimension = uiDialogGenerateProblem.spinBoxMaximumDimension->value();
    int fillPercentage = uiDialogGenerateProblem.spinBoxFillPercentage->value();
    int maximumDifferentBoxes = uiDialogGenerateProblem.spinBoxDifferentTypes->value();
    generateProblemTableView(minimumDimension, maximumDimension, fillPercentage, maximumDifferentBoxes);
}

void MainWindow::setMaximumDisplayedBoxes(int value)
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
}

void MainWindow::setTextUnit(QString text)
{
    foreach (QLabel *label, listLabelsUnits)
    {
        label->setText(text);
    }
}

void MainWindow::on_actionGenerateProblem_triggered()
{
    if (dialogGenerateProblem.exec() == QDialog::Accepted)
    {
        generateProblemFromDialog();
    }
}

#ifdef DEBUG_ISSUES
void MainWindow::testGenerateProblem()
{
    uiDialogGenerateProblem.spinBoxMinimumDimension->setValue(25);
    uiDialogGenerateProblem.spinBoxMaximumDimension->setValue(115);
    uiDialogGenerateProblem.spinBoxFillPercentage->setValue(90);
    uiDialogGenerateProblem.spinBoxDifferentTypes->setValue(20);
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
    containerProblemSolverThread.start();
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
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Guardar archivo"),
                                                    QString(),
                                                    "Problema de planificación de carga (*.xml)");
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::WriteOnly))
            containerXmlParser.writeProblem(containerProblem, &file);
        else
            QMessageBox::critical(this, tr("Error"),
                                  tr("No se puede abrir el archivo %1 para escritura").arg(filename));
    }
}

void MainWindow::on_actionOpenProblem_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Abrir archivo"));
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::ReadOnly))
            containerXmlParser.readProblem(&file, containerProblem);
        else
            QMessageBox::critical(this, tr("Error"),
                                  tr("No se pudo abrir el archivo %1 para lectura").arg(filename));
    }
}

void MainWindow::on_actionSaveSolution_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Guardar solución"));
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::WriteOnly))
            containerXmlParser.writeSolution(containerSolution, &file);
        else
            QMessageBox::critical(this, tr("Error"),
                                  tr("No se puede abrir el archivo %1 para escritura").arg(filename));
    }
}

void MainWindow::on_actionNewProblem_triggered()
{
    containerProblem.clear();
    containerSolution.clear();
}

void MainWindow::on_actionDeleteBox_triggered()
{
    containerProblem.removeBoxes(ui->tableViewBoxes->selectedIndexes());
}

void MainWindow::on_actionAbout_triggered()
{
    dialogAbout.show();
}

void MainWindow::on_actionSetMeasurementSystem_triggered()
{
    if (dialogMeasurementSystem.exec() == QDialog::Accepted)
    {
        if (uiDialogMeasurementSystem.radioButtonCentimeters->isChecked())
        {
            setTextUnit("cm.");
        }
        else if (uiDialogMeasurementSystem.radioButtonInches->isChecked())
        {
            setTextUnit("in.");
        }
    }
}
