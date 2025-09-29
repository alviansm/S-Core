#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DockWidget.h"

#include "WelcomePage.h"
#include "Pages/TechnicalPage.h"
#include "Pages/DecisionSupportPage.h"
#include "Pages/VoyagePlanningPage.h"
#include "Pages/HistoryPage.h"
#include "Pages/SettingPage.h"
#include "Pages/DashboardPage.h"

#include "MapboxWidget.h"

#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui_MainWindow)
    , m_DockManager(nullptr)
    , m_availableDockArea(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("EEShip - EnergyEfficientShip");

    setupDockManager();
    createContents();
    createActions();

    // signal-slots

    // --- File ---
    connect(ui->actionOpen_Configuration, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionSave_Configuration, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionExport_Data, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::applicationExit);

    // --- Edit ---
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionUser_Management, &QAction::triggered, this, &MainWindow::previewFeature_clicked);

    // --- View ---
    connect(ui->actionSave_Layout, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionSwitch_Vessel, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionSwitch_Fleet, &QAction::triggered, this, &MainWindow::previewFeature_clicked);

    // --- Tools ---
    connect(ui->actionIO_Device, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionCalibrate_Sensor, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionSystem_Health_Check, &QAction::triggered, this, &MainWindow::previewFeature_clicked);
    connect(ui->actionSecurity_Settings, &QAction::triggered, this, &MainWindow::previewFeature_clicked);

    // --- Help ---
    connect(ui->actionUser_Manual, &QAction::triggered, this, &MainWindow::applicationUserManual);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::applicationAbout);
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
            color: white;  /* Added for white text/icons */
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
            color: white;  /* Added for white text */
        }

        ads--CDockWidgetTab[activeTab="true"]:disabled
        {
            background: rgb(140, 140, 140);
        }

        ads--CDockWidgetTab[activeTab="true"] #dockWidgetTabLabel
        {
            color: white;  /* Changed from rgb(51, 51, 51) to white */
        }

        ads--CDockWidgetTab[activeTab="false"] #dockWidgetTabLabel
        {
            color: white;  /* Added for inactive tab white text */
        }

        #tabCloseButton
        {
            margin-top: 2px;
            background: none;
            border: none;
            padding: 2px 0px;
            qproperty-iconSize: 11px;
            color: white;  /* Added for white close button */
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
            color: white;  /* Added for white toolbar text */
        }

        ads--CDockWidget > QToolBar > QToolButton
        {
            color: white;  /* Added for white toolbar button text */
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
    m_availableDockArea = m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DockWidget);
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

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Dashboard");
    DockWidget->setWidget(m, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");
    return DockWidget;
}

