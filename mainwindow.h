#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QList>
#include "containerproblemgenerator.h"
#include "containerxmlparser.h"
#include "containerproblemsolverthread.h"
#include "ui_dialogalgorithmexecution.h"
#include "ui_dialogaddboxes.h"
#include "ui_dialoggenerateproblem.h"
#include "ui_dialogmeasurementsystem.h"
#include "ui_dialogabout.h"

namespace Ui {
class MainWindow;
}

class ContainerProblemTableModel;
class ContainerSolutionTableModel;
class ContainerSolutionTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void generateProblemFromDialog();
    void setMaximumDisplayedBoxes(int value);
    void setTextUnit(QString text);

public:
#ifdef DEBUG_ISSUES
    void testGenerateProblem();
#endif

public slots:
    void on_actionGenerateProblem_triggered();

    void on_actionSolveProblem_triggered();

    void on_actionAddBoxes_triggered();

    void on_actionSaveProblem_triggered();

    void on_actionOpenProblem_triggered();
    
    void on_actionSaveSolution_triggered();

    void on_actionOpenSolution_triggered();

    void on_actionNewProblem_triggered();

    void on_actionDeleteBoxes_triggered();

    void on_actionAbout_triggered();

    void on_actionSetMeasurementSystem_triggered();


private slots:
    void on_actionEditBoxes_triggered();

private:
    Ui::MainWindow *ui;
    QDialog dialogGenerateProblem;
    Ui::DialogGenerateProblem uiDialogGenerateProblem;
    QDialog dialogAddBoxes;
    Ui::DialogAddBox uiDialogAddBoxes;
    QDialog dialogEditBoxes;
    Ui::DialogAddBox uiDialogEditBoxes;
    QDialog dialogAlgorithmExecution;
    Ui::DialogAlgorithmExecution uiDialogAlgorithmExecution;
    QDialog dialogMeasurementSystem;
    Ui::DialogMeasurementSystem uiDialogMeasurementSystem;
    QDialog dialogAbout;
    Ui::DialogAbout uiDialogAbout;

    ContainerProblemGenerator containerProblemGenerator;
    ContainerProblemSolverThread containerProblemSolverThread;
    ContainerProblem containerProblem;
    ContainerSolution containerSolution;
    ContainerXmlParser containerXmlParser;
    ContainerProblemTableModel *containerProblemTableModel;
    ContainerSolutionTableModel *containerSolutionTableModel;

    QList<QLabel*> listLabelsUnits;
};

#endif // MAINWINDOW_H
