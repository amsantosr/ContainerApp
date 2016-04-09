#ifndef ALGORITHMEXECUTIONDIALOG_H
#define ALGORITHMEXECUTIONDIALOG_H

#include <QDialog>

namespace Ui {
class AlgorithmExecutionDialog;
}

class AlgorithmExecutionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlgorithmExecutionDialog(QWidget *parent = 0);
    ~AlgorithmExecutionDialog();

private:
    Ui::AlgorithmExecutionDialog *ui;
};

#endif // ALGORITHMEXECUTIONDIALOG_H
