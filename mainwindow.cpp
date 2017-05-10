#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialoggenerateproblem.h"
#include "ui_dialogabout.h"
#include "Pisinger/testcont.h"
#include "glcontainerwidget.h"
#include "boxesgroupstablemodel.h"
#include "solutionboxestablemodel.h"
#include "containerxmlparserexception.h"
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTableView>
#include <QList>
#include <QXmlStreamWriter>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dialogGenerateProblem(this),
    dialogGroupData(this),
    dialogAlgorithmExecution(this),
    dialogMeasurementSystem(this),
    boxesGroupsTableModel(new BoxesGroupsTableModel(this)),
    containerPackedBoxesTableModel(new SolutionBoxesTableModel(this))
{
    ui->setupUi(this);
    uiDialogAbout.setupUi(&dialogAbout);
    uiDialogGroupData.setupUi(&dialogGroupData);
    uiDialogAlgorithmExecution.setupUi(&dialogAlgorithmExecution);
    uiDialogGenerateProblem.setupUi(&dialogGenerateProblem);
    uiDialogMeasurementSystem.setupUi(&dialogMeasurementSystem);

    setupColorDialog(&dialogGroupData, &uiDialogGroupData);

    containerSolution.setContainerProblem(&containerProblem);
    ui->openGLWidget->setContainerSolution(&containerSolution);
    ui->tableViewGroups->setModel(boxesGroupsTableModel);
    ui->tableViewOrdering->setModel(containerPackedBoxesTableModel);
    boxesGroupsTableModel->setContainerProblem(&containerProblem);
    containerPackedBoxesTableModel->setContainerSolution(&containerSolution);
    connect(uiDialogMeasurementSystem.buttonBox, &QDialogButtonBox::accepted, [this]
    {
        if (uiDialogMeasurementSystem.radioButtonCentimeters->isChecked() ||
                uiDialogMeasurementSystem.radioButtonInches->isChecked())
            dialogMeasurementSystem.accept();
    });

    auto spinBoxSetValue = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::setValue);
    connect(&containerProblem, &ContainerProblem::containerLengthXChanged,
            ui->spinBoxContainerDimensionX, spinBoxSetValue);
    connect(&containerProblem, &ContainerProblem::containerLengthYChanged,
            ui->spinBoxContainerDimensionY, spinBoxSetValue);
    connect(&containerProblem, &ContainerProblem::containerLengthZChanged,
            ui->spinBoxContainerDimensionZ, spinBoxSetValue);

    auto spinBoxValueChanged = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged);
    connect(ui->spinBoxContainerDimensionX, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthX);
    connect(ui->spinBoxContainerDimensionY, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthY);
    connect(ui->spinBoxContainerDimensionZ, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthZ);

    // connect the slider to the GLContainerWidget
    connect(ui->sliderDisplayedBoxes, &QSlider::valueChanged, this, &MainWindow::setMaximumDisplayedBoxes);

    connect(&containerSolution, &ContainerSolution::afterDataChange, [this]()
    {
        // update the maximum value in the slider and set the value to the maximum
        ui->sliderDisplayedBoxes->setMaximum(containerSolution.boxesCount());
        ui->sliderDisplayedBoxes->setValue(containerSolution.boxesCount());
    });

    ContainerProblemSolver *containerProblemSolver = new ContainerProblemSolver(this);
    containerProblemSolver->moveToThread(&workerThread);
    connect(this, &MainWindow::problemReady, &dialogAlgorithmExecution, &QDialog::show);
    connect(this, &MainWindow::problemReady, containerProblemSolver, &ContainerProblemSolver::solveProblem);
    connect(containerProblemSolver, &ContainerProblemSolver::solutionReady,
            &containerSolution, &ContainerSolution::setSolutionData);
    connect(containerProblemSolver, &ContainerProblemSolver::solutionReady,
            &dialogAlgorithmExecution, &QDialog::close);
    workerThread.start();
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    delete ui;
}

