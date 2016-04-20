#ifndef ALGORITHMEXECUTIONDIALOG_H
#define ALGORITHMEXECUTIONDIALOG_H

#include <QDialog>

namespace Ui {
class DialogAlgorithmExecution;
}

class DialogAlgorithmExecution : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAlgorithmExecution(QWidget *parent = 0);
    ~DialogAlgorithmExecution();

private:
    Ui::DialogAlgorithmExecution *ui;
};

#endif // ALGORITHMEXECUTIONDIALOG_H
