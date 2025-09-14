#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DockWidget.h"

#include "WelcomePage.h"
#include "MapboxWidget.h"

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

    QString darkStyle = R"(
        /*===========================================================================
         * QtAdvancedDockingSystem - DockManager
         *===========================================================================*/
        ads--CDockContainerWidget
        {
            background: rgb(31, 31, 31);
        }

        ads--CDockSplitter::handle:horizontal
        {
            image: none;
        }

        ads--CDockSplitter::handle:vertical
        {
            image: none;
        }


        /*===========================================================================
         * QtAdvancedDockingSystem - DockArea
         *===========================================================================*/
        ads--CDockAreaWidget
        {
            border-top: none;
            background: rgb(51, 51, 51);
        }


        /*===========================================================================
         * QtAdvancedDockingSystem - DockArea Titlebar
         *===========================================================================*/
        ads--CDockAreaTitleBar
        {
            background: rgb(81, 81, 81);
            border-bottom: 1px solid palette(highlight);
            min-height: 23px;
        }


        ads--CDockAreaTitleBar:disabled
        {
            border-bottom: 1px solid rgb(140, 140, 140);
        }

        ads--CDockAreaTitleBar > QToolButton
        {
            background: rgb(81, 81, 81);
            padding: 3 2px;
        }

        ads--CDockAreaTitleBar > QToolButton:hover
        {
            background: rgb(91, 91, 91);
        }

        ads--CDockAreaTitleBar > QToolButton:pressed
        {
            border: 1px solid palette(highlight);
            background: rgba(255, 196, 125, 127);
        }


        ads--CDockAreaTabBar
        {

        }


        ads--CDockAreaTabBar #tabsContainerWidget
        {
            background: rgb(81, 81, 81);
        }


        /*===========================================================================
         * QtAdvancedDockingSystem - Dock Widget Tab
         *===========================================================================*/
        ads--CDockWidgetTab
        {
            padding: 0 0px;
        }

        ads--CDockWidgetTab[activeTab="false"]
        {
            background: rgb(81, 81, 81);
            border-right: 1px solid rgb(51, 51, 51);
        }


        ads--CDockWidgetTab[activeTab="true"]
        {
            background: palette(highlight);
        }

        ads--CDockWidgetTab > QLabel
        {

        }


        ads--CDockWidgetTab[activeTab="true"]:disabled
        {
            background: rgb(140, 140, 140);
        }

        ads--CDockWidgetTab[activeTab="true"] #dockWidgetTabLabel
        {
            color: rgb(51, 51, 51);
        }

        #tabCloseButton
        {
            margin-top: 2px;
            background: none;
            border: none;
            padding: 2px 0px;
            qproperty-iconSize: 11px;
        }

        #tabCloseButton:hover
        {
            border: 1px solid rgba(0, 0, 0, 32);
            background: rgba(0, 0, 0, 16);
        }

        #tabCloseButton:pressed
        {
            background: rgba(0, 0, 0, 32);
        }

        /*===========================================================================
         * QtAdvancedDockingSystem - Dock Widget
         *===========================================================================*/
        ads--CDockWidget > QToolBar
        {
            border-bottom: 1px solid palette(light);
        }

        ads--CDockWidget
        {
            border: 1px solid rgb(81, 81, 81);
            border-top: none;
            background: rgb(51, 51, 51);
        }

        /**
         * The border comes from the itemview that is inside of the dock widget so we
         * we can disable the border
         */
        ads--CDockWidget[ItemViewContainer="true"]
        {
            border-left: none;
            border-right: none;
            border-bottom: none;
        }


        ads--CDockWidget[ItemViewContainer="true"] QtLabb--CHeaderWidget
        {
            border-left: 1px solid rgb(81, 81, 81);
            border-right: 1px solid rgb(81, 81, 81);
        }


        ads--CDockWidget[ItemViewContainer="true"] QTableView
        {
            gridline-color: rgb(81, 81, 81);
            padding-left: -1px;
            padding-right: -1px;
            background-color: rgb(46, 46, 46);
        }


        ads--CDockWidget[ItemViewContainer="true"]  QHeaderView::section:horizontal:first
        {
             border-left: 1px solid rgb(81, 81, 81);
        }

        ads--CDockWidget[ItemViewContainer="true"]  QHeaderView::section:horizontal:last
        {
             border-right: 1px solid rgb(81, 81, 81);
        }


        QWidget[dockWidgetContent="true"]
        {
            border: none;
        }


        /*===========================================================================
         * QtAdvancedDockingSystem - Overlay
         *===========================================================================*/
        ads--CDockOverlayCross
        {
            qproperty-iconFrameColor: palette(highlight);
            qproperty-iconBackgroundColor: palette(base);
            qproperty-iconOverlayColor: palette(highlight);
            qproperty-iconArrowColor: rgb(180, 180, 180);
            qproperty-iconShadowColor: rgb(0, 0, 0);
        }
    )";
    m_DockManager->setStyleSheet(darkStyle);
}

void MainWindow::createContents()
{
    auto DockWidget = createWelcomePage();

    m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DockWidget);

    // auto DockWidget2 = createWelcomePage();
    // m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DockWidget2);
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

ads::CDockWidget *MainWindow::createMapboxPage()
{
    auto m = new MapboxWidget();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Maps");
    DockWidget->setWidget(m, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");
    return DockWidget;
}

void MainWindow::on_pushButton_4_clicked()
{
    auto DockWidget3 = createMapboxPage();
    m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DockWidget3);
}

