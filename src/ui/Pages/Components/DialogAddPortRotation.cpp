#include "DialogAddPortRotation.h"
#include "ui_DialogAddPortRotation.h"

DialogAddPortRotation::DialogAddPortRotation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddPortRotation)
{
    ui->setupUi(this);
}

DialogAddPortRotation::~DialogAddPortRotation()
{
    delete ui;
}
