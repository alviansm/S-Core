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

signals:
    void onFuelPlanAdded(QString type, QString quantity, QString speed, QString units);

private:
    Ui::DialogAddFuelPlan *ui;
};

#endif // DIALOGADDFUELPLAN_H
