#ifndef CONTAINERPROBLEMFORM_H
#define CONTAINERPROBLEMFORM_H

#include <QWidget>

namespace Ui {
class ContainerProblemForm;
}

class ContainerProblemForm : public QWidget
{
    Q_OBJECT

public:
    explicit ContainerProblemForm(QWidget *parent = 0);
    ~ContainerProblemForm();

private:
    Ui::ContainerProblemForm *ui;
};

#endif // CONTAINERPROBLEMFORM_H
