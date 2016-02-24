#ifndef DIALOGANADIRCAJA_H
#define DIALOGANADIRCAJA_H

#include <QDialog>

namespace Ui {
class DialogAnadirCaja;
}

class DialogAnadirCaja : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAnadirCaja(QWidget *parent = 0);
    ~DialogAnadirCaja();

public:
    Ui::DialogAnadirCaja *ui;
};

#endif // DIALOGANADIRCAJA_H
