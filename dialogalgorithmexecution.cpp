#include "dialogalgorithmexecution.h"
#include "ui_algorithmexecutiondialog.h"

DialogAlgorithmExecution::DialogAlgorithmExecution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlgorithmExecutionDialog)
{
    ui->setupUi(this);
}

DialogAlgorithmExecution::~DialogAlgorithmExecution()
{
    delete ui;
}
