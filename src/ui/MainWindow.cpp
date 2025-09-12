#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DockWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui_MainWindow)
    , m_dockManager(nullptr)
    , m_centralPages(nullptr)
    , m_dashboardPage(nullptr)
    , m_technicalPage(nullptr)
    , m_savingsPage(nullptr)
    , m_voyagePlanningPage(nullptr)
    , m_historyPage(nullptr)
    , m_settingsPage(nullptr)
{
    ui->setupUi(this);
    setupDockManager();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDockManager()
{
    // Create the dock manager
    m_dockManager = new ads::CDockManager(ui->dockContainer);
    
    // Configure dock manager settings
    ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::XmlCompressionEnabled, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AllTabsHaveCloseButton, false);
}
