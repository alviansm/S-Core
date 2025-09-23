#include "TechnicalPage.h"
#include "ui_TechnicalPage.h"

#include <QSizePolicy>
#include <QLabel>

#include "CircleProgressBar.h"

TechnicalPage::TechnicalPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TechnicalPage)
    , m_splitter(nullptr)
    , m_sidebarContainer(nullptr)
    , m_topSidebar(nullptr)
    , m_mainSidebar(nullptr)
    , m_contentContainer(nullptr)
    , m_topContent(nullptr)
    , m_mainContent(nullptr)
    , m_currentPage(PropulsionSystemPage)
{
    ui->setupUi(this);

    setupWidget();
    createPageContent();
    setupStylesheet();

    // Set initial page
    setCurrentPage(PropulsionSystemPage);
}

TechnicalPage::~TechnicalPage()
{
    delete ui;
}

void TechnicalPage::PageSelector_currentIndex_changed(int index)
{
    qInfo() << "PageSelector_currentIndex_changed called with index:" << index;
    setCurrentPage(index);
}

void TechnicalPage::IODevice_toggled(bool checked)
{
    qInfo() << "IODevice toggled:" << checked;
}

void TechnicalPage::setCurrentPage(int pageIndex)
{
    if (pageIndex >= 0 && pageIndex < 5) // We have 5 pages
    {
        m_currentPage = pageIndex;

        // Update all stacked widgets to show the same page index
        m_topSidebar->setCurrentIndex(pageIndex);
        m_mainSidebar->setCurrentIndex(pageIndex);
        m_topContent->setCurrentIndex(pageIndex);
        m_mainContent->setCurrentIndex(pageIndex);

        qInfo() << "Current page changed to:" << pageIndex;
    }
}

void TechnicalPage::setupWidget()
{
    // --- Splitter ---
    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(Qt::Horizontal);

    // --- Sidebar container ---
    QWidget *sidebarWidget = new QWidget(m_splitter);
    m_sidebarContainer = new QVBoxLayout(sidebarWidget);
    m_sidebarContainer->setContentsMargins(0, 0, 0, 0);
    m_sidebarContainer->setSpacing(6);

    // Create stacked widgets instead of regular widgets
    m_topSidebar = new QStackedWidget(sidebarWidget);
    m_mainSidebar = new QStackedWidget(sidebarWidget);

    // Ratio 1:3 vertically in the sidebar
    m_sidebarContainer->addWidget(m_topSidebar, 1);  // stretch factor 1
    m_sidebarContainer->addWidget(m_mainSidebar, 3); // stretch factor 3

    // --- Content container ---
    QWidget *contentWidget = new QWidget(m_splitter);
    m_contentContainer = new QVBoxLayout(contentWidget);
    m_contentContainer->setContentsMargins(0, 0, 0, 0);
    m_contentContainer->setSpacing(6);

    // Create stacked widgets instead of regular widgets
    m_topContent = new QStackedWidget(contentWidget);
    m_mainContent = new QStackedWidget(contentWidget);

    // Ratio 1:3 vertically in the content
    m_contentContainer->addWidget(m_topContent, 1);  // stretch factor 1
    m_contentContainer->addWidget(m_mainContent, 3); // stretch factor 3

    // --- Add to splitter ---
    // Sidebar first, content second
    m_splitter->addWidget(sidebarWidget);
    m_splitter->addWidget(contentWidget);

    // --- Sidebar sizing rules ---
    sidebarWidget->setMinimumWidth(240);
    sidebarWidget->setMaximumWidth(280);

    // Allow splitter to resize content dynamically
    QList<int> sizes;
    sizes << 260 << 700; // initial sizes, sidebar ~260px
    m_splitter->setSizes(sizes);

    // --- Top-level layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_splitter);
    this->setLayout(mainLayout);
}

void TechnicalPage::createPageContent()
{
    // Create content for each page using switch case
    for (int i = PropulsionSystemPage; i <= HotelLoadPage; ++i)
    {
        createPageWidgets(i, "");
    }
}

void TechnicalPage::createPageWidgets(int pageIndex, const QString& pageName)
{
    Q_UNUSED(pageName); // We're not using this parameter anymore

    switch (pageIndex)
    {
        case PropulsionSystemPage:
            createPropulsionSystemPageContent();
            break;
        case ElectricalSystemPage:
            createElectricalSystemPageContent();
            break;
        case FuelManagementSystemPage:
            createFuelManagementSystemPageContent();
            break;
        case BallastSystemPage:
            createBallastSystemPageContent();
            break;
        case HotelLoadPage:
            createHotelLoadSystemPageContent();
            break;
        default:
            break;
    }
}

