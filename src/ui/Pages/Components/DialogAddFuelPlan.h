#ifndef DIALOGADDFUELPLAN_H
#define DIALOGADDFUELPLAN_H

#include <QDialog>

namespace Ui {
class DialogAddFuelPlan;
}

class DialogAddFuelPlan : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddFuelPlan(QWidget *parent = nullptr);
    ~DialogAddFuelPlan();

private:
    Ui::DialogAddFuelPlan *ui;
};

#endif // DIALOGADDFUELPLAN_H
