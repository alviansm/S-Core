#include "TechnicalPage.h"
#include "ui_TechnicalPage.h"

TechnicalPage::TechnicalPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TechnicalPage)
{
    ui->setupUi(this);
}

TechnicalPage::~TechnicalPage()
{
    delete ui;
}