// --- Propulsion System Page Contents ---
void TechnicalPage::createPropulsionSystemPageContent()
{
    createPropulsionSystemPageContent_topSidebar();
    createPropulsionSystemPageContent_mainSidebar();
    createPropulsionSystemPageContent_topContent();
    createPropulsionSystemPageContent_mainContent();
}

void TechnicalPage::createPropulsionSystemPageContent_topSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);

    // Top row with Summary label and Fuel consumption
    QWidget* topRowWidget = new QWidget();
    QHBoxLayout* topRowLayout = new QHBoxLayout(topRowWidget);
    topRowLayout->setContentsMargins(0, 0, 0, 0);

    // Keep the existing Summary label
    QLabel* label = new QLabel("Summary");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    topRowLayout->addWidget(label);

    topRowLayout->addStretch();

    // Fuel consumption on the right with tooltip
    QWidget* fuelWidget = new QWidget();
    fuelWidget->setContentsMargins(0, 6, 6, 0);
    QVBoxLayout* fuelLayout = new QVBoxLayout(fuelWidget);
    fuelLayout->setContentsMargins(0, 0, 0, 0);
    fuelLayout->setSpacing(2);

    QLabel* fuelRateValue = new QLabel("8.7 T/H");
    fuelRateValue->setStyleSheet("color: #3498DB; font-size: 12px; font-weight: bold;");
    fuelRateValue->setAlignment(Qt::AlignCenter);
    fuelLayout->addWidget(fuelRateValue);

    fuelWidget->setToolTip("Fuel Consumption Rate");
    topRowLayout->addWidget(fuelWidget);

    layout->addWidget(topRowWidget);

    // Add some spacing
    layout->addSpacing(10);

    // Circle widget with flanking power values
    QWidget* circleWidget = new QWidget();
    QHBoxLayout* circleLayout = new QHBoxLayout(circleWidget);
    circleLayout->setContentsMargins(0, 0, 0, 0);

    // Current power value on the left
    QWidget* currentPowerWidget = new QWidget();
    QVBoxLayout* currentPowerLayout = new QVBoxLayout(currentPowerWidget);
    currentPowerLayout->setContentsMargins(0, 0, 0, 0);
    currentPowerLayout->setSpacing(2);

    QLabel* currentPowerValue = new QLabel("32.5\nMW");
    currentPowerValue->setStyleSheet("color: #00BF63; font-size: 12px; font-weight: bold;");
    currentPowerValue->setAlignment(Qt::AlignCenter);
    currentPowerLayout->addWidget(currentPowerValue);

    currentPowerWidget->setToolTip("Current Power Usage");
    circleLayout->addWidget(currentPowerWidget);

    // Create the circular progress bar for power usage
    CircleProgressBar* powerProgressBar = new CircleProgressBar();
    powerProgressBar->setMaximum(100); // MCR percentage
    powerProgressBar->setValue(65); // Current power usage (example: 65% of MCR)
    powerProgressBar->setText("MCR %");

    // Set colors based on power usage level
    int currentPowerPercent = 65; // This should be dynamic based on your data
    if (currentPowerPercent <= 75) {
        // Green - Optimal efficiency range
        powerProgressBar->setStartColor(QColor(46, 204, 113));
        powerProgressBar->setEndColor(QColor(39, 174, 96));
    } else if (currentPowerPercent <= 90) {
        // Amber - High load/low efficiency
        powerProgressBar->setStartColor(QColor(241, 196, 15));
        powerProgressBar->setEndColor(QColor(230, 126, 34));
    } else {
        // Red - Overload/Critical deviation
        powerProgressBar->setStartColor(QColor(231, 76, 60));
        powerProgressBar->setEndColor(QColor(192, 57, 43));
    }

    circleLayout->addWidget(powerProgressBar);

    // Maximum power value on the right
    QWidget* maxPowerWidget = new QWidget();
    QVBoxLayout* maxPowerLayout = new QVBoxLayout(maxPowerWidget);
    maxPowerLayout->setContentsMargins(0, 0, 0, 0);
    maxPowerLayout->setSpacing(2);

    QLabel* maxPowerValue = new QLabel("50.0\nMW");
    maxPowerValue->setStyleSheet("color: #BDC3C7; font-size: 12px; font-weight: bold;");
    maxPowerValue->setAlignment(Qt::AlignCenter);
    maxPowerLayout->addWidget(maxPowerValue);

    maxPowerWidget->setToolTip("Maximum Continuous Rating");
    circleLayout->addWidget(maxPowerWidget);

    layout->addWidget(circleWidget);

    layout->addStretch();
    m_topSidebar->addWidget(page);
}