ads::CDockWidget *MainWindow::createDashboardPage()
{
    auto m = new DashboardPage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Dashboard");
    DockWidget->setWidget(m, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");

    // --- Set Toolbar ---
    QToolBar *toolbar = new QToolBar();
    toolbar->setMovable(false);

    // Dark theme stylesheet for toolbar
    toolbar->setStyleSheet(R"(
        QToolBar {
            background-color: #2b2b2b;
            border: 1px solid #3c3c3c;
            spacing: 6px;
            padding: 4px;
        }

        QToolBar::separator {
            background-color: #555555;
            width: 1px;
            margin: 2px;
        }

        QLabel {
            color: #ffffff;
            font-size: 12px;
            padding: 4px 8px;
            background: transparent;
        }
    )");

    QComboBox *styleSelector = new QComboBox(toolbar);
    styleSelector->addItem("Dark");
    styleSelector->addItem("Light");
    styleSelector->addItem("Balanced");

    // Dark theme stylesheet for combobox
    styleSelector->setStyleSheet(R"(
        QComboBox {
            background-color: #404040;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 4px 8px;
            color: #ffffff;
            font-size: 12px;
            min-width: 80px;
        }

        QComboBox:hover {
            background-color: #4a4a4a;
            border-color: #777777;
        }

        QComboBox:pressed {
            background-color: #363636;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(:/icons/general/ic-down_arrow.png);
            width: 10px;
            height: 10px;
        }

        QComboBox QAbstractItemView {
            background-color: #404040;
            border: 1px solid #555555;
            selection-background-color: #5a5a5a;
            color: #ffffff;
        }

        QComboBox QAbstractItemView::item {
            padding: 6px 12px;
            border: none;
        }

        QComboBox QAbstractItemView::item:selected {
            background-color: #5a5a5a;
        }

        QComboBox QAbstractItemView::item:hover {
            background-color: #4a4a4a;
        }
    )");

    QLabel *label = new QLabel("Map Theme:");
    toolbar->addWidget(label);
    toolbar->addWidget(styleSelector);

    // --- Toolbar Actions: Map Feature ---
    toolbar->addSeparator();

    // --- Map Feature Toggle ---

    // -- Weather --
    QAction* WeatherToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/weather-weather.png",
        "Toggle Show/Hide Weather on Map"
    );
    // WeatherToggleAction->setCheckable(true);

    // -- Bathymeter --
    QAction* BathymeterToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/sonar.png",
        "Toggle Show/Hide Bathymeter on Map"
    );
    // BathymeterToggleAction->setCheckable(true);

    // -- Ports --
    QAction* PortsToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/port.png",
        "Toggle Show/Hide Ports on Map"
    );
    // PortsToggleAction->setCheckable(true);

    // -- Ocean Route --
    QAction* OceanRouteToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/route.png",
        "Toggle Show/Hide Ocean Route on Map"
    );
    // OceanRouteToggleAction->setCheckable(true);

    // -- Emission Control Area Zone --
    QAction* ECAToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/emission.png",
        "Toggle Show/Hide Emission Control Area (ECA) on Map"
    );
    // ECAToggleAction->setCheckable(true);

    // -- Piracy Area --
    QAction* PiracyZoneToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/security.png",
        "Toggle Show/Hide Piracy Zone on Map"
    );
    // PiracyZoneToggleAction->setCheckable(true);

    // -- Planned Route --
    QAction* PlannedRouteToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/planned-route.png",
        "Toggle Show/Hide Planned Route on Map"
    );
    // PlannedRouteToggleAction->setCheckable(true);

    // -- Actual Route --
    QAction* ActualRouteToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/actual-route.png",
        "Toggle Show/Hide Actual Route on Map"
    );
    // ActualRouteToggleAction->setCheckable(true);

    // --- Toolbar Actions: Frame + Summary Dashboards ---
    toolbar->addSeparator();

    QAction* AlertAndRecomendationFrameAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/notification.png",
        "Notification"
    );
    AlertAndRecomendationFrameAction->setCheckable(true);

    QAction* KPIOverviewFrameAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/kpi.png",
        "KPI Overview"
    );
    KPIOverviewFrameAction->setCheckable(true);

    QAction* WeatherFrameAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/weather.png",
        "Weather Info"
    );
    WeatherFrameAction->setCheckable(true);

    // --- Toolbar Actions: Map View ---
    toolbar->addSeparator();

    // -- Refresh Map --
    QAction* RefreshMapToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/load.png",
        "Refresh Map"
    );

    // -- Return to Original View --
    QAction* ReturnToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/return.png",
        "Return to Original Position on Map"
    );

    // --- Toolbar Action: Map 2D/3D selection ---
    toolbar->addSeparator();

    // -- 3D Map --
    QAction* ThreeDimensionMapToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/earth.png",
        "Change to 3D View Map"
    );
    ThreeDimensionMapToggleAction->setCheckable(true);

    // -- 2D Map --
    QAction* TwoDimensionMapToggleAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/maps.png",
        "Change to 2D View Map"
    );
    TwoDimensionMapToggleAction->setCheckable(true);

    // --- Toolbars: Signal-Slot Connections ---
    connect(styleSelector, &QComboBox::currentIndexChanged, m, &DashboardPage::setCurrentMapStyle);

    connect(WeatherToggleAction, &QAction::triggered, m, &DashboardPage::WeatherToggleAction_toggled);
    connect(BathymeterToggleAction, &QAction::triggered, m, &DashboardPage::BathymeterTiggkeAction_toggled);
    connect(PortsToggleAction, &QAction::triggered, m, &DashboardPage::PortsToggleAction_toggled);
    connect(OceanRouteToggleAction, &QAction::triggered, m, &DashboardPage::OceanRouteToggleAction_toggled);
    connect(ECAToggleAction, &QAction::triggered, m, &DashboardPage::ECAToggleAction_toggled);
    connect(PiracyZoneToggleAction, &QAction::triggered, m, &DashboardPage::PiracyZoneToggleAction_toggled);
    connect(PlannedRouteToggleAction, &QAction::triggered, m, &DashboardPage::PlannedRouteToggleAction_toggled);
    connect(ActualRouteToggleAction, &QAction::triggered, m, &DashboardPage::ActualRouteToggleAction_toggled);
    connect(AlertAndRecomendationFrameAction, &QAction::triggered, m, &DashboardPage::AlretAndRecomendationFrame_toggled);
    connect(KPIOverviewFrameAction, &QAction::triggered, m, &DashboardPage::KPIOverviewFrame_toggled);
    connect(WeatherFrameAction, &QAction::triggered, m, &DashboardPage::WeatherFrame_toggled);
    connect(RefreshMapToggleAction, &QAction::triggered, m, &DashboardPage::RefreshMapToggleAction_toggled);
    connect(ReturnToggleAction, &QAction::triggered, m, &DashboardPage::ReturnToggleAction_toggled);
    connect(ThreeDimensionMapToggleAction, &QAction::triggered, m, &DashboardPage::ThreeDimensionMapToggleAction);
    connect(TwoDimensionMapToggleAction, &QAction::triggered, m, &DashboardPage::TwoDimensionMapToggleAction);

    toolbar->setIconSize(QSize(16, 16)); // NOTE: Seems to not working
    DockWidget->setToolBar(toolbar);

    return DockWidget;
}