void MainWindow::setMaximumDisplayedBoxes(int value)
{
    ui->openGLWidget->setDisplayedBoxesLimit(value);
    if (value > 0)
    {
        int index = value - 1;
        int lastBoxGroupIndex = containerSolution.boxGroupIndex(index);
        QString groupName = containerProblem.groupName(lastBoxGroupIndex);
        int posX = containerSolution.boxCoordinateX(index);
        int posY = containerSolution.boxCoordinateY(index);
        int posZ = containerSolution.boxCoordinateZ(index);
        int dimX = containerSolution.boxLengthX(index);
        int dimY = containerSolution.boxLengthY(index);
        int dimZ = containerSolution.boxLengthZ(index);
        QString label = tr("Caja nro. %1: Tomada del grupo %2 (%3)\n"
                           "Posición: (%4, %5, %6)\n"
                           "Orientación: (%7, %8, %9)\n")
                .arg(value).arg(lastBoxGroupIndex + 1).arg(groupName)
                .arg(posX).arg(posY).arg(posZ)
                .arg(dimX).arg(dimY).arg(dimZ);
        ui->labelLastBox->setText(label);
    }
    else
    {
        ui->labelLastBox->setText("Contenedor vacío");
    }
}

void MainWindow::setupColorDialog(QDialog *dialog, Ui::DialogGroupData *uiDialog)
{
    connect(uiDialog->pushButtonPickColor, &QPushButton::clicked, [=]
    {
        QPalette palette = uiDialog->labelColor->palette();
        QColor oldColor = palette.background().color();
        QColor color = QColorDialog::getColor(oldColor, dialog, tr("Elegir color"));
        if (color.isValid())
        {
            uiDialog->labelColor->setText(color.name());
            uiDialog->labelColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                                     .arg(color.red())
                                                     .arg(color.green())
                                                     .arg(color.blue()));
        }
    });
}

void MainWindow::on_actionGenerateProblem_triggered()
{
    if (dialogGenerateProblem.exec() == QDialog::Accepted)
    {
        int minimumDimension = uiDialogGenerateProblem.spinBoxMinimumDimension->value();
        int maximumDimension = uiDialogGenerateProblem.spinBoxMaximumDimension->value();
        int fillPercentage = uiDialogGenerateProblem.spinBoxFillPercentage->value();
        int maximumDifferentBoxes = uiDialogGenerateProblem.spinBoxDifferentTypes->value();
        containerProblemGenerator.generate(minimumDimension,
                                           maximumDimension,
                                           fillPercentage,
                                           maximumDifferentBoxes,
                                           containerProblem);
    }
}

void MainWindow::on_actionSolveProblem_triggered()
{
    if (ui->spinBoxContainerDimensionX->value() == 0 ||
            ui->spinBoxContainerDimensionY->value() == 0 ||
            ui->spinBoxContainerDimensionZ->value() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Todas las dimensiones del contenedor deben ser mayor que cero."));
        return;
    }
    if (containerProblem.groupsCounter() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("No se han ingresado cajas para procesar."));
        return;
    }
    emit problemReady(containerProblem.data());
}

void MainWindow::on_actionAddGroup_triggered()
{
    dialogGroupData.setWindowTitle("Añadir grupo");
    if (dialogGroupData.exec() == QDialog::Accepted)
    {
        int dimensionX = uiDialogGroupData.spinBoxLengthX->value();
        int dimensionY = uiDialogGroupData.spinBoxLengthY->value();
        int dimensionZ = uiDialogGroupData.spinBoxLengthZ->value();
        int quantity = uiDialogGroupData.spinBoxQuantity->value();
        QColor color = uiDialogGroupData.labelColor->palette().background().color();
        QString description = uiDialogGroupData.lineEditGroupName->text();

        containerProblem.addGroup(dimensionX, dimensionY, dimensionZ, quantity, color, description);
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Abrir archivo"),
                                                    QString(),
                                                    "Problema de planificación de carga (*.xml)");
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::ReadOnly))
        {
            try
            {
                containerXmlParser.readProblem(&file, containerProblem);
                containerSolution.clear();
            }
            catch (ContainerXmlParserException &exception)
            {
                QMessageBox::critical(this, tr("Error"), exception.message());
            }
        }
        else
            QMessageBox::critical(this, tr("Error"),
                                  tr("No se pudo abrir el archivo %1 para lectura").arg(filename));
    }
}

