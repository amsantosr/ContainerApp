#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "containerproblemgenerator.h"
#include "containerproblemsolver.h"
#include "algorithmthread.h"
#include "dialogalgorithmexecution.h"
#include "ui_dialogalgorithmexecution.h"

namespace Ui {
class MainWindow;
}

class DialogGenerateProblem;
class DialogAddBox;
class ContainerProblemTableModel;
class ContainerSolutionTableModel;
class BoxesOrderingTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void solveProblem();

#ifdef DEBUG_ISSUES
    void testGenerateInstance();
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

private:
    Ui::MainWindow *ui;
    DialogGenerateProblem *dialogGenerateProblem;
    DialogAddBox *dialogAddBox;
    ContainerProblemGenerator containerProblemGenerator;
    ContainerProblemSolver containerProblemSolver;
    ContainerProblem containerProblem;
    ContainerSolution containerSolution;
    ContainerProblemTableModel *containerProblemTableModel;
    ContainerSolutionTableModel *containerSolutionTableModel;
    BoxesOrderingTableModel *boxesOrderingTableModel;
    AlgorithmThread algorithmThread;
    QDialog dialogAlgorithmExecution;
    Ui::DialogAlgorithmExecution *UiAlgorithmExecution;
    void generateTestInstanceTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes);
    void generateInstanceFromDialog();
};

#endif // MAINWINDOW_H
