#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QList>
#include "containerproblemgenerator.h"
#include "containerxmlparser.h"
#include "containerproblemsolverthread.h"
#include "ui_dialogalgorithmexecution.h"
#include "ui_dialogaddgroup.h"
#include "ui_dialoggenerateproblem.h"
#include "ui_dialogmeasurementsystem.h"
#include "ui_dialogabout.h"

namespace Ui {
class MainWindow;
}

class ContainerGroupsTableModel;
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
    void generateProblemFromDialog();
    void setMaximumDisplayedBoxes(int value);
    void setTextUnit(QString text);
    void setupColorDialog(QDialog *dialog, Ui::DialogAddGroup *uiDialog);

public:
#ifdef DEBUG_ISSUES
    void testGenerateProblem();
#endif

public slots:
    void on_actionGenerateProblem_triggered();

    void on_actionSolveProblem_triggered();

    void on_actionAddGroup_triggered();

    void on_actionSaveProblem_triggered();

    void on_actionOpenProblem_triggered();
    
    void on_actionSaveSolution_triggered();

    void on_actionOpenSolution_triggered();

    void on_actionNewProblem_triggered();

    void on_actionDeleteGroup_triggered();

    void on_actionAbout_triggered();

private slots:
    void on_actionEditGroup_triggered();

    void on_tableViewBoxes_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QDialog dialogGenerateProblem;
    Ui::DialogGenerateProblem uiDialogGenerateProblem;
    QDialog dialogAddGroup;
    Ui::DialogAddGroup uiDialogAddGroup;
    QDialog dialogEditGroup;
    Ui::DialogAddGroup uiDialogEditGroup;
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
    BoxesGroupsTableModel *containerProblemTableModel;
    SolutionBoxesTableModel *containerPackedBoxesTableModel;
    ContainerGroupsTableModel *containerGroupsTableModel;
};

#endif // MAINWINDOW_H
