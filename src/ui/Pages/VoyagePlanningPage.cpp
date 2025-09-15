#include "VoyagePlanningPage.h"
#include "ui_VoyagePlanningPage.h"

VoyagePlanningPage::VoyagePlanningPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VoyagePlanningPage)
{
    ui->setupUi(this);
}

VoyagePlanningPage::~VoyagePlanningPage()
{
    delete ui;
}
