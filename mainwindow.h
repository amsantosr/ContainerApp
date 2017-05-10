#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QList>
#include "containerproblemgenerator.h"
#include "containerxmlparser.h"
#include "containerproblemsolver.h"
#include "ui_dialogalgorithmexecution.h"
#include "ui_dialoggroupdata.h"
#include "ui_dialoggenerateproblem.h"
#include "ui_dialogmeasurementsystem.h"
#include "ui_dialogabout.h"

namespace Ui {
class MainWindow;
}

class BoxesGroupsTableModel;
class SolutionBoxesTableModel;
class SolutionBoxesTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setMaximumDisplayedBoxes(int value);
    void setupColorDialog(QDialog *dialog, Ui::DialogGroupData *uiDialog);

signals:
    void problemReady(const ProblemData &problemData);

public slots:
    void on_actionGenerateProblem_triggered();

    void on_actionSolveProblem_triggered();

    void on_actionAddGroup_triggered();

    void on_actionDeleteGroup_triggered();

    void on_actionEditGroup_triggered();

    void on_actionSaveProblem_triggered();

    void on_actionOpenProblem_triggered();
    
    void on_actionSaveSolution_triggered();

    void on_actionOpenSolution_triggered();

    void on_actionNewProblem_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QDialog dialogGenerateProblem;
    Ui::DialogGenerateProblem uiDialogGenerateProblem;
    QDialog dialogGroupData;
    Ui::DialogGroupData uiDialogGroupData;
    QDialog dialogAlgorithmExecution;
    Ui::DialogAlgorithmExecution uiDialogAlgorithmExecution;
    QDialog dialogMeasurementSystem;
    Ui::DialogMeasurementSystem uiDialogMeasurementSystem;
    QDialog dialogAbout;
    Ui::DialogAbout uiDialogAbout;

    ContainerProblemGenerator containerProblemGenerator;
    ContainerProblem containerProblem;
    ContainerSolution containerSolution;
    ContainerXmlParser containerXmlParser;
    BoxesGroupsTableModel *boxesGroupsTableModel;
    SolutionBoxesTableModel *containerPackedBoxesTableModel;
    QThread workerThread;
};

#endif // MAINWINDOW_H
