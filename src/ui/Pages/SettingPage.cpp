#include "SettingPage.h"
#include "ui_SettingPage.h"

SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingPage)
{
    ui->setupUi(this);
}

SettingPage::~SettingPage()
{
    delete ui;
}
