#ifndef CONTAINERLOADFORM_H
#define CONTAINERLOADFORM_H

#include <QWidget>

namespace Ui {
class ContainerLoadForm;
}

class ContainerLoadForm : public QWidget
{
    Q_OBJECT

public:
    explicit ContainerLoadForm(QWidget *parent = 0);
    ~ContainerLoadForm();

private:
    Ui::ContainerLoadForm *ui;
};

#endif // CONTAINERLOADFORM_H
