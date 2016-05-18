#include "mainwindow.h"
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
    dialogAddBoxes(this),
    dialogEditBoxes(this),
    dialogAlgorithmExecution(this),
    dialogMeasurementSystem(this),
    containerProblemTableModel(new ContainerProblemTableModel(this)),
    containerSolutionTableModel(new ContainerSolutionTableModel(this))
{
    ui->setupUi(this);
    uiDialogAbout.setupUi(&dialogAbout);
    uiDialogAddBoxes.setupUi(&dialogAddBoxes);
    uiDialogEditBoxes.setupUi(&dialogEditBoxes);
    uiDialogAlgorithmExecution.setupUi(&dialogAlgorithmExecution);
    uiDialogGenerateProblem.setupUi(&dialogGenerateProblem);
    uiDialogMeasurementSystem.setupUi(&dialogMeasurementSystem);

    connect(uiDialogAddBoxes.pushButtonPickColor, &QPushButton::clicked, [&]
    {
        QColor color = QColorDialog::getColor(Qt::red, &dialogAddBoxes, tr("Elegir color"));
        if (color.isValid())
        {
            QPalette palette = uiDialogAddBoxes.labelColor->palette();
            palette.setColor(uiDialogAddBoxes.labelColor->backgroundRole(), color);
            uiDialogAddBoxes.labelColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                                     .arg(color.red())
                                                     .arg(color.green())
                                                     .arg(color.blue()));
        }
    });
    uiDialogEditBoxes.spinBoxCantidad->setDisabled(true);

    ui->splitterHorizontal->setStretchFactor(0, 0);
    ui->splitterHorizontal->setStretchFactor(1, 1);
    ui->tableViewBoxes->setModel(containerProblemTableModel);
    ui->tableViewSolution->setModel(containerSolutionTableModel);
    containerProblemTableModel->setContainerProblem(&containerProblem);
    containerSolutionTableModel->setContainerSolution(&containerSolution);
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
    ui->openGLWidget->setContainerSolution(&containerSolution);

    auto spinBoxValueChanged = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged);
    connect(ui->spinBoxContainerDimensionX, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthX);
    connect(ui->spinBoxContainerDimensionY, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthY);
    connect(ui->spinBoxContainerDimensionZ, spinBoxValueChanged,
            &containerProblem, &ContainerProblem::setContainerLengthZ);

    listLabelsUnits << uiDialogAddBoxes.labelUnit1
                    << uiDialogAddBoxes.labelUnit2
                    << uiDialogAddBoxes.labelUnit3
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

    containerProblemSolverThread.setContainerProblem(&containerProblem);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::started,
            &dialogAlgorithmExecution, &QDialog::show, Qt::BlockingQueuedConnection);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::finished,
            &dialogAlgorithmExecution, &QDialog::close, Qt::BlockingQueuedConnection);
    connect(&containerProblemSolverThread, &ContainerProblemSolverThread::solutionReady,
            &containerSolution, &ContainerSolution::setPackedBoxes, Qt::BlockingQueuedConnection);
    connect(uiDialogAlgorithmExecution.pushButtonCancel, &QPushButton::clicked,
            this, [&]
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

void MainWindow::on_actionAddBoxes_triggered()
{
    if (dialogAddBoxes.exec() == QDialog::Accepted)
    {
        int dimensionX = uiDialogAddBoxes.spinBoxDimensionX->value();
        int dimensionY = uiDialogAddBoxes.spinBoxDimensionY->value();
        int dimensionZ = uiDialogAddBoxes.spinBoxDimensionZ->value();
        int quantity = uiDialogAddBoxes.spinBoxCantidad->value();
        QColor color = uiDialogAddBoxes.labelColor->palette().background().color();
        QString description = uiDialogAddBoxes.lineEditDescription->text();

        containerProblem.addBox(dimensionX, dimensionY, dimensionZ, quantity, color, description);
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

void MainWindow::on_actionDeleteBoxes_triggered()
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

void MainWindow::on_actionEditBoxes_triggered()
{
    QVector<int> rows = ui->tableViewBoxes->selectedRows();
    if (!rows.isEmpty())
    {
        if (dialogEditBoxes.exec() == QDialog::Accepted)
        {
            int dimensionX = uiDialogAddBoxes.spinBoxDimensionX->value();
            int dimensionY = uiDialogAddBoxes.spinBoxDimensionY->value();
            int dimensionZ = uiDialogAddBoxes.spinBoxDimensionZ->value();
            int quantity = uiDialogAddBoxes.spinBoxCantidad->value();
            QColor color = uiDialogAddBoxes.labelColor->palette().background().color();
            QString description = uiDialogAddBoxes.lineEditDescription->text();

            containerProblem.addBox(dimensionX, dimensionY, dimensionZ, quantity, color, description);
        }
    }
}
