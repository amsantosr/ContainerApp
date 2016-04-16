#include "dialoggenerateproblem.h"
#include "ui_dialoggenerateproblem.h"

DialogGenerateProblem::DialogGenerateProblem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGenerateProblem)
{
    ui->setupUi(this);
}

DialogGenerateProblem::~DialogGenerateProblem()
{
    delete ui;
}
