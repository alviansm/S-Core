#include "DecisionSupportPage.h"
#include "ui_DecisionSupportPage.h"

#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QMessageBox>

DecisionSupportPage::DecisionSupportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DecisionSupportPage)
    , m_splitter(nullptr)
    , m_sidebarContainer(nullptr)
    , m_topSidebar(nullptr)
    , m_mainSidebar(nullptr)
    , m_contentContainer(nullptr)
    , m_topContent(nullptr)
    , m_mainContent(nullptr)
    , m_currentPage(CIIPerformanceComparisonGraph)
{
    ui->setupUi(this);

    setupWidget();
    createPageContent();
    setupStylesheet();

    // Set initial page
    setCurrentPage(CIIPerformanceComparisonGraph);
}

DecisionSupportPage::~DecisionSupportPage()
{
    delete ui;
}

void DecisionSupportPage::setCurrentPage(int pageIndex)
{
    if (pageIndex >= 0 && pageIndex < 5) // We have 5 pages
    {
        m_currentPage = pageIndex;

        // Update all the main content stacked widget to show the same page index
        m_mainContent->setCurrentIndex(pageIndex);

        qInfo() << "Current page changed to:" << pageIndex;
    }
}

void DecisionSupportPage::createPageContent()
{
    createPageContent_topSidebar();
    createPageContent_mainSidebar();
    createPageContent_topContent();

    // Create all main content pages
    createMainContent_CIIPerformanceComparisonGraph();
    createMainContent_ThreeYearImplementationSummary();
    createMainContent_SelfEvaluationFeedbackLoop();
    createMainContent_CorrectiveActionPlan();
}

void DecisionSupportPage::createPageContent_topSidebar()
{
    QVBoxLayout *layout = new QVBoxLayout(m_topSidebar);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    // Title
    QLabel *titleLabel = new QLabel("CARBON INTENSITY MONITOR");
    titleLabel->setStyleSheet("QLabel { color: #FFFFFF; font-weight: bold; font-size: 12px; letter-spacing: 1px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Speedometer gauge
    CarbonIntensityGauge *gauge = new CarbonIntensityGauge();
    gauge->setValue(250); // Set initial value
    layout->addWidget(gauge, 0, Qt::AlignCenter);

    // Store reference for updates
    m_carbonIntensityGauge = gauge;

    // Additional info panel
    QFrame *infoFrame = new QFrame();
    infoFrame->setStyleSheet("QFrame { background-color: rgba(255, 255, 255, 0.1); border-radius: 6px; padding: 8px; }");

    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setSpacing(4);

    QLabel *statusLabel = new QLabel("Grid Status: Optimal");
    statusLabel->setStyleSheet("QLabel { color: #40A745; font-size: 10px; font-weight: bold; }");
    statusLabel->setAlignment(Qt::AlignCenter);

    QLabel *updateLabel = new QLabel("Last updated: 2 min ago");
    updateLabel->setStyleSheet("QLabel { color: #CCCCCC; font-size: 9px; }");
    updateLabel->setAlignment(Qt::AlignCenter);

    infoLayout->addWidget(statusLabel);
    infoLayout->addWidget(updateLabel);

    layout->addWidget(infoFrame);
    layout->addStretch();

    m_topSidebar->setLayout(layout);
}

void DecisionSupportPage::createPageContent_mainSidebar()
{
    QVBoxLayout *layout = new QVBoxLayout(m_mainSidebar);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // Enhanced title with icon
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(16, 16));
    QLabel* titleLabel = new QLabel("AI RECOMMENDATIONS");
    titleLabel->setStyleSheet("QLabel { color: #FFFFFF; font-weight: bold; font-size: 14px; letter-spacing: 0.5px; }");

    titleLayout->addWidget(iconLabel);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    layout->addLayout(titleLayout);

    // Filter/category buttons (optional)
    QHBoxLayout* filterLayout = new QHBoxLayout();
    QButtonGroup* filterGroup = new QButtonGroup(this);

    QStringList filters = {"All", "Critical", "Fuel", "Maintenance"};
    for (const QString& filter : filters) {
        QPushButton* btn = new QPushButton(filter);
        btn->setCheckable(true);
        btn->setStyleSheet(
            "QPushButton {"
            "    background-color: #404040; color: white; border: none;"
            "    padding: 4px 12px; border-radius: 12px; font-size: 10px;"
            "}"
            "QPushButton:checked {"
            "    background-color: #007bff;"
            "}"
            "QPushButton:hover {"
            "    background-color: #505050;"
            "}"
        );
        if (filter == "All") btn->setChecked(true);
        filterGroup->addButton(btn);
        filterLayout->addWidget(btn);
    }
    filterLayout->addStretch();
    layout->addLayout(filterLayout);

    // Scroll area for recommendations with both vertical and horizontal scrolling
    QScrollArea *scrollArea = new QScrollArea(m_mainSidebar);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; background: transparent; }"
        "QScrollBar:vertical { background: #2d2d2d; width: 8px; border-radius: 4px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #555555; border-radius: 4px; min-height: 20px; margin: 1px; }"
        "QScrollBar::handle:vertical:hover { background: #666666; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar:horizontal { background: #2d2d2d; height: 8px; border-radius: 4px; margin: 0px; }"
        "QScrollBar::handle:horizontal { background: #555555; border-radius: 4px; min-width: 20px; margin: 1px; }"
        "QScrollBar::handle:horizontal:hover { background: #666666; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }"
        "QScrollBar::corner { background: #2d2d2d; }"
    );

    // Scroll widget
    QWidget *scrollWidget = new QWidget();
    scrollWidget->setMinimumWidth(300); // Set minimum width to ensure horizontal scrolling when needed
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollLayout->setContentsMargins(0, 0, 4, 0); // Adjusted for both scrollbars
    scrollLayout->setSpacing(8);

    // Create recommendation items with proper categorization
    QList<RecommendationItem> recommendations = {
        {RecommendationItem::Critical, "Speed Optimization Required",
         "Reduce speed to 12.5 knots for optimal efficiency", "8.2% fuel savings", 1, true},

        {RecommendationItem::Recommendation, "Ballast Transfer",
         "Transfer 50T FWD 1P to AFT 3S for better trim", "3.1% fuel savings", 2, true},

        {RecommendationItem::Recommendation, "AE Standby Mode",
         "Switch AE 2 to cold standby mode", "2.8% fuel savings", 3, true},

        {RecommendationItem::Info, "JIT Arrival Optimization",
         "Reduce speed by 1.2 kts for just-in-time arrival", "4.2 hrs port wait saved", 2, true},

        {RecommendationItem::Warning, "Maintenance Due",
         "ME 1 Bearing inspection scheduled", "Schedule within 7 days", 1, true},

        {RecommendationItem::Recommendation, "Trim Optimization",
         "Optimize trim by 0.5m forward", "1.8% fuel savings", 4, true},

        {RecommendationItem::Info, "Generator Load Mode",
         "Switch to low-load mode on DG 3", "2.1% fuel savings", 3, true},

        {RecommendationItem::Maintenance, "Hull Cleaning",
         "Schedule hull cleaning at next port", "4.0% fuel savings", 2, false},

        {RecommendationItem::Recommendation, "Route Optimization",
         "Re-route to avoid adverse currents", "3.6% fuel savings", 2, true}
    };

    // Sort by priority (1 = highest priority first)
    std::sort(recommendations.begin(), recommendations.end(),
              [](const RecommendationItem& a, const RecommendationItem& b) {
                  return a.priority < b.priority;
              });

    // Add recommendation cards
    for (const RecommendationItem& item : recommendations) {
        RecommendationCard* card = new RecommendationCard(item, scrollWidget);

        // Connect click signal
        connect(card, &RecommendationCard::recommendationClicked,
                this, &DecisionSupportPage::onRecommendationClicked);

        scrollLayout->addWidget(card);
    }

    scrollLayout->addStretch();
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);

    // Summary footer
    QFrame* summaryFrame = new QFrame();
    summaryFrame->setStyleSheet("QFrame { background-color: #1a1a1a; border-radius: 6px; padding: 8px; }");
    QHBoxLayout* summaryLayout = new QHBoxLayout(summaryFrame);

    QLabel* summaryLabel = new QLabel("Total Potential Savings: 25.8% fuel, 4.2 hrs");
    summaryLabel->setStyleSheet("QLabel { color: #28a745; font-weight: bold; font-size: 11px; }");
    summaryLayout->addWidget(summaryLabel);

    layout->addWidget(summaryFrame);
    m_mainSidebar->setLayout(layout);
}