ads::CDockWidget *MainWindow::createTechnicalPage()
{
    auto w = new TechnicalPage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Technical");
    DockWidget->setWidget(w, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");

    // --- Set Toolbar ---
    QToolBar *toolbar = new QToolBar();
    toolbar->setMovable(false);

    // Dark theme stylesheet for toolbar
    toolbar->setStyleSheet(R"(
        QToolBar {
            background-color: #2b2b2b;
            border: 1px solid #3c3c3c;
            spacing: 6px;
            padding: 4px;
        }

        QToolBar::separator {
            background-color: #555555;
            width: 1px;
            margin: 2px;
        }

        QLabel {
            color: #ffffff;
            font-size: 12px;
            padding: 4px 8px;
            background: transparent;
        }
    )");

    QComboBox *pageSelector = new QComboBox(toolbar);
    pageSelector->addItem("Propulsion System");
    pageSelector->addItem("Electrical System");
    pageSelector->addItem("Fuel Management System");
    pageSelector->addItem("Ballast System");
    pageSelector->addItem("Hotel Load");

    // Dark theme stylesheet for combobox
    pageSelector->setStyleSheet(R"(
        QComboBox {
            background-color: #404040;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 4px 8px;
            color: #ffffff;
            font-size: 12px;
            min-width: 80px;
        }

        QComboBox:hover {
            background-color: #4a4a4a;
            border-color: #777777;
        }

        QComboBox:pressed {
            background-color: #363636;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(:/icons/general/ic-down_arrow.png);
            width: 10px;
            height: 10px;
        }

        QComboBox QAbstractItemView {
            background-color: #404040;
            border: 1px solid #555555;
            selection-background-color: #5a5a5a;
            color: #ffffff;
        }

        QComboBox QAbstractItemView::item {
            padding: 6px 12px;
            border: none;
        }

        QComboBox QAbstractItemView::item:selected {
            background-color: #5a5a5a;
        }

        QComboBox QAbstractItemView::item:hover {
            background-color: #4a4a4a;
        }
    )");

    QLabel *label = new QLabel("Select Page:");
    toolbar->addWidget(label);
    toolbar->addWidget(pageSelector);

    // --- Toolbar Actions ---
    toolbar->addSeparator();

    // -- IO Device Manager --

    // IO Device
    QAction* IODeviceAction = createToolbarAction(
        toolbar,
        ":/icons/toolbar_technical/ic-io_devices.png",
        "IO Device"
    );

    // Load Device Configuration
    QAction* LoadDeviceConfigAction = createToolbarAction(
        toolbar,
        ":/icons/toolbar_technical/ic-device_load.png",
        "Load Device Configuration"
    );

    // Export Device Configuration
    QAction* ExportDeviceConfigAction = createToolbarAction(
        toolbar,
        ":/icons/toolbar_technical/ic-device_save.png",
        "Export Device Configuration"
    );

    // -- Load System Configuration --
    toolbar->addSeparator();

    // Load Propulsion System Configuration
    QAction* LoadPropulsionSysConfigAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/load-propulsion.png",
        "Load Propulsion System Configuration"
    );

    // Load Electrical System Configuration
    QAction* LoadElectricalSysConfigAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/load-electrical.png",
        "Load Electrical System Configuration"
    );

    // Load Fuel Management System Configuration
    QAction* LoadFuelManagementSysConfigAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/fuel-management.png",
        "Load Fuel Management System Configuration"
    );

    // Load Ballast System Configuration
    QAction* LoadBallastSysConfigAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/ballast.png",
        "Load Ballast System Configuration"
    );

    // Load Hotel Load Configuration
    QAction* LoadHotelLoadConfigAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/hotel-load.png",
        "Load Hotel Load Configuration"
    );

    // -- Technical Page Layouting --
    toolbar->addSeparator();

    // Toggle Hide/Show Top Sidebar
    QAction* ToggleTopSidebarAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/topbar.png",
        "Toggle Show/Hide Top Sidebar"
    );
    ToggleTopSidebarAction->setCheckable(true);
    ToggleTopSidebarAction->setChecked(true);

    // Toggle Hide/Show Main Sidebar
    // QAction* ToggleMainSidebarAction = createToolbarAction(
    //     toolbar,
    //     ":/icons/ribbon/sidebar.png",
    //     "Toggle Show/Hide Main Sidebar"
    // );
    // ToggleMainSidebarAction->setCheckable(true);

    // Toggle Hide/Show Top Content
    QAction* ToggleTopContentAction = createToolbarAction(
        toolbar,
        ":/icons/ribbon/main-top.png",
        "Toggle Show/Hide Top Content"
    );
    ToggleTopContentAction->setCheckable(true);
    ToggleTopContentAction->setChecked(true);

    // --- Toolbars: Signal-Slot Connections ---
    connect(pageSelector, &QComboBox::currentIndexChanged, w, &TechnicalPage::setCurrentPage);

    connect(IODeviceAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(LoadDeviceConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(ExportDeviceConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(LoadPropulsionSysConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(LoadElectricalSysConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(LoadFuelManagementSysConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(LoadBallastSysConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(LoadHotelLoadConfigAction, &QAction::toggled, w, &TechnicalPage::IODevice_toggled);
    connect(ToggleTopSidebarAction, &QAction::toggled, w, &TechnicalPage::toggleHideShow_topSidebar);
    // connect(ToggleMainSidebarAction, &QAction::toggled, w, &TechnicalPage::toggleHideShow_mainSidebar);
    connect(ToggleTopContentAction, &QAction::toggled, w, &TechnicalPage::toggleHideShow_topContent);

    toolbar->setIconSize(QSize(16, 16)); // NOTE: Seems to not working
    DockWidget->setToolBar(toolbar);

    return DockWidget;
}

ads::CDockWidget *MainWindow::createDecisionSupportPage()
{
    auto w = new DecisionSupportPage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Decision Support");
    DockWidget->setWidget(w, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");

    // --- Set Toolbar ---
    QToolBar *toolbar = new QToolBar();
    toolbar->setMovable(false);

    // Dark theme stylesheet for toolbar
    toolbar->setStyleSheet(R"(
        QToolBar {
            background-color: #2b2b2b;
            border: 1px solid #3c3c3c;
            spacing: 6px;
            padding: 4px;
        }

        QToolBar::separator {
            background-color: #555555;
            width: 1px;
            margin: 2px;
        }

        QLabel {
            color: #ffffff;
            font-size: 12px;
            padding: 4px 8px;
            background: transparent;
        }
    )");

    QComboBox *pageSelector = new QComboBox(toolbar);
    pageSelector->addItem("CII Performance");
    pageSelector->addItem("3-Year Implementation Plan Summary");
    pageSelector->addItem("Self-Evaluation Feedback Loop");
    pageSelector->addItem("Corrective Action Plan");

    // Dark theme stylesheet for combobox
    pageSelector->setStyleSheet(R"(
        QComboBox {
            background-color: #404040;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 4px 8px;
            color: #ffffff;
            font-size: 12px;
            min-width: 80px;
        }

        QComboBox:hover {
            background-color: #4a4a4a;
            border-color: #777777;
        }

        QComboBox:pressed {
            background-color: #363636;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(:/icons/general/ic-down_arrow.png);
            width: 10px;
            height: 10px;
        }

        QComboBox QAbstractItemView {
            background-color: #404040;
            border: 1px solid #555555;
            selection-background-color: #5a5a5a;
            color: #ffffff;
        }

        QComboBox QAbstractItemView::item {
            padding: 6px 12px;
            border: none;
        }

        QComboBox QAbstractItemView::item:selected {
            background-color: #5a5a5a;
        }

        QComboBox QAbstractItemView::item:hover {
            background-color: #4a4a4a;
        }
    )");

    QLabel *label = new QLabel("Page:");
    toolbar->addWidget(label);
    toolbar->addWidget(pageSelector);

    // --- Toolbars: Signal-Slot Connections ---
    connect(pageSelector, &QComboBox::currentIndexChanged, w, &DecisionSupportPage::setCurrentPage);

    toolbar->setIconSize(QSize(16, 16)); // NOTE: Seems to not working
    DockWidget->setToolBar(toolbar);

    return DockWidget;
}

ads::CDockWidget *MainWindow::createVoyagePlanningPage()
{
    auto w = new VoyagePlanningPage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Voyage Planning");
    DockWidget->setWidget(w, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");
    return DockWidget;
}

ads::CDockWidget *MainWindow::createHistoryPage()
{
    auto w = new HistoryPage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("History");
    DockWidget->setWidget(w, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");
    return DockWidget;
}

ads::CDockWidget *MainWindow::createSettingPage()
{
    auto w = new SettingPage();

    ads::CDockWidget* DockWidget = m_DockManager->createDockWidget("Setting");
    DockWidget->setWidget(w, ads::CDockWidget::ForceNoScrollArea);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
    DockWidget->setStyleSheet("ads--CDockWidget::pane { background-color: #2b2b2b; }");
    return DockWidget;
}

QAction *MainWindow::createToolbarAction(QToolBar *toolbar, const QString &iconPath, const QString &text, QObject *parent)
{
    QAction* action = new QAction(parent);
    action->setIcon(QIcon(iconPath));
    action->setText(text);
    toolbar->addAction(action);
    return action;
}

void MainWindow::createActions()
{
    ui->toolBar->setIconSize(QSize(24, 24));

    m_pushButtonWelcomePage = new QPushButton();
    m_pushButtonWelcomePage->setIcon(QIcon(":/icons/ribbon/manual.png"));
    m_pushButtonWelcomePage->setIconSize(QSize(24, 24));
    m_pushButtonWelcomePage->setToolTip("Welcome Page");
    m_pushButtonWelcomePage->setFlat(true);
    m_pushButtonWelcomePage->setStyleSheet("QPushButton { padding: 0px; border: none; }");
    m_pushButtonWelcomePage->setCheckable(true);
    m_pushButtonWelcomePage->setChecked(true);
    ui->toolBar->addWidget(m_pushButtonWelcomePage);
    connect(m_pushButtonWelcomePage, &QPushButton::toggled, this, &MainWindow::pushButton_WelcomePage_clicked);

    ui->toolBar->addSeparator();

    QAction* savePerspective = new QAction();
    savePerspective->setIcon(QIcon(":/icons/ribbon/save.png"));
    savePerspective->setText("Save Perspective");
    ui->toolBar->addAction(savePerspective);

    connect(savePerspective, &QAction::triggered, this, &MainWindow::previewFeature_clicked);

    QAction* loadPerspective = new QAction();
    loadPerspective->setIcon(QIcon(":/icons/ribbon/load-perspective.png"));
    loadPerspective->setText("Load Perspective");
    ui->toolBar->addAction(loadPerspective);

    connect(loadPerspective, &QAction::triggered, this, &MainWindow::previewFeature_clicked);

    // Add spacer to push SDG buttons to the right
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->toolBar->addWidget(spacer);

    // SDG 13 - Climate Action
    QToolButton* sdg13Btn = new QToolButton();
    sdg13Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-13.png"));
    sdg13Btn->setToolTip("SDG 13 - Climate Action");
    sdg13Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg13Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 13 - Climate Action",
            "Take urgent action to combat climate change and its impacts\n\n"
            "Directly addressed: reducing CO₂ and other GHG emissions from shipping. "
            "Aligns with IMO Initial and Revised GHG Strategies.");
    });
    ui->toolBar->addWidget(sdg13Btn);

    // SDG 14 - Life Below Water
    QToolButton* sdg14Btn = new QToolButton();
    sdg14Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-14.png"));
    sdg14Btn->setToolTip("SDG 14 - Life Below Water");
    sdg14Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg14Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 14 - Life Below Water",
            "Conserve and sustainably use the oceans, seas and marine resources\n\n"
            "Less air pollution reduces acidification, deposition of harmful pollutants, "
            "and supports healthier marine ecosystems.");
    });
    ui->toolBar->addWidget(sdg14Btn);

    // SDG 3 - Good Health and Well-being
    QToolButton* sdg3Btn = new QToolButton();
    sdg3Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-03.png"));
    sdg3Btn->setToolTip("SDG 3 - Good Health and Well-being");
    sdg3Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg3Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 3 - Good Health and Well-being",
            "Ensure healthy lives and promote well-being for all\n\n"
            "Cutting SOx, NOx, and particulate matter improves coastal air quality, "
            "reducing respiratory and cardiovascular disease.");
    });
    ui->toolBar->addWidget(sdg3Btn);

    // SDG 7 - Affordable and Clean Energy
    QToolButton* sdg7Btn = new QToolButton();
    sdg7Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-07.png"));
    sdg7Btn->setToolTip("SDG 7 - Affordable and Clean Energy");
    sdg7Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg7Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 7 - Affordable and Clean Energy",
            "Ensure access to affordable, reliable, sustainable and modern energy\n\n"
            "Drives uptake of cleaner fuels (LNG, methanol, ammonia, biofuels) "
            "and energy efficiency tech in shipping.");
    });
    ui->toolBar->addWidget(sdg7Btn);

    // SDG 9 - Industry, Innovation, and Infrastructure
    QToolButton* sdg9Btn = new QToolButton();
    sdg9Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-09.png"));
    sdg9Btn->setToolTip("SDG 9 - Industry, Innovation, and Infrastructure");
    sdg9Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg9Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 9 - Industry, Innovation, and Infrastructure",
            "Build resilient infrastructure, promote sustainable industrialization and foster innovation\n\n"
            "Encourages innovation in ship design, propulsion systems, and port infrastructure "
            "to support low-carbon fuels.");
    });
    ui->toolBar->addWidget(sdg9Btn);

    // SDG 12 - Responsible Consumption and Production
    QToolButton* sdg12Btn = new QToolButton();
    sdg12Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-12.png"));
    sdg12Btn->setToolTip("SDG 12 - Responsible Consumption and Production");
    sdg12Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg12Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 12 - Responsible Consumption and Production",
            "Ensure sustainable consumption and production patterns\n\n"
            "Promotes resource-efficient operations and life-cycle thinking in shipping, "
            "including recycling of ships and equipment.");
    });
    ui->toolBar->addWidget(sdg12Btn);

    // SDG 17 - Partnerships for the Goals
    QToolButton* sdg17Btn = new QToolButton();
    sdg17Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-17.png"));
    sdg17Btn->setToolTip("SDG 17 - Partnerships for the Goals");
    sdg17Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg17Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 17 - Partnerships for the Goals",
            "Strengthen the means of implementation and revitalize the global partnership for sustainable development\n\n"
            "Global regulatory frameworks (IMO, MEPC, industry coalitions) are international partnerships "
            "to reduce shipping emissions.");
    });
    ui->toolBar->addWidget(sdg17Btn);

    // SDG 8 - Decent Work and Economic Growth
    QToolButton* sdg8Btn = new QToolButton();
    sdg8Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-08.png"));
    sdg8Btn->setToolTip("SDG 8 - Decent Work and Economic Growth");
    sdg8Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg8Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 8 - Decent Work and Economic Growth",
            "Promote sustained, inclusive and sustainable economic growth, full and productive employment and decent work for all\n\n"
            "Transition to green shipping can create green jobs and sustainable maritime industries.");
    });
    ui->toolBar->addWidget(sdg8Btn);

    // SDG 11 - Sustainable Cities and Communities
    QToolButton* sdg11Btn = new QToolButton();
    sdg11Btn->setIcon(QIcon(":/icons/sdgs/E-WEB-Goal-11.png"));
    sdg11Btn->setToolTip("SDG 11 - Sustainable Cities and Communities");
    sdg11Btn->setStyleSheet("QToolButton { border: none; }");
    connect(sdg11Btn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "SDG 11 - Sustainable Cities and Communities",
            "Make cities and human settlements inclusive, safe, resilient and sustainable\n\n"
            "Better air quality in port cities from reduced SOx/NOx and particulates.");
    });
    ui->toolBar->addWidget(sdg11Btn);
}

