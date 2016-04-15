#include "dialogaddbox.h"
#include "ui_dialogaddbox.h"

DialogAddBox::DialogAddBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddBox)
{
    ui->setupUi(this);
    ui->spinBoxDimensionX->setValue(10);
    ui->spinBoxDimensionY->setValue(10);
    ui->spinBoxDimensionZ->setValue(10);
    ui->spinBoxCantidad->setValue(1);
}

DialogAddBox::~DialogAddBox()
{
    delete ui;
}
