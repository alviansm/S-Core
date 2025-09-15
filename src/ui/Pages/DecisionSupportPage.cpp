#include "DecisionSupportPage.h"
#include "ui_DecisionSupportPage.h"

DecisionSupportPage::DecisionSupportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DecisionSupportPage)
{
    ui->setupUi(this);
}

DecisionSupportPage::~DecisionSupportPage()
{
    delete ui;
}
