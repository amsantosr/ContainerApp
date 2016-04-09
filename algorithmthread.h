#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H

#include <QThread>

class MainWindow;

class AlgorithmThread : public QThread
{
public:
    AlgorithmThread(MainWindow *window);
    void run();

private:
    MainWindow *mainWindow;
};

#endif // ALGORITHMTHREAD_H
