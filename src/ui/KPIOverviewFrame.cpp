#include "KPIOverviewFrame.h"
#include "ui_KPIOverviewFrame.h"

KPIOverviewFrame::KPIOverviewFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::KPIOverviewFrame)
{
    ui->setupUi(this);
}

KPIOverviewFrame::~KPIOverviewFrame()
{
    delete ui;
}
