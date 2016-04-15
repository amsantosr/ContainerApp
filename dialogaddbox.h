#ifndef DIALOGADDBOX_H
#define DIALOGADDBOX_H

#include <QDialog>

namespace Ui {
class DialogAddBox;
}

class DialogAddBox : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddBox(QWidget *parent = 0);
    ~DialogAddBox();

public:
    Ui::DialogAddBox *ui;
};

#endif // DIALOGADDBOX_H
