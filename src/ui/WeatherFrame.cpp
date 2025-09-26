#include "WeatherFrame.h"
#include "ui_WeatherFrame.h"

WeatherFrame::WeatherFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::WeatherFrame)
{
    ui->setupUi(this);
}

WeatherFrame::~WeatherFrame()
{
    delete ui;
}
