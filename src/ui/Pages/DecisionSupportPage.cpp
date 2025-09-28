#include "DecisionSupportPage.h"
#include "ui_DecisionSupportPage.h"

#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QMessageBox>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QChartView>

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

    // Add stretch to push content to top
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
    // Create the main widget for CII Performance Comparison Graph
    QWidget* ciiGraphWidget = new QWidget();
    ciiGraphWidget->setStyleSheet("QWidget { background-color: #1e1e1e; color: #ffffff; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(ciiGraphWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("CII Performance Comparison Graph");
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #ffffff; margin-bottom: 20px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Create chart view (you'll need to include QtCharts)
    #ifdef QT_CHARTS_LIB
    QChart* chart = new QChart();
    chart->setTitle("Annual CII Performance Trend");
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setBackgroundBrush(QBrush(QColor("#2d2d2d")));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Sample data - replace with your actual data
    QLineSeries* requiredCII = new QLineSeries();
    requiredCII->setName("Required CII (Baseline)");
    requiredCII->append(2020, 8.5);
    requiredCII->append(2021, 8.3);
    requiredCII->append(2022, 8.1);
    requiredCII->append(2023, 7.9);
    requiredCII->append(2024, 7.7);
    requiredCII->append(2025, 7.5);

    QLineSeries* targetedCII = new QLineSeries();
    targetedCII->setName("Targeted CII (Implementation Plan)");
    targetedCII->append(2020, 8.4);
    targetedCII->append(2021, 8.0);
    targetedCII->append(2022, 7.8);
    targetedCII->append(2023, 7.6);
    targetedCII->append(2024, 7.4);
    targetedCII->append(2025, 7.2);

    QLineSeries* attainedCII = new QLineSeries();
    attainedCII->setName("Attained Annual Operational CII");
    attainedCII->append(2020, 8.6);
    attainedCII->append(2021, 8.2);
    attainedCII->append(2022, 7.9);
    attainedCII->append(2023, 7.8);
    attainedCII->append(2024, 7.5);

    // Style the series with dark mode colors
    QPen requiredPen(QColor("#e74c3c"), 3);
    requiredCII->setPen(requiredPen);

    QPen targetedPen(QColor("#3498db"), 3);
    targetedCII->setPen(targetedPen);

    QPen attainedPen(QColor("#27ae60"), 3);
    attainedCII->setPen(attainedPen);

    chart->addSeries(requiredCII);
    chart->addSeries(targetedCII);
    chart->addSeries(attainedCII);

    // Create axes with dark mode styling
    QValueAxis* axisX = new QValueAxis;
    axisX->setTitleText("Year");
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setLabelsBrush(QBrush(Qt::white));
    axisX->setLinePen(QPen(Qt::white));
    axisX->setGridLinePen(QPen(QColor("#404040")));
    axisX->setRange(2020, 2025);
    axisX->setTickCount(6);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("CII Value (gCO2/dwt·nm)");
    axisY->setTitleBrush(QBrush(Qt::white));
    axisY->setLabelsBrush(QBrush(Qt::white));
    axisY->setLinePen(QPen(Qt::white));
    axisY->setGridLinePen(QPen(QColor("#404040")));
    axisY->setRange(7.0, 9.0);
    chart->addAxis(axisY, Qt::AlignLeft);

    requiredCII->attachAxis(axisX);
    requiredCII->attachAxis(axisY);
    targetedCII->attachAxis(axisX);
    targetedCII->attachAxis(axisY);
    attainedCII->attachAxis(axisX);
    attainedCII->attachAxis(axisY);

    // Style the legend for dark mode
    chart->legend()->setBrush(QBrush(QColor("#2d2d2d")));
    chart->legend()->setLabelBrush(QBrush(Qt::white));

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
    chartView->setStyleSheet("QChartView { background-color: #2d2d2d; border: 1px solid #404040; }");
    mainLayout->addWidget(chartView);
    #else
    // Fallback if QtCharts is not available
    QLabel* chartPlaceholder = new QLabel("Chart visualization requires QtCharts module");
    chartPlaceholder->setStyleSheet("QLabel { border: 2px dashed #666; padding: 100px; text-align: center; color: #ccc; background-color: #2d2d2d; }");
    chartPlaceholder->setAlignment(Qt::AlignCenter);
    chartPlaceholder->setMinimumHeight(400);
    mainLayout->addWidget(chartPlaceholder);
    #endif

    // Legend and analysis section
    QGroupBox* analysisGroup = new QGroupBox("Performance Analysis");
    analysisGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #ffffff; "
        "    background-color: #2d2d2d; "
        "    border: 1px solid #404040; "
        "    border-radius: 5px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "} "
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}"
    );
    QVBoxLayout* analysisLayout = new QVBoxLayout(analysisGroup);

    QLabel* trendLabel = new QLabel("Current Trend: Performance tracking shows steady improvement in CII values.");
    trendLabel->setWordWrap(true);
    trendLabel->setStyleSheet("QLabel { color: #27ae60; font-size: 12px; margin: 5px; }");
    analysisLayout->addWidget(trendLabel);

    QLabel* complianceLabel = new QLabel("Compliance Status: Meeting targeted CII requirements for regulatory compliance.");
    complianceLabel->setWordWrap(true);
    complianceLabel->setStyleSheet("QLabel { color: #3498db; font-size: 12px; margin: 5px; }");
    analysisLayout->addWidget(complianceLabel);

    mainLayout->addWidget(analysisGroup);

    // Add the widget to the stacked widget
    m_mainContent->insertWidget(MainContentPage::CIIPerformanceComparisonGraph, ciiGraphWidget);
}

