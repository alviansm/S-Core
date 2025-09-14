#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DockWidget.h"

#include "WelcomePage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui_MainWindow)
    , m_DockManager(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("S-Core");

    setupDockManager();
    createContents();
}

MainWindow::~MainWindow()
{
    // Clean up dock manager before UI
    if (m_DockManager) {
        m_DockManager->deleteLater();
    }
    delete ui;
}

void MainWindow::setupDockManager()
{
    ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHideDisabledButtons, true);
    ads::CDockManager::setAutoHideConfigFlags({ads::CDockManager::DefaultAutoHideConfig});

    // Create the dock manager
    QVBoxLayout* Layout = new QVBoxLayout(ui->dockContainer);
    Layout->setContentsMargins(QMargins(6, 6, 0, 6));
    m_DockManager = new ads::CDockManager(ui->dockContainer);
    Layout->addWidget(m_DockManager);

    m_DockManager->setDockWidgetToolBarStyle(Qt::ToolButtonIconOnly, ads::CDockWidget::StateFloating);
}

void MainWindow::createContents()
{
    auto DockWidget = createWelcomePage();

    m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DockWidget);
}

ads::CDockWidget *MainWindow::createWelcomePage()
{
    auto w = new WelcomePage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Welcome");
    DockWidget->setWidget(w, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");
    return DockWidget;
}