void MainWindow::on_pushButton_DashboardPage_clicked(bool checked)
{
    addDockWidgetWithDockManager(ui->pushButton_DashboardPage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::Dashboard);
}


void MainWindow::on_pushButton_TechnicalPage_clicked(bool checked)
{
    addDockWidgetWithDockManager(ui->pushButton_TechnicalPage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::Technical);
}

void MainWindow::on_pushButton_DecisionSupportPage_clicked(bool checked)
{
    addDockWidgetWithDockManager(ui->pushButton_DecisionSupportPage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::DecisionSupport);
}


void MainWindow::on_pushButton_RoutePlanningPage_clicked(bool checked)
{
    addDockWidgetWithDockManager(ui->pushButton_RoutePlanningPage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::VoyagePlanning);
}


void MainWindow::on_pushButton_HistoryPage_clicked(bool checked)
{
    addDockWidgetWithDockManager(ui->pushButton_HistoryPage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::History);
}


void MainWindow::on_pushButton_SettingPage_clicked(bool checked)
{
    addDockWidgetWithDockManager(ui->pushButton_SettingPage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::Setting);
}

void MainWindow::pushButton_WelcomePage_clicked(bool checked)
{
    qInfo() << "Welcome Page button clicked, checked:" << checked;
    addDockWidgetWithDockManager(m_pushButtonWelcomePage, checked, ads::DockWidgetArea::CenterDockWidgetArea, MainWindow::Page::Welcome);
}

