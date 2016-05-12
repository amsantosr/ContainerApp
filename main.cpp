#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
#ifdef DEBUG_ISSUES
    a.processEvents();
    w.testGenerateProblem();
    a.processEvents();
    w.on_actionSolveProblem_triggered();
#endif
    return a.exec();
}