void DecisionSupportPage::createMainContent_ThreeYearImplementationSummary()
{
    // Create the main widget for Three Year Implementation Summary
    QWidget* implementationWidget = new QWidget();
    implementationWidget->setStyleSheet("QWidget { background-color: #1e1e1e; color: #ffffff; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(implementationWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("3-Year Implementation Plan Summary");
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #ffffff; margin-bottom: 20px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Create table
    QTableWidget* implementationTable = new QTableWidget(0, 6);
    QStringList headers = {"Year", "Measure", "Implementation Timeline", "Responsible Personnel", "Status", "Expected CII Impact"};
    implementationTable->setHorizontalHeaderLabels(headers);

    // Style the table for dark mode
    implementationTable->setStyleSheet(
        "QTableWidget { "
        "    border: 1px solid #404040; "
        "    gridline-color: #404040; "
        "    background-color: #2d2d2d; "
        "    color: #ffffff; "
        "    alternate-background-color: #333333; "
        "} "
        "QHeaderView::section { "
        "    background-color: #34495e; "
        "    color: white; "
        "    padding: 8px; "
        "    border: none; "
        "    font-weight: bold; "
        "} "
        "QTableWidget::item { "
        "    padding: 8px; "
        "    border-bottom: 1px solid #404040; "
        "    color: #ffffff; "
        "} "
        "QTableWidget::item:selected { "
        "    background-color: #3498db; "
        "}"
    );

    // Sample data - replace with your actual implementation plan data
    QVector<QStringList> implementationData = {
        {"2025", "Hull Cleaning Optimization", "Q1 2025", "Chief Engineer", "Planned", "-0.15"},
        {"2025", "Propeller Polishing", "Q2 2025", "Deck Officer", "In Progress", "-0.08"},
        {"2025", "Engine Tuning Program", "Q3 2025", "Chief Engineer", "Planned", "-0.12"},
        {"2026", "Weather Routing System", "Q1 2026", "Navigation Officer", "Planned", "-0.20"},
        {"2026", "Waste Heat Recovery", "Q2 2026", "Chief Engineer", "Under Review", "-0.18"},
        {"2026", "Trim Optimization", "Q4 2026", "Chief Officer", "Planned", "-0.10"},
        {"2027", "Air Lubrication System", "Q2 2027", "Chief Engineer", "Evaluation Phase", "-0.25"},
        {"2027", "Alternative Fuel Integration", "Q4 2027", "Chief Engineer", "Planning", "-0.30"},
        {"2027", "Advanced Monitoring Systems", "Q1 2027", "Electronics Officer", "Planned", "-0.05"}
    };

    implementationTable->setRowCount(implementationData.size());
    for (int row = 0; row < implementationData.size(); ++row) {
        for (int col = 0; col < implementationData[row].size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(implementationData[row][col]);

            // Color code status column with dark mode colors
            if (col == 4) { // Status column
                QString status = implementationData[row][col];
                if (status == "Completed") {
                    item->setBackground(QColor("#27ae60"));
                    item->setForeground(QColor("#ffffff"));
                } else if (status == "In Progress") {
                    item->setBackground(QColor("#f39c12"));
                    item->setForeground(QColor("#ffffff"));
                } else if (status == "Planned") {
                    item->setBackground(QColor("#3498db"));
                    item->setForeground(QColor("#ffffff"));
                } else if (status == "Under Review" || status == "Evaluation Phase") {
                    item->setBackground(QColor("#e74c3c"));
                    item->setForeground(QColor("#ffffff"));
                }
            }

            // Color code impact column with dark mode colors
            if (col == 5) { // Expected CII Impact column
                double impact = implementationData[row][col].toDouble();
                if (impact < -0.15) {
                    item->setBackground(QColor("#27ae60")); // High impact - green
                    item->setForeground(QColor("#ffffff"));
                } else if (impact < -0.10) {
                    item->setBackground(QColor("#f39c12")); // Medium impact - orange
                    item->setForeground(QColor("#ffffff"));
                } else {
                    item->setBackground(QColor("#95a5a6")); // Low impact - gray
                    item->setForeground(QColor("#ffffff"));
                }
            }

            implementationTable->setItem(row, col, item);
        }
    }

    // Resize columns to content
    implementationTable->resizeColumnsToContents();
    implementationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    implementationTable->setAlternatingRowColors(true);

    mainLayout->addWidget(implementationTable);

    // Summary statistics
    QGroupBox* summaryGroup = new QGroupBox("Implementation Summary");
    summaryGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #ffffff; "
        "    background-color: #2d2d2d; "
        "    border: 1px solid #404040; "
        "    border-radius: 5px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "} "
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}"
    );
    QHBoxLayout* summaryLayout = new QHBoxLayout(summaryGroup);

    QLabel* totalMeasures = new QLabel("Total Measures: 9");
    totalMeasures->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; padding: 10px; }");

    QLabel* plannedMeasures = new QLabel("Planned: 5");
    plannedMeasures->setStyleSheet("QLabel { color: #3498db; font-size: 12px; padding: 10px; }");

    QLabel* inProgress = new QLabel("In Progress: 1");
    inProgress->setStyleSheet("QLabel { color: #f39c12; font-size: 12px; padding: 10px; }");

    QLabel* totalImpact = new QLabel("Expected Total CII Reduction: -1.43");
    totalImpact->setStyleSheet("QLabel { color: #27ae60; font-size: 12px; padding: 10px; font-weight: bold; }");

    summaryLayout->addWidget(totalMeasures);
    summaryLayout->addWidget(plannedMeasures);
    summaryLayout->addWidget(inProgress);
    summaryLayout->addStretch();
    summaryLayout->addWidget(totalImpact);

    mainLayout->addWidget(summaryGroup);

    // Add the widget to the stacked widget
    m_mainContent->insertWidget(MainContentPage::ThreeYearImplementationSummary, implementationWidget);
}

