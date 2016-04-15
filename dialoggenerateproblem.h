#ifndef DIALOGGENERATEPROBLEM_H
#define DIALOGGENERATEPROBLEM_H

#include <QDialog>

namespace Ui {
class DialogGenerarInstancia;
}

class DialogGenerateProblem : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGenerateProblem(QWidget *parent = 0);
    ~DialogGenerateProblem();

public:
    Ui::DialogGenerarInstancia *ui;
};

#endif // DIALOGGENERATEPROBLEM_H
