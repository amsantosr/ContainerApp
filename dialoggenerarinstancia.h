#ifndef DIALOGGENERARINSTANCIA_H
#define DIALOGGENERARINSTANCIA_H

#include <QDialog>

namespace Ui {
class DialogGenerarInstancia;
}

class DialogGenerarInstancia : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGenerarInstancia(QWidget *parent = 0);
    ~DialogGenerarInstancia();

public:
    Ui::DialogGenerarInstancia *ui;
};

#endif // DIALOGGENERARINSTANCIA_H
