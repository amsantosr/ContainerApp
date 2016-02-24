#include "dialoganadircaja.h"
#include "ui_dialoganadircaja.h"

DialogAnadirCaja::DialogAnadirCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAnadirCaja)
{
    ui->setupUi(this);
    ui->spinBoxDimensionX->setValue(10);
    ui->spinBoxDimensionY->setValue(10);
    ui->spinBoxDimensionZ->setValue(10);
    ui->spinBoxCantidad->setValue(1);
}

DialogAnadirCaja::~DialogAnadirCaja()
{
    delete ui;
}