void DecisionSupportPage::createMainContent_SelfEvaluationFeedbackLoop()
{
    // Create the main widget for Self Evaluation Feedback Loop
    QWidget* evaluationWidget = new QWidget();
    evaluationWidget->setStyleSheet("QWidget { background-color: #1e1e1e; color: #ffffff; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(evaluationWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("Self-Evaluation Feedback Loop");
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #ffffff; margin-bottom: 20px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Description
    QLabel* descLabel = new QLabel("Evaluate the effectiveness of implemented measures and provide recommendations for the next SEEMP cycle.");
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("QLabel { color: #bdc3c7; font-size: 14px; margin-bottom: 15px; }");
    descLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(descLabel);

    // Create evaluation table
    QTableWidget* evaluationTable = new QTableWidget(0, 4);
    QStringList headers = {"Measure", "Implementation Status", "Effectiveness Rating", "Recommended Update for Next SEEMP Cycle"};
    evaluationTable->setHorizontalHeaderLabels(headers);

    // Style the table for dark mode
    evaluationTable->setStyleSheet(
        "QTableWidget { "
        "    border: 1px solid #404040; "
        "    gridline-color: #404040; "
        "    background-color: #2d2d2d; "
        "    color: #ffffff; "
        "    alternate-background-color: #333333; "
        "} "
        "QHeaderView::section { "
        "    background-color: #2c3e50; "
        "    color: white; "
        "    padding: 10px; "
        "    border: none; "
        "    font-weight: bold; "
        "} "
        "QTableWidget::item { "
        "    padding: 10px; "
        "    border-bottom: 1px solid #404040; "
        "    color: #ffffff; "
        "} "
        "QTableWidget::item:selected { "
        "    background-color: #3498db; "
        "}"
    );

    // Sample evaluation data
    QVector<QStringList> evaluationData = {
        {"Hull Cleaning", "Completed", "High", "Continue quarterly cleaning schedule; consider hull coating upgrade"},
        {"Propeller Polishing", "Completed", "Medium", "Extend interval to 6 months; monitor performance impact"},
        {"Engine Tuning", "Completed", "High", "Implement predictive maintenance; add cylinder condition monitoring"},
        {"Trim Optimization", "Ongoing", "High", "Digitalize trim optimization; integrate with cargo planning system"},
        {"Weather Routing", "Completed", "High", "Upgrade to AI-enhanced routing; include current data"},
        {"Waste Heat Recovery", "Partial", "Medium", "Complete installation; improve heat exchanger efficiency"},
        {"Speed Optimization", "Ongoing", "High", "Implement just-in-time arrival; coordinate with port schedules"},
        {"Air Lubrication System", "Not Started", "N/A", "Conduct feasibility study; evaluate ROI and installation complexity"},
        {"Biofuel Trials", "Completed", "Low", "Discontinue current blend; explore advanced biofuel options"},
        {"Energy Management System", "Completed", "High", "Expand monitoring scope; add crew training program"}
    };

    evaluationTable->setRowCount(evaluationData.size());
    for (int row = 0; row < evaluationData.size(); ++row) {
        for (int col = 0; col < evaluationData[row].size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(evaluationData[row][col]);

            // Color code implementation status with dark mode colors
            if (col == 1) { // Implementation Status column
                QString status = evaluationData[row][col];
                if (status == "Completed") {
                    item->setBackground(QColor("#27ae60"));
                    item->setForeground(QColor("#ffffff"));
                } else if (status == "Ongoing") {
                    item->setBackground(QColor("#f39c12"));
                    item->setForeground(QColor("#ffffff"));
                } else if (status == "Partial") {
                    item->setBackground(QColor("#e67e22"));
                    item->setForeground(QColor("#ffffff"));
                } else {
                    item->setBackground(QColor("#95a5a6"));
                    item->setForeground(QColor("#ffffff"));
                }
            }

            // Color code effectiveness rating with dark mode colors
            if (col == 2) { // Effectiveness Rating column
                QString rating = evaluationData[row][col];
                if (rating == "High") {
                    item->setBackground(QColor("#27ae60"));
                    item->setForeground(QColor("#ffffff"));
                } else if (rating == "Medium") {
                    item->setBackground(QColor("#f39c12"));
                    item->setForeground(QColor("#ffffff"));
                } else if (rating == "Low") {
                    item->setBackground(QColor("#e74c3c"));
                    item->setForeground(QColor("#ffffff"));
                } else {
                    item->setBackground(QColor("#95a5a6"));
                    item->setForeground(QColor("#ffffff"));
                }
            }

            evaluationTable->setItem(row, col, item);
        }
    }

    // Resize columns to content
    evaluationTable->resizeColumnsToContents();
    evaluationTable->horizontalHeader()->setStretchLastSection(true);
    evaluationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    evaluationTable->setAlternatingRowColors(true);

    mainLayout->addWidget(evaluationTable);

    // Feedback summary
    QGroupBox* feedbackGroup = new QGroupBox("Evaluation Summary & Next Steps");
    feedbackGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #ffffff; "
        "    background-color: #2d2d2d; "
        "    border: 1px solid #404040; "
        "    border-radius: 5px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "} "
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}"
    );
    QVBoxLayout* feedbackLayout = new QVBoxLayout(feedbackGroup);

    // Statistics
    QHBoxLayout* statsLayout = new QHBoxLayout();

    QLabel* highEffective = new QLabel("High Effectiveness: 6 measures");
    highEffective->setStyleSheet("QLabel { color: #27ae60; font-size: 12px; padding: 5px; }");

    QLabel* mediumEffective = new QLabel("Medium Effectiveness: 2 measures");
    mediumEffective->setStyleSheet("QLabel { color: #f39c12; font-size: 12px; padding: 5px; }");

    QLabel* lowEffective = new QLabel("Low Effectiveness: 1 measure");
    lowEffective->setStyleSheet("QLabel { color: #e74c3c; font-size: 12px; padding: 5px; }");

    statsLayout->addWidget(highEffective);
    statsLayout->addWidget(mediumEffective);
    statsLayout->addWidget(lowEffective);
    statsLayout->addStretch();

    feedbackLayout->addLayout(statsLayout);

    // Key recommendations
    QLabel* recommendationsTitle = new QLabel("Key Recommendations for Next SEEMP Cycle:");
    recommendationsTitle->setStyleSheet("QLabel { font-weight: bold; color: #ffffff; margin-top: 10px; }");
    feedbackLayout->addWidget(recommendationsTitle);

    QLabel* recommendations = new QLabel(
        "• Continue and enhance high-effectiveness measures (Hull Cleaning, Engine Tuning, Weather Routing)\n"
        "• Investigate alternatives for low-effectiveness measures (Biofuel Trials)\n"
        "• Complete partial implementations (Waste Heat Recovery)\n"
        "• Initiate feasibility studies for new technologies (Air Lubrication System)\n"
        "• Implement digital solutions for existing manual processes"
    );
    recommendations->setWordWrap(true);
    recommendations->setStyleSheet("QLabel { color: #bdc3c7; font-size: 12px; margin: 10px; line-height: 1.4; }");
    feedbackLayout->addWidget(recommendations);

    mainLayout->addWidget(feedbackGroup);

    // Add the widget to the stacked widget
    m_mainContent->insertWidget(MainContentPage::SelfEvaluationFeedbackLoop, evaluationWidget);
}