void MainWindow::previewFeature_clicked(bool checked)
{
    QMessageBox::information(this, "Preview", "This button is a preview-only");
}

void MainWindow::applicationExit(bool checked)
{
    // Confirm dialog before exit
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Application", "Are you sure you want to exit the application?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    } else {
        // Do nothing if No is selected
    }
}

void MainWindow::applicationUserManual(bool checked)
{
    // Path inside qrc
    QString resourcePath = ":/reports/manual.pdf";

    // Open resource as QFile
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open embedded PDF:" << resourcePath;
        return;
    }

    // Create temporary file on disk with proper template
    QTemporaryFile tmpFile(QDir::tempPath() + "/userManual_XXXXXX.pdf");
    tmpFile.setAutoRemove(false); // keep file after program exits
    if (!tmpFile.open()) {
        qDebug() << "Failed to create temporary PDF file";
        return;
    }

    // Write the PDF data to the temporary file
    tmpFile.write(file.readAll());
    tmpFile.flush();

    QString tempPath = tmpFile.fileName();
    tmpFile.close(); // Close before opening with external viewer

    // Open using the default system PDF viewer
    QDesktopServices::openUrl(QUrl::fromLocalFile(tempPath));
}

void MainWindow::applicationAbout(bool checked)
{
    // About dialog via QMessageBox
    QMessageBox::about(this, "About EnergyEfficientShip",
                       "<b>Vessel Performance Monitoring and Decision Support System</b><br>"
                       "Version Prototype (0.0.1)<br><br>"
                       "Developed by:<br>"
                       "Tim A<br><br>"
                       "© 2025 Nautipro Connect Solutions. All rights reserved.");
}

