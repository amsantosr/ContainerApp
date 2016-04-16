#ifndef DIALOGGENERATEPROBLEM_H
#define DIALOGGENERATEPROBLEM_H

#include <QDialog>

namespace Ui {
class DialogGenerateProblem;
}

class DialogGenerateProblem : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGenerateProblem(QWidget *parent = 0);
    ~DialogGenerateProblem();

public:
    Ui::DialogGenerateProblem *ui;
};

#endif // DIALOGGENERATEPROBLEM_H
