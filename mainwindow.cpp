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
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::generateTestInstancePlainText(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
//{
//    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);

//    QPlainTextEdit *plainText = new QPlainTextEdit;
//    plainText->setFont(QFont("Courier"));
//    ui->tabWidget->addTab(plainText, tr("Problema generado"));
//    ui->tabWidget->setCurrentWidget(plainText);

//    QString string;
//    {
//        QTextStream output(&string);
//        output << " no [ dx, dy, dz]\n";
//        for (int index = 0; index < containerProblem.boxCount(); ++index)
//        {
//            output << qSetFieldWidth(3) << right << index + 1 << qSetFieldWidth(0) << " [";
//            output << qSetFieldWidth(3) << right << containerProblem.boxLengthX(index) << qSetFieldWidth(0) << ",";
//            output << qSetFieldWidth(3) << right << containerProblem.boxLengthY(index) << qSetFieldWidth(0) << ",";
//            output << qSetFieldWidth(3) << right << containerProblem.boxLengthZ(index) << qSetFieldWidth(0) << "]\n";
//        }
//    }
//    plainText->setPlainText(string);
//}

void MainWindow::generateTestInstanceTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);

    QList<int> widths, heights, depths;
    widths.reserve(containerProblem.boxCount());
    heights.reserve(containerProblem.boxCount());
    depths.reserve(containerProblem.boxCount());
    for (int index = 0; index < containerProblem.boxCount(); ++index)
    {
        widths.append(containerProblem.boxLengthX(index));
        heights.append(containerProblem.boxLengthY(index));
        depths.append(containerProblem.boxLengthZ(index));
    }
    containerProblemTableModel->initialize(widths, heights, depths);
    ui->spinBoxContainerDimensionX->setValue(containerProblem.containerLengthX());
    ui->spinBoxContainerDimensionY->setValue(containerProblem.containerLengthY());
    ui->spinBoxContainerDimensionZ->setValue(containerProblem.containerLengthZ());
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
    //this->on_actionResolverProblema_triggered();
    //this->on_actionVisualizarSolucion_triggered();
}
#endif

//void MainWindow::on_tabWidget_tabCloseRequested(int index)
//{
//    QWidget *widget = ui->tabWidget->widget(index);
//    ui->tabWidget->removeTab(index);
//    if (widget != 0)
//        delete widget;
//}

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
            containerProblemTableModel->addBox(dimensionX, dimensionY, dimensionZ);
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
            int boxCount = containerProblemTableModel->lengthXValues.size();
            for (int index = 0; index < boxCount; ++index)
            {
                stream.writeEmptyElement("Box");
                int lengthX = containerProblemTableModel->lengthXValues[index];
                int lengthY = containerProblemTableModel->lengthYValues[index];
                int lengthZ = containerProblemTableModel->lengthZValues[index];
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
            containerProblemTableModel->clear();
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
                                ui->spinBoxContainerDimensionX->setValue(dimensionX);
                                containerProblem.setContainerLengthX(dimensionX);
                            }
                            else if (attribute.name() == "DimensionY")
                            {
                                int dimensionY = attribute.value().toInt();
                                ui->spinBoxContainerDimensionY->setValue(dimensionY);
                                containerProblem.setContainerLengthY(dimensionY);
                            }
                            else if (attribute.name() == "DimensionZ")
                            {
                                int dimensionZ = attribute.value().toInt();
                                ui->spinBoxContainerDimensionZ->setValue(dimensionZ);
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
                        containerProblemTableModel->addBox(boxDimensionX, boxDimensionY, boxDimensionZ);
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
            int boxCount = containerProblemTableModel->lengthXValues.size();
            for (int index = 0; index < boxCount; ++index)
            {
                stream.writeEmptyElement("Box");
                int posX = containerSolution.boxCoordinateX(index);
                int posY = containerSolution.boxCoordinateY(index);
                int posZ = containerSolution.boxCoordinateZ(index);
                stream.writeAttribute("PositionX", QString::number(posX));
                stream.writeAttribute("PositionY", QString::number(posY));
                stream.writeAttribute("PositionZ", QString::number(posZ));
            }
            stream.writeEndElement();
            stream.writeEndDocument();
        }
    }
}