void MainWindow::on_actionSaveSolution_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Guardar solución"), QString(),
                                                    "Solución de planificación de carga (*.xml)");
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

void MainWindow::on_actionOpenSolution_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Abrir solución"), QString(),
                                                    "Solución de planificación de carga (*.xml)");
    if (!filename.isNull())
    {
        QFile file(filename);
        if (file.open(QFile::ReadOnly))
        {
            try
            {
                containerXmlParser.readSolution(&file, containerSolution);
            }
            catch (ContainerXmlParserException &exception)
            {
                QMessageBox::critical(this, tr("Error"), exception.message());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Error"),
                                  tr("No se pudo abrir el archivo %1 para lectura\n%1")
                                  .arg(filename)
                                  .arg(file.errorString()));
        }
    }
}

void MainWindow::on_actionNewProblem_triggered()
{
    containerSolution.clear();
    containerProblem.clear();
}

void MainWindow::on_actionDeleteGroup_triggered()
{
    auto selectedIndexes = ui->tableViewGroups->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return;
    int button = QMessageBox::question(this,
                                       tr("Eliminar grupos"),
                                       tr("¿Desea eliminar los grupos seleccionados?"),
                                       QMessageBox::Ok, QMessageBox::Cancel);
    if (button == QMessageBox::Ok)
    {
        QVector<int> indexes;
        foreach (QModelIndex index, selectedIndexes)
        {
            indexes.append(index.row());
        }
        std::sort(indexes.begin(), indexes.end(), std::greater<int>());
        int newSize = std::unique(indexes.begin(), indexes.end()) - indexes.begin();
        indexes.resize(newSize);
        foreach (int index, indexes)
        {
            containerProblem.removeGroup(index);
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    dialogAbout.show();
}

void MainWindow::on_actionEditGroup_triggered()
{
    QModelIndexList selectedIndexes = ui->tableViewGroups->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return;
    dialogGroupData.setWindowTitle("Modificar grupo");
    QModelIndex index = selectedIndexes.front();
    int row = index.row();
    int lengthX = containerProblem.groupLengthX(row);
    int lengthY = containerProblem.groupLengthY(row);
    int lengthZ = containerProblem.groupLengthZ(row);
    int quantity = containerProblem.groupBoxesCounter(row);
    QColor color = containerProblem.groupColor(row);
    QString name = containerProblem.groupName(row);

    uiDialogGroupData.spinBoxLengthX->setValue(lengthX);
    uiDialogGroupData.spinBoxLengthY->setValue(lengthY);
    uiDialogGroupData.spinBoxLengthZ->setValue(lengthZ);
    uiDialogGroupData.spinBoxQuantity->setValue(quantity);
    uiDialogGroupData.labelColor->setText(color.name());
    uiDialogGroupData.labelColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                                .arg(color.red())
                                                .arg(color.green())
                                                .arg(color.blue()));

    uiDialogGroupData.lineEditGroupName->setText(name);
    if (dialogGroupData.exec() == QDialog::Accepted)
    {
        lengthX = uiDialogGroupData.spinBoxLengthX->value();
        lengthY = uiDialogGroupData.spinBoxLengthY->value();
        lengthZ = uiDialogGroupData.spinBoxLengthZ->value();
        quantity = uiDialogGroupData.spinBoxQuantity->value();
        color = uiDialogGroupData.labelColor->palette().background().color();
        name = uiDialogGroupData.lineEditGroupName->text();
        containerProblem.setGroup(row, lengthX, lengthY, lengthZ, quantity, color, name);
    }
}