void DecisionSupportPage::createPageContent_topContent()
{

}

void DecisionSupportPage::createMainContent_CIIPerformanceComparisonGraph()
{

}

void DecisionSupportPage::createMainContent_ThreeYearImplementationSummary()
{

}

void DecisionSupportPage::createMainContent_SelfEvaluationFeedbackLoop()
{

}

void DecisionSupportPage::createMainContent_CorrectiveActionPlan()
{

}

void DecisionSupportPage::onRecommendationClicked(const RecommendationItem& item)
{
    // Handle the clicked recommendation
    // You can show details, execute actions, etc.
    qDebug() << "Recommendation clicked:" << item.title;

    // Example: Show detailed dialog or execute action
    if (item.isActionable) {
        // Show confirmation dialog or execute immediately
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Execute Action",
                                      QString("Do you want to execute the action for:\n\n%1").arg(item.title),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {

        } else {
            // User cancelled
        }
    } else {
        // Show information dialog
        QMessageBox::information(this, "Information",
                                 QString("This is an informational recommendation:\n\n%1").arg(item.title));
    }
}

void DecisionSupportPage::setupWidget()
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
    m_topSidebar = new QWidget(sidebarWidget);
    m_mainSidebar = new QWidget(sidebarWidget);

    // Ratio 1:3 vertically in the sidebar
    m_sidebarContainer->addWidget(m_topSidebar, 1);  // stretch factor 1
    m_sidebarContainer->addWidget(m_mainSidebar, 3); // stretch factor 3

    // --- Content container ---
    QWidget *contentWidget = new QWidget(m_splitter);
    m_contentContainer = new QVBoxLayout(contentWidget);
    m_contentContainer->setContentsMargins(0, 0, 0, 0);
    m_contentContainer->setSpacing(6);

    // Create stacked widgets instead of regular widgets
    m_topContent = new QWidget(contentWidget);
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

void DecisionSupportPage::setupStylesheet()
{
    QString basicStyleSheet = "QWidget { background-color: #212121; }";
    m_topSidebar->setStyleSheet(basicStyleSheet );
    m_mainSidebar->setStyleSheet(basicStyleSheet );
    m_topContent->setStyleSheet(basicStyleSheet );

    QString stackedWidgetStyleSheet = "QStackedWidget { background-color: #212121; }";
    m_mainContent->setStyleSheet(stackedWidgetStyleSheet);

    m_splitter->setStyleSheet(
        "QSplitter::handle { background: transparent; }"
    );
}