void TechnicalPage::createPropulsionSystemPageContent_mainSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Detailed Consumption");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainSidebar->addWidget(page);
}

void TechnicalPage::createPropulsionSystemPageContent_topContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Engine Performance Graph");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topContent->addWidget(page);
}

void TechnicalPage::createPropulsionSystemPageContent_mainContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("P&ID Diagram");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainContent->addWidget(page);
}

// --- Electrical System Page Content ---
void TechnicalPage::createElectricalSystemPageContent()
{
    createElectricalSystemPageContent_topSidebar();
    createElectricalSystemPageContent_mainSidebar();
    createElectricalSystemPageContent_topContent();
    createElectricalSystemPageContent_mainContent();
}

void TechnicalPage::createElectricalSystemPageContent_topSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Power Status");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topSidebar->addWidget(page);
}

void TechnicalPage::createElectricalSystemPageContent_mainSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Load Distribution");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainSidebar->addWidget(page);
}

void TechnicalPage::createElectricalSystemPageContent_topContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Voltage & Frequency Monitoring");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topContent->addWidget(page);
}

void TechnicalPage::createElectricalSystemPageContent_mainContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Electrical Single Line Diagram");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainContent->addWidget(page);
}

// --- Fuel Management System Page Content ---
void TechnicalPage::createFuelManagementSystemPageContent()
{
    createFuelManagementSystemPageContent_topSidebar();
    createFuelManagementSystemPageContent_mainSidebar();
    createFuelManagementSystemPageContent_topContent();
    createFuelManagementSystemPageContent_mainContent();
}

void TechnicalPage::createFuelManagementSystemPageContent_topSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Fuel Levels");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topSidebar->addWidget(page);
}

void TechnicalPage::createFuelManagementSystemPageContent_mainSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Transfer Operations");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainSidebar->addWidget(page);
}

void TechnicalPage::createFuelManagementSystemPageContent_topContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Consumption Rate Graph");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topContent->addWidget(page);
}

void TechnicalPage::createFuelManagementSystemPageContent_mainContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Fuel System P&ID");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainContent->addWidget(page);
}

// --- Ballast System Page Content ---
void TechnicalPage::createBallastSystemPageContent()
{
    createBallastSystemPageContent_topSidebar();
    createBallastSystemPageContent_mainSidebar();
    createBallastSystemPageContent_topContent();
    createBallastSystemPageContent_mainContent();
}

void TechnicalPage::createBallastSystemPageContent_topSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Tank Levels");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topSidebar->addWidget(page);
}

void TechnicalPage::createBallastSystemPageContent_mainSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Pump Operations");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainSidebar->addWidget(page);
}

void TechnicalPage::createBallastSystemPageContent_topContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Stability Parameters");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topContent->addWidget(page);
}

void TechnicalPage::createBallastSystemPageContent_mainContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Ballast System Layout");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainContent->addWidget(page);
}

// --- Hotel Load Page Content ---
void TechnicalPage::createHotelLoadSystemPageContent()
{
    createHotelLoadSystemPageContent_topSidebar();
    createHotelLoadSystemPageContent_mainSidebar();
    createHotelLoadSystemPageContent_topContent();
    createHotelLoadSystemPageContent_mainContent();
}

void TechnicalPage::createHotelLoadSystemPageContent_topSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Current Load");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topSidebar->addWidget(page);
}

void TechnicalPage::createHotelLoadSystemPageContent_mainSidebar()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Load Breakdown");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainSidebar->addWidget(page);
}

void TechnicalPage::createHotelLoadSystemPageContent_topContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Load Trend Analysis");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_topContent->addWidget(page);
}

void TechnicalPage::createHotelLoadSystemPageContent_mainContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 0, 0);
    QLabel* label = new QLabel("Hotel Load Distribution");
    label->setStyleSheet("color: white; font-weight: bold;");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);
    layout->addStretch();
    m_mainContent->addWidget(page);
}

void TechnicalPage::setupStylesheet()
{
    QString stackedWidgetStyleSheet = "QStackedWidget { background-color: #212121; }";

    m_topSidebar->setStyleSheet(stackedWidgetStyleSheet);
    m_mainSidebar->setStyleSheet(stackedWidgetStyleSheet);
    m_topContent->setStyleSheet(stackedWidgetStyleSheet);
    m_mainContent->setStyleSheet(stackedWidgetStyleSheet);

    m_splitter->setStyleSheet(
        "QSplitter::handle { background: transparent; }"
    );
}
