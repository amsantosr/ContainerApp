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
#endif
    return a.exec();
}
