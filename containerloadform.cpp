#include "containerloadform.h"
#include "ui_containerloadform.h"

ContainerLoadForm::ContainerLoadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContainerLoadForm)
{
    ui->setupUi(this);
}

ContainerLoadForm::~ContainerLoadForm()
{
    delete ui;
}
