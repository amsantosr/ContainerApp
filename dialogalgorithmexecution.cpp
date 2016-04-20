#include "dialogalgorithmexecution.h"
#include "ui_dialogalgorithmexecution.h"

DialogAlgorithmExecution::DialogAlgorithmExecution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAlgorithmExecution)
{
    ui->setupUi(this);
}

DialogAlgorithmExecution::~DialogAlgorithmExecution()
{
    delete ui;
}
