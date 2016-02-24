#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "containerproblemgenerator.h"
#include "containerproblemsolver.h"

namespace Ui {
class MainWindow;
}

class DialogGenerarInstancia;
class DialogAnadirCaja;
class ContainerProblemTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

#ifdef DEBUG_ISSUES
    void testGenerateInstance();
#endif

private slots:
    void on_actionGenerarInstanciaDePrueba_triggered();

    //void on_tabWidget_tabCloseRequested(int index);

    //void on_actionResolverProblema_triggered();

    //void on_actionVisualizarSolucion_triggered();

    void on_actionAnadirCaja_triggered();

    void on_actionGuardarDatos_triggered();

    void on_actionAbrirProblema_triggered();
    
private:
    Ui::MainWindow *ui;
    DialogGenerarInstancia *dialogGenerarInstancia;
    DialogAnadirCaja *dialogAnadirCaja;
    ContainerProblemGenerator containerProblemGenerator;
    ContainerProblemSolver containerProblemSolver;
    ContainerProblem containerProblem;
    ContainerSolution containerSolution;
    ContainerProblemTableModel *containerProblemTableModel;
    //void generateTestInstancePlainText(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes);
    void generateTestInstanceTableView(int minLength, int maxLength, int fillPercentage, int maxDifferentBoxes);
    void generateInstanceFromDialog();
};

#endif // MAINWINDOW_H
