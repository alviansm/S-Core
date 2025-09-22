#ifndef DIALOGADDCARGO_H
#define DIALOGADDCARGO_H

#include <QDialog>

namespace Ui {
class DialogAddCargo;
}

class DialogAddCargo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddCargo(QWidget *parent = nullptr);
    ~DialogAddCargo();

private:
    Ui::DialogAddCargo *ui;
};

#endif // DIALOGADDCARGO_H
