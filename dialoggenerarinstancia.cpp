#include "dialoggenerarinstancia.h"
#include "ui_dialoggenerarinstancia.h"

DialogGenerarInstancia::DialogGenerarInstancia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGenerarInstancia)
{
    ui->setupUi(this);
}

DialogGenerarInstancia::~DialogGenerarInstancia()
{
    delete ui;
}
