#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "containerproblemgenerator.h"
#include "containerproblemsolver.h"
#include "ui_dialogalgorithmexecution.h"
#include "ui_dialogaddbox.h"
#include "ui_dialoggenerateproblem.h"

namespace Ui {
class MainWindow;
}

class ContainerProblemTableModel;
class ContainerSolutionTableModel;
class BoxesOrderingTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void generateProblemTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes);
    void generateProblemFromDialog();
    void setMaximumDisplayedBoxes(int value);

signals:
    void solveProblemAsync(ContainerProblem *problem, ContainerSolution *solution);

public:
#ifdef DEBUG_ISSUES
    void testGenerateProblem();
#endif

private slots:
    void on_actionGenerateProblem_triggered();

    void on_actionSolveProblem_triggered();

    void on_actionAddBox_triggered();

    void on_actionSaveProblem_triggered();

    void on_actionOpenProblem_triggered();
    
    void on_actionSaveSolution_triggered();

    void on_actionNewProblem_triggered();

    void on_actionDeleteBox_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QDialog dialogGenerateProblem;
    Ui::DialogGenerateProblem uiDialogGenerateProblem;
    QDialog dialogAddBox;
    Ui::DialogAddBox uiDialogAddBox;
    QDialog dialogAlgorithmExecution;
    Ui::DialogAlgorithmExecution uiDialogAlgorithmExecution;
    ContainerProblemGenerator containerProblemGenerator;
    ContainerProblemSolver containerProblemSolver;
    ContainerProblem containerProblem;
    ContainerSolution containerSolution;
    ContainerProblemTableModel *containerProblemTableModel;
    ContainerSolutionTableModel *containerSolutionTableModel;
    BoxesOrderingTableModel *boxesOrderingTableModel;
    QThread threadWorker;
    QDialog dialogAbout;
};

#endif // MAINWINDOW_H
