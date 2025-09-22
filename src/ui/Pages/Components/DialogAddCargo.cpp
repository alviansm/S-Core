#include "DialogAddCargo.h"
#include "ui_DialogAddCargo.h"

DialogAddCargo::DialogAddCargo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddCargo)
{
    ui->setupUi(this);
}

DialogAddCargo::~DialogAddCargo()
{
    delete ui;
}
