#include "algorithmexecutiondialog.h"
#include "ui_algorithmexecutiondialog.h"

AlgorithmExecutionDialog::AlgorithmExecutionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlgorithmExecutionDialog)
{
    ui->setupUi(this);
}

AlgorithmExecutionDialog::~AlgorithmExecutionDialog()
{
    delete ui;
}