void MainWindow::addDockWidgetWithDockManager(QPushButton *pushButtonSource, bool checked, ads::DockWidgetArea area, MainWindow::Page page)
{
    if (pushButtonSource == nullptr) return;

    if (checked)
    {
        ads::CDockWidget* DockWidget = nullptr;
        switch (page) {
            case MainWindow::Page::Dashboard:
                DockWidget = createDashboardPage();
                DockWidget->setObjectName("DashboardPage");
                break;
            case MainWindow::Page::Technical:
                DockWidget = createTechnicalPage();
                DockWidget->setObjectName("TechnicalPage");
                break;
            case MainWindow::Page::DecisionSupport:
                DockWidget = createDecisionSupportPage();
                DockWidget->setObjectName("DecisionSupportPage");
                break;
            case MainWindow::Page::VoyagePlanning:
                DockWidget = createVoyagePlanningPage();
                DockWidget->setObjectName("VoyagePlanningPage");
                break;
            case MainWindow::Page::History:
                DockWidget = createHistoryPage();
                DockWidget->setObjectName("HistoryPage");
                break;
            case MainWindow::Page::Setting:
                DockWidget = createSettingPage();
                DockWidget->setObjectName("SettingPage");
                break;
            case MainWindow::Page::Welcome:
                DockWidget = createWelcomePage();
                DockWidget->setObjectName("WelcomePage");
                break;
            default:
                break;
        }

        if (!DockWidget)
            return;

        if (m_availableDockArea == nullptr)
        {
            auto DockAreaWidget = m_DockManager->addDockWidget(area, DockWidget);
            m_availableDockArea = DockAreaWidget;
        }
        else
        {
            m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DockWidget, m_availableDockArea);
        }

        // Connect signal-slots when closed
        connect(DockWidget, &ads::CDockWidget::closed, this, [pushButtonSource]() {
            pushButtonSource->setChecked(false);
        });
    }
    else
    {
        QString dockToRemoveObjectName = "";
        ads::CDockWidget* dockToRemove = nullptr;
        switch (page) {
            case MainWindow::Page::Dashboard:
                dockToRemoveObjectName = "DashboardPage";
                break;
            case MainWindow::Page::Technical:
                dockToRemoveObjectName = "TechnicalPage";
                break;
            case MainWindow::Page::DecisionSupport:
                dockToRemoveObjectName = "DecisionSupportPage";
                break;
            case MainWindow::Page::VoyagePlanning:
                dockToRemoveObjectName = "VoyagePlanningPage";
                break;
            case MainWindow::Page::History:
                dockToRemoveObjectName = "HistoryPage";
                break;
            case MainWindow::Page::Setting:
                dockToRemoveObjectName = "SettingPage";
                break;
            case MainWindow::Page::Welcome:
                dockToRemoveObjectName = "WelcomePage";
                break;
            default:
                break;
        }

        QMap<QString, ads::CDockWidget*> map = m_DockManager->dockWidgetsMap();
        for (auto it = map.constBegin(); it != map.constEnd(); ++it)
        {
            if (it.key() == dockToRemoveObjectName)
            {
                dockToRemove = it.value();
                break;
            }
        }

        if (dockToRemove != nullptr)
            m_DockManager->removeDockWidget(dockToRemove);

        if (map.size()<1)
            m_availableDockArea = nullptr;
    }
}

