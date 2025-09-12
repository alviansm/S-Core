#include "PageWidgets.h"
#include "ui_DashboardPage.h"
#include "ui_TechnicalPage.h"
#include "ui_SavingsPage.h"
#include "ui_VoyagePlanningPage.h"
#include "ui_HistoryPage.h"
#include "ui_SettingsPage.h"

// Dashboard Page Implementation
DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_DashboardPage)
{
    ui->setupUi(this);
}

DashboardPage::~DashboardPage()
{
    delete ui;
}

// Technical Page Implementation
TechnicalPage::TechnicalPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_TechnicalPage)
{
    ui->setupUi(this);
}

TechnicalPage::~TechnicalPage()
{
    delete ui;
}

// Savings Page Implementation
SavingsPage::SavingsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_SavingsPage)
{
    ui->setupUi(this);
}

SavingsPage::~SavingsPage()
{
    delete ui;
}

// Voyage Planning Page Implementation
VoyagePlanningPage::VoyagePlanningPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_VoyagePlanningPage)
{
    ui->setupUi(this);
}

VoyagePlanningPage::~VoyagePlanningPage()
{
    delete ui;
}

// History Page Implementation
HistoryPage::HistoryPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_HistoryPage)
{
    ui->setupUi(this);
}

HistoryPage::~HistoryPage()
{
    delete ui;
}

// Settings Page Implementation
SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_SettingsPage)
{
    ui->setupUi(this);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}