void DecisionSupportPage::createMainContent_CorrectiveActionPlan()
{
    // Create the main widget for Corrective Action Plan
    QWidget* correctiveWidget = new QWidget();
    correctiveWidget->setStyleSheet("QWidget { background-color: #1e1e1e; color: #ffffff; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(correctiveWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("Corrective Action Plan");
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #e74c3c; margin-bottom: 20px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Check if corrective action is needed (this would be based on your actual CII rating logic)
    bool needsCorrectiveAction = true; // Replace with your actual logic
    QString currentRating = "D"; // Replace with your actual rating
    int consecutiveYears = 3; // Replace with your actual data

    if (needsCorrectiveAction) {
        // Alert box for regulatory compliance
        QGroupBox* alertGroup = new QGroupBox("⚠ REGULATORY COMPLIANCE ALERT");
        alertGroup->setStyleSheet(
            "QGroupBox { "
            "    font-weight: bold; "
            "    font-size: 16px; "
            "    color: #e74c3c; "
            "    background-color: #3d2424; "
            "    border: 2px solid #e74c3c; "
            "    border-radius: 5px; "
            "    padding: 15px; "
            "    margin: 10px; "
            "} "
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
        );
        QVBoxLayout* alertLayout = new QVBoxLayout(alertGroup);

        QString alertText = QString("Ship CII rating of '%1' for %2 consecutive years requires mandatory corrective action plan submission.")
                               .arg(currentRating)
                               .arg(consecutiveYears);

        QLabel* alertLabel = new QLabel(alertText);
        alertLabel->setStyleSheet("QLabel { color: #e74c3c; font-size: 14px; font-weight: bold; }");
        alertLabel->setWordWrap(true);
        alertLayout->addWidget(alertLabel);

        QLabel* deadlineLabel = new QLabel("Deadline: Plan must be submitted within 18 months of rating notification.");
        deadlineLabel->setStyleSheet("QLabel { color: #f39c12; font-size: 12px; margin-top: 5px; }");
        deadlineLabel->setWordWrap(true);
        alertLayout->addWidget(deadlineLabel);

        mainLayout->addWidget(alertGroup);

        // Analysis of Causes
        QGroupBox* analysisGroup = new QGroupBox("Analysis of Causes for Inferior CII Rating");
        analysisGroup->setStyleSheet(
            "QGroupBox { "
            "    font-weight: bold; "
            "    font-size: 14px; "
            "    color: #ffffff; "
            "    background-color: #2d2d2d; "
            "    border: 1px solid #404040; "
            "    border-radius: 5px; "
            "    margin-top: 10px; "
            "    padding-top: 10px; "
            "} "
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
        );
        QVBoxLayout* analysisLayout = new QVBoxLayout(analysisGroup);

        QTextEdit* causesAnalysis = new QTextEdit();
        causesAnalysis->setPlainText(
            "ROOT CAUSE ANALYSIS:\n\n"
            "1. OPERATIONAL FACTORS:\n"
            "   • Increased voyage duration due to port congestion (15% impact)\n"
            "   • Deviation from optimal routes due to weather conditions (8% impact)\n"
            "   • Higher cargo load factor than design specifications (12% impact)\n\n"
            "2. TECHNICAL FACTORS:\n"
            "   • Hull fouling exceeding maintenance intervals (20% impact)\n"
            "   • Engine performance degradation beyond expected parameters (18% impact)\n"
            "   • Propeller damage affecting hydrodynamic efficiency (10% impact)\n\n"
            "3. EXTERNAL FACTORS:\n"
            "   • Fuel quality variations affecting combustion efficiency (5% impact)\n"
            "   • Mandatory speed restrictions in certain areas (7% impact)\n"
            "   • Loading/discharge delays increasing auxiliary power consumption (5% impact)\n\n"
            "CONCLUSION:\n"
            "The primary contributors to poor CII performance are hull maintenance issues (20%) "
            "and engine degradation (18%), representing 38% of the total impact. Immediate focus "
            "should be placed on these technical aspects while implementing operational improvements."
        );
        causesAnalysis->setStyleSheet(
            "QTextEdit { "
            "    background-color: #2d2d2d; "
            "    border: 1px solid #404040; "
            "    padding: 10px; "
            "    font-family: 'Courier New', monospace; "
            "    font-size: 11px; "
            "    color: #ffffff; "
            "}"
        );
        causesAnalysis->setMaximumHeight(200);
        causesAnalysis->setReadOnly(true);
        analysisLayout->addWidget(causesAnalysis);

        mainLayout->addWidget(analysisGroup);

        // Additional Measures Required
        QGroupBox* measuresGroup = new QGroupBox("Additional Measures Required");
        measuresGroup->setStyleSheet(
            "QGroupBox { "
            "    font-weight: bold; "
            "    font-size: 14px; "
            "    color: #ffffff; "
            "    background-color: #2d2d2d; "
            "    border: 1px solid #404040; "
            "    border-radius: 5px; "
            "    margin-top: 10px; "
            "    padding-top: 10px; "
            "} "
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
        );
        QVBoxLayout* measuresLayout = new QVBoxLayout(measuresGroup);

        // Create corrective measures table
        QTableWidget* measuresTable = new QTableWidget(0, 5);
        QStringList measuresHeaders = {"Priority", "Corrective Measure", "Implementation Timeline", "Expected Impact", "Verification Method"};
        measuresTable->setHorizontalHeaderLabels(measuresHeaders);

        // Style the corrective measures table for dark mode
        measuresTable->setStyleSheet(
            "QTableWidget { "
            "    border: 1px solid #404040; "
            "    gridline-color: #404040; "
            "    background-color: #2d2d2d; "
            "    color: #ffffff; "
            "    alternate-background-color: #333333; "
            "} "
            "QHeaderView::section { "
            "    background-color: #c0392b; "
            "    color: white; "
            "    padding: 8px; "
            "    border: none; "
            "    font-weight: bold; "
            "} "
            "QTableWidget::item { "
            "    padding: 8px; "
            "    border-bottom: 1px solid #404040; "
            "    color: #ffffff; "
            "} "
            "QTableWidget::item:selected { "
            "    background-color: #3498db; "
            "}"
        );

        // Corrective measures data
        QVector<QStringList> correctiveMeasures = {
            {"CRITICAL", "Emergency Hull Cleaning & Inspection", "Within 30 days", "-15% CII", "Before/after performance measurement"},
            {"CRITICAL", "Engine Overhaul & Optimization", "Within 60 days", "-12% CII", "Power curve analysis & fuel consumption"},
            {"HIGH", "Propeller Repair/Replacement", "Within 45 days", "-8% CII", "Propeller efficiency measurement"},
            {"HIGH", "Advanced Weather Routing System", "Within 90 days", "-6% CII", "Route optimization analysis"},
            {"MEDIUM", "Crew Training - Efficient Operations", "Within 30 days", "-4% CII", "Performance monitoring & assessment"},
            {"MEDIUM", "Trim Optimization Program", "Within 60 days", "-5% CII", "Resistance measurement & fuel monitoring"},
            {"LOW", "Energy Management System Upgrade", "Within 120 days", "-3% CII", "Energy consumption analysis"},
            {"LOW", "Auxiliary Systems Optimization", "Within 90 days", "-2% CII", "Power consumption monitoring"}
        };

        measuresTable->setRowCount(correctiveMeasures.size());
        for (int row = 0; row < correctiveMeasures.size(); ++row) {
            for (int col = 0; col < correctiveMeasures[row].size(); ++col) {
                QTableWidgetItem* item = new QTableWidgetItem(correctiveMeasures[row][col]);

                // Color code priority with dark mode colors
                if (col == 0) { // Priority column
                    QString priority = correctiveMeasures[row][col];
                    if (priority == "CRITICAL") {
                        item->setBackground(QColor("#e74c3c"));
                        item->setForeground(QColor("#ffffff"));
                    } else if (priority == "HIGH") {
                        item->setBackground(QColor("#f39c12"));
                        item->setForeground(QColor("#ffffff"));
                    } else if (priority == "MEDIUM") {
                        item->setBackground(QColor("#3498db"));
                        item->setForeground(QColor("#ffffff"));
                    } else {
                        item->setBackground(QColor("#95a5a6"));
                        item->setForeground(QColor("#ffffff"));
                    }
                }

                measuresTable->setItem(row, col, item);
            }
        }

        measuresTable->resizeColumnsToContents();
        measuresTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        measuresTable->setAlternatingRowColors(true);

        measuresLayout->addWidget(measuresTable);

        mainLayout->addWidget(measuresGroup);

        // Implementation timeline and compliance
        QGroupBox* complianceGroup = new QGroupBox("Compliance Timeline & Monitoring");
        complianceGroup->setStyleSheet(
            "QGroupBox { "
            "    font-weight: bold; "
            "    font-size: 14px; "
            "    color: #ffffff; "
            "    background-color: #2d2d2d; "
            "    border: 1px solid #404040; "
            "    border-radius: 5px; "
            "    margin-top: 10px; "
            "    padding-top: 10px; "
            "} "
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
        );
        QVBoxLayout* complianceLayout = new QVBoxLayout(complianceGroup);

        QLabel* timelineInfo = new QLabel(
            "MANDATORY SUBMISSION REQUIREMENTS:\n"
            "• Submit plan to flag state within 18 months of CII rating notification\n"
            "• Implement critical measures within 60 days of plan approval\n"
            "• Provide quarterly progress reports to regulatory authorities\n"
            "• Demonstrate CII improvement within next rating period\n\n"
            "ESTIMATED TOTAL CII IMPROVEMENT: -55% (Target: Achieve 'C' rating or better)\n"
            "TOTAL IMPLEMENTATION COST: Under review\n"
            "COMPLIANCE VERIFICATION: Third-party verification required for technical measures"
        );
        timelineInfo->setWordWrap(true);
        timelineInfo->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; background-color: #2d2d2d; padding: 15px; border-left: 4px solid #e74c3c; }");
        complianceLayout->addWidget(timelineInfo);

        mainLayout->addWidget(complianceGroup);

    } else {
        // No corrective action needed - show compliance status
        QGroupBox* statusGroup = new QGroupBox("✓ COMPLIANCE STATUS");
        statusGroup->setStyleSheet(
            "QGroupBox { "
            "    font-weight: bold; "
            "    font-size: 16px; "
            "    color: #27ae60; "
            "    background-color: #1e3a28; "
            "    border: 2px solid #27ae60; "
            "    border-radius: 5px; "
            "    padding: 15px; "
            "    margin: 10px; "
            "} "
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
        );
        QVBoxLayout* statusLayout = new QVBoxLayout(statusGroup);

        QLabel* statusLabel = new QLabel("Ship CII performance meets regulatory requirements. No corrective action plan required.");
        statusLabel->setStyleSheet("QLabel { color: #27ae60; font-size: 14px; font-weight: bold; }");
        statusLabel->setWordWrap(true);
        statusLayout->addWidget(statusLabel);

        QLabel* maintenanceLabel = new QLabel("Continue current SEEMP implementation and monitoring practices.");
        maintenanceLabel->setStyleSheet("QLabel { color: #2ecc71; font-size: 12px; margin-top: 5px; }");
        maintenanceLabel->setWordWrap(true);
        statusLayout->addWidget(maintenanceLabel);

        mainLayout->addWidget(statusGroup);

        // Optional improvement suggestions
        QGroupBox* improvementGroup = new QGroupBox("Optional Performance Enhancement Opportunities");
        improvementGroup->setStyleSheet(
            "QGroupBox { "
            "    font-weight: bold; "
            "    font-size: 14px; "
            "    color: #ffffff; "
            "    background-color: #2d2d2d; "
            "    border: 1px solid #404040; "
            "    border-radius: 5px; "
            "    margin-top: 10px; "
            "    padding-top: 10px; "
            "} "
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
        );
        QVBoxLayout* improvementLayout = new QVBoxLayout(improvementGroup);

        QLabel* improvementText = new QLabel(
            "While no corrective action is required, consider these opportunities for continued improvement:\n\n"
            "• Implement advanced fuel monitoring systems\n"
            "• Explore alternative fuel options for future compliance\n"
            "• Enhance crew training programs for operational efficiency\n"
            "• Consider hull coating upgrades during next dry dock\n"
            "• Evaluate emerging energy-saving technologies"
        );
        improvementText->setWordWrap(true);
        improvementText->setStyleSheet("QLabel { color: #bdc3c7; font-size: 12px; padding: 10px; }");
        improvementLayout->addWidget(improvementText);

        mainLayout->addWidget(improvementGroup);
    }

    // Action buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    if (needsCorrectiveAction) {
        QPushButton* exportPlanBtn = new QPushButton("Export Corrective Action Plan");
        exportPlanBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #e74c3c; "
            "    color: white; "
            "    border: none; "
            "    padding: 10px 20px; "
            "    border-radius: 5px; "
            "    font-weight: bold; "
            "} "
            "QPushButton:hover { "
            "    background-color: #c0392b; "
            "} "
            "QPushButton:pressed { "
            "    background-color: #a93226; "
            "}"
        );

        QPushButton* submitBtn = new QPushButton("Submit to Flag State");
        submitBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #2c3e50; "
            "    color: white; "
            "    border: none; "
            "    padding: 10px 20px; "
            "    border-radius: 5px; "
            "    font-weight: bold; "
            "} "
            "QPushButton:hover { "
            "    background-color: #34495e; "
            "} "
            "QPushButton:pressed { "
            "    background-color: #1b2631; "
            "}"
        );

        QPushButton* trackProgressBtn = new QPushButton("Track Implementation Progress");
        trackProgressBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #3498db; "
            "    color: white; "
            "    border: none; "
            "    padding: 10px 20px; "
            "    border-radius: 5px; "
            "    font-weight: bold; "
            "} "
            "QPushButton:hover { "
            "    background-color: #2980b9; "
            "} "
            "QPushButton:pressed { "
            "    background-color: #1f618d; "
            "}"
        );

        buttonLayout->addWidget(exportPlanBtn);
        buttonLayout->addWidget(submitBtn);
        buttonLayout->addWidget(trackProgressBtn);
        buttonLayout->addStretch();
    } else {
        QPushButton* exportStatusBtn = new QPushButton("Export Compliance Report");
        exportStatusBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #27ae60; "
            "    color: white; "
            "    border: none; "
            "    padding: 10px 20px; "
            "    border-radius: 5px; "
            "    font-weight: bold; "
            "} "
            "QPushButton:hover { "
            "    background-color: #229954; "
            "} "
            "QPushButton:pressed { "
            "    background-color: #1e8449; "
            "}"
        );

        buttonLayout->addWidget(exportStatusBtn);
        buttonLayout->addStretch();
    }

    mainLayout->addLayout(buttonLayout);

    // Add the widget to the stacked widget
    m_mainContent->insertWidget(MainContentPage::CorrectiveActionPlan, correctiveWidget);
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
    // m_contentContainer->addWidget(m_topContent, 1);  // stretch factor 1
    m_contentContainer->addWidget(m_mainContent, 4); // stretch factor 3

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
