#include "containerproblemform.h"
#include "ui_containerproblemform.h"

ContainerProblemForm::ContainerProblemForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContainerProblemForm)
{
    ui->setupUi(this);
}

ContainerProblemForm::~ContainerProblemForm()
{
    delete ui;
}
