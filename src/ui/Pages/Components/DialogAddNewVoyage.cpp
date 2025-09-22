#include "DialogAddNewVoyage.h"
#include "ui_DialogAddNewVoyage.h"

DialogAddNewVoyage::DialogAddNewVoyage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddNewVoyage)
{
    ui->setupUi(this);
}

DialogAddNewVoyage::~DialogAddNewVoyage()
{
    delete ui;
}
