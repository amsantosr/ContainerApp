﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialoggenerateproblem.h"
#include "ui_dialogabout.h"
#include "Pisinger/testcont.h"
#include "glcontainerwidget.h"
#include "containerproblemtablemodel.h"
#include "containersolutiontablemodel.h"
#include "containersolutiontablemodel.h"
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
    dialogAddGroup(this),
    dialogEditGroup(this),
    dialogAlgorithmExecution(this),
    dialogMeasurementSystem(this),
    containerProblemTableModel(new ContainerProblemTableModel(this)),
    containerSolutionTableModel(new ContainerSolutionTableModel(this))
{
    ui->setupUi(this);
    uiDialogAbout.setupUi(&dialogAbout);
    uiDialogAddGroup.setupUi(&dialogAddGroup);
    uiDialogEditGroup.setupUi(&dialogEditGroup);
    uiDialogAlgorithmExecution.setupUi(&dialogAlgorithmExecution);
    uiDialogGenerateProblem.setupUi(&dialogGenerateProblem);
    uiDialogMeasurementSystem.setupUi(&dialogMeasurementSystem);

    setupColorDialog(&dialogAddGroup, &uiDialogAddGroup);
    setupColorDialog(&dialogEditGroup, &uiDialogEditGroup);

    ui->splitterHorizontal->setStretchFactor(0, 0);
    ui->splitterHorizontal->setStretchFactor(1, 1);
    ui->tableViewBoxes->setModel(containerProblemTableModel);
    ui->tableViewSolution->setModel(containerSolutionTableModel);
    containerProblemTableModel->setContainerProblem(&containerProblem);
    containerSolutionTableModel->setContainerSolution(&containerSolution);
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
    connect(&containerProblem, &ContainerProblem::textUnitChanged,
            this, &MainWindow::setTextUnit);
    containerSolution.setContainerProblem(&containerProblem);
    ui->openGLWidget->setContainerSolution(&containerSolution);

    auto spinBoxValueChanged = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged);
    connect(ui->spinBoxContainerDimensionX, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthX);
    connect(ui->spinBoxContainerDimensionY, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthY);
    connect(ui->spinBoxContainerDimensionZ, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthZ);

    listLabelsUnits << uiDialogAddGroup.labelUnit1
                    << uiDialogAddGroup.labelUnit2
                    << uiDialogAddGroup.labelUnit3
                    << uiDialogEditGroup.labelUnit1
                    << uiDialogEditGroup.labelUnit2
                    << uiDialogEditGroup.labelUnit3
                    << uiDialogGenerateProblem.labelUnit1
                    << uiDialogGenerateProblem.labelUnit2
                    << ui->labelContainerUnit1
                    << ui->labelContainerUnit2
                    << ui->labelContainerUnit3;

    setTextUnit(containerProblem.textUnit());

    // connect the slider to the GLContainerWidget
    connect(ui->sliderDisplayedBoxes, &QSlider::valueChanged, this, &MainWindow::setMaximumDisplayedBoxes);

    connect(&containerSolution, &ContainerSolution::afterDataChange, [this]()
    {
        // update the maximum value in the slider and set the value to the maximum
        ui->sliderDisplayedBoxes->setMaximum(containerSolution.packedBoxesCount());
        ui->sliderDisplayedBoxes->setValue(containerSolution.packedBoxesCount());
    });

    containerProblemSolverThread.setContainerProblem(&containerProblem);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::started,
            &dialogAlgorithmExecution, &QDialog::show, Qt::BlockingQueuedConnection);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::finished,
            &dialogAlgorithmExecution, &QDialog::close, Qt::BlockingQueuedConnection);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::solutionReady,
            &containerSolution, &ContainerSolution::setPackedBoxes, Qt::BlockingQueuedConnection);
    connect(uiDialogAlgorithmExecution.pushButtonCancel, &QPushButton::clicked, this, [this]
    {
        uiDialogAlgorithmExecution.pushButtonCancel->setEnabled(false);
        QApplication::processEvents();
        containerProblemSolverThread.terminate();
        containerProblemSolverThread.wait();
        dialogAlgorithmExecution.hide();
        uiDialogAlgorithmExecution.pushButtonCancel->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateProblemFromDialog()
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

void MainWindow::setMaximumDisplayedBoxes(int value)
{
    ui->openGLWidget->setDisplayedBoxesLimit(value);
    if (value > 0)
    {
        int lastBoxIndex = containerSolution.packedBoxGroupIndex(value - 1) + 1;
        ui->labelLastBox->setText(tr("Caja de grupo %1").arg(lastBoxIndex));
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

void MainWindow::setupColorDialog(QDialog *dialog, Ui::DialogAddGroup *uiDialog)
{
    connect(uiDialog->pushButtonPickColor, &QPushButton::clicked, [=]
    {
        QPalette palette = uiDialog->labelColor->palette();
        QColor oldColor = palette.background().color();
        QColor color = QColorDialog::getColor(oldColor, dialog, tr("Elegir color"));
        if (color.isValid())
        {
            QPalette palette = uiDialog->labelColor->palette();
            palette.setColor(uiDialog->labelColor->backgroundRole(), color);
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
    if (containerProblem.groupsCounter() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("No se han ingresado cajas para procesar."));
        return;
    }
    containerProblemSolverThread.start();
}

void MainWindow::on_actionAddGroup_triggered()
{
    if (dialogAddGroup.exec() == QDialog::Accepted)
    {
        int dimensionX = uiDialogAddGroup.spinBoxDimensionX->value();
        int dimensionY = uiDialogAddGroup.spinBoxDimensionY->value();
        int dimensionZ = uiDialogAddGroup.spinBoxDimensionZ->value();
        int quantity = uiDialogAddGroup.spinBoxCantidad->value();
        QColor color = uiDialogAddGroup.labelColor->palette().background().color();
        QString description = uiDialogAddGroup.lineEditDescription->text();

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
    containerProblem.clear();
    containerSolution.clear();
}

void MainWindow::on_actionDeleteGroup_triggered()
{
    auto selectedIndexes = ui->tableViewBoxes->selectedIndexes();
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

void MainWindow::on_actionEditGroup_triggered()
{
    //QVector<int> rows = ui->tableViewBoxes->selectedRows();
    QModelIndexList selectedIndexes = ui->tableViewBoxes->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return;
    on_tableViewBoxes_doubleClicked(selectedIndexes.front());
}

void MainWindow::on_tableViewBoxes_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    int lengthX = containerProblem.groupLengthX(row);
    int lengthY = containerProblem.groupLengthY(row);
    int lengthZ = containerProblem.groupLengthZ(row);
    int quantity = containerProblem.groupBoxesCounter(row);
    QColor color = containerProblem.groupColor(row);
    QString description = containerProblem.groupDescription(row);

    uiDialogEditGroup.spinBoxDimensionX->setValue(lengthX);
    uiDialogEditGroup.spinBoxDimensionY->setValue(lengthY);
    uiDialogEditGroup.spinBoxDimensionZ->setValue(lengthZ);
    uiDialogEditGroup.spinBoxCantidad->setValue(quantity);
    uiDialogEditGroup.labelColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                                .arg(color.red())
                                                .arg(color.green())
                                                .arg(color.blue()));

    uiDialogEditGroup.lineEditDescription->setText(description);
    if (dialogEditGroup.exec() == QDialog::Accepted)
    {
        lengthX = uiDialogEditGroup.spinBoxDimensionX->value();
        lengthY = uiDialogEditGroup.spinBoxDimensionY->value();
        lengthZ = uiDialogEditGroup.spinBoxDimensionZ->value();
        quantity = uiDialogEditGroup.spinBoxCantidad->value();
        color = uiDialogEditGroup.labelColor->palette().background().color();
        description = uiDialogEditGroup.lineEditDescription->text();
        containerProblem.setGroup(row, lengthX, lengthY, lengthZ, quantity, color, description);
    }
}
