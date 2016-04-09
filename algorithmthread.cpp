#include "algorithmthread.h"
#include "mainwindow.h"

AlgorithmThread::AlgorithmThread(MainWindow *window)
    : QThread(window), mainWindow(window)
{

}

void AlgorithmThread::run()
{
    mainWindow->solveProblem();
}
