#include "DialogAddFuelPlan.h"
#include "ui_DialogAddFuelPlan.h"

DialogAddFuelPlan::DialogAddFuelPlan(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddFuelPlan)
{
    ui->setupUi(this);
}

DialogAddFuelPlan::~DialogAddFuelPlan()
{
    delete ui;
}
