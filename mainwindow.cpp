#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoggenerarinstancia.h"
#include "ui_dialoggenerarinstancia.h"
#include "Pisinger/testcont.h"
#include "glcontainerwidget.h"
#include "containerproblemtablemodel.h"
#include "containersolutiontablemodel.h"
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTableView>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dialogGenerarInstancia(new DialogGenerarInstancia(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateTestInstancePlainText(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);

    QPlainTextEdit *plainText = new QPlainTextEdit;
    plainText->setFont(QFont("Courier"));
    ui->tabWidget->addTab(plainText, tr("Problema generado"));
    ui->tabWidget->setCurrentWidget(plainText);

    QString string;
    {
        QTextStream output(&string);
        output << " no [ dx, dy, dz]\n";
        for (int index = 0; index < containerProblem.boxCount(); ++index)
        {
            output << qSetFieldWidth(3) << right << index + 1 << qSetFieldWidth(0) << " [";
            output << qSetFieldWidth(3) << right << containerProblem.boxLengthX(index) << qSetFieldWidth(0) << ",";
            output << qSetFieldWidth(3) << right << containerProblem.boxLengthY(index) << qSetFieldWidth(0) << ",";
            output << qSetFieldWidth(3) << right << containerProblem.boxLengthZ(index) << qSetFieldWidth(0) << "]\n";
        }
    }
    plainText->setPlainText(string);
}

void MainWindow::generateTestInstanceTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes)
{
    containerProblemGenerator.generate(minLength, maxLength, fillPercentage, maxDifferentBoxes, containerProblem);

    QTableView *tableView = new QTableView;
    int newIndex = ui->tabWidget->addTab(tableView, tr("Problema generado"));
    ui->tabWidget->setCurrentIndex(newIndex);

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
    ContainerProblemTableModel *model = new ContainerProblemTableModel(this);
    model->initialize(widths, heights, depths);
    tableView->setModel(model);
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
    this->on_actionResolverProblema_triggered();
    this->on_actionVisualizarSolucion_triggered();
}
#endif

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *widget = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    if (widget != 0)
        delete widget;
}

void MainWindow::on_actionResolverProblema_triggered()
{
    containerProblemSolver.solve(containerProblem, containerSolution);
    ContainerSolutionTableModel *model = new ContainerSolutionTableModel(this);
    model->initialize(containerSolution.boxLengthsX(),
                      containerSolution.boxLengthsY(),
                      containerSolution.boxLengthsZ(),
                      containerSolution.boxCoordinatesX(),
                      containerSolution.boxCoordinatesY(),
                      containerSolution.boxCoordinatesZ(),
                      containerSolution.boxPackedFlags());
    QTableView *tableView = new QTableView;
    int currentIndex = ui->tabWidget->addTab(tableView, "Problema resuelto");
    ui->tabWidget->setCurrentIndex(currentIndex);
    tableView->setModel(model);
}

void MainWindow::on_actionVisualizarSolucion_triggered()
{
    GLContainerWidget *glWidget = new GLContainerWidget;
    glWidget->setContainerInfo(containerProblem, containerSolution);
    int currentIndex = ui->tabWidget->addTab(glWidget, tr("Visualización"));
    ui->tabWidget->setCurrentIndex(currentIndex);
    glWidget->updateGL();
}
