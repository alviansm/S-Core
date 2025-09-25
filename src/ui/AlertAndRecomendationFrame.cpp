#include "AlertAndRecomendationFrame.h"
#include "ui_AlertAndRecomendationFrame.h"

AlertAndRecomendationFrame::AlertAndRecomendationFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::AlertAndRecomendationFrame)
{
    ui->setupUi(this);
}

AlertAndRecomendationFrame::~AlertAndRecomendationFrame()
{
    delete ui;
}
