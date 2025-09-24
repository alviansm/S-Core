#include "TechnicalPage.h"
#include "ui_TechnicalPage.h"

#include <QSizePolicy>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QListWidget>
#include <QListWidgetItem>

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QBrush>
#include <QRandomGenerator>

#include "CircleProgressBar.h"
#include "SpeedometerWidget.h"
#include "EngineStatusWidget.h"
#include "PropulsionPIDWidget.h"

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

    QLabel* fuelRateValue = new QLabel("8.7 T/H");
    fuelRateValue->setStyleSheet("color: #3498DB; font-size: 12px; font-weight: bold;");
    fuelRateValue->setAlignment(Qt::AlignCenter);
    fuelLayout->addWidget(fuelRateValue);

    fuelWidget->setToolTip("Fuel Consumption Rate");
    topRowLayout->addWidget(fuelWidget);

    layout->addWidget(topRowWidget);

    // Add some spacing
    layout->addSpacing(0);

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
    QVBoxLayout* mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(8);

    // Header section with title and component selector
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);

    QLabel* titleLabel = new QLabel("Detailed Consumption");
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "}"
    );
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Component selector combo box
    QComboBox* componentSelector = new QComboBox();
    componentSelector->setMinimumWidth(120);
    componentSelector->setStyleSheet(
        "QComboBox {"
        "    background-color: #3c3c3c;"
        "    border: 1px solid #555555;"
        "    border-radius: 4px;"
        "    padding: 4px 8px;"
        "    color: white;"
        "    font-size: 11px;"
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #555555;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 4px solid transparent;"
        "    border-right: 4px solid transparent;"
        "    border-top: 4px solid #cccccc;"
        "    margin-top: 2px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: #3c3c3c;"
        "    border: 1px solid #555555;"
        "    color: white;"
        "    selection-background-color: #0078d4;"
        "}"
    );

    // Add component options
    componentSelector->addItem("ME 1 - Main Engine");
    componentSelector->addItem("ME 2 - Main Engine");
    componentSelector->addItem("ME 3 - Main Engine");
    componentSelector->addItem("GB 1 - Gearbox");
    componentSelector->addItem("Shaft System");
    componentSelector->addItem("Sterntube Bearing");
    componentSelector->addItem("Propeller");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(componentSelector);

    mainLayout->addLayout(headerLayout);

    // Scrollable area for parameters
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QScrollBar:vertical {"
        "    background: #2b2b2b;"
        "    width: 12px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #555555;"
        "    border-radius: 6px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #666666;"
        "}"
    );

    // Parameters list widget
    QListWidget* parametersList = new QListWidget();
    parametersList->setStyleSheet(
        "QListWidget {"
        "    background-color: transparent;"
        "    border: none;"
        "    outline: none;"
        "}"
        "QListWidget::item {"
        "    background-color: #2d2d2d;"
        "    border: 1px solid #404040;"
        "    border-radius: 4px;"
        "    margin: 2px 0px;"
        "    padding: 8px;"
        "    color: white;"
        "    font-size: 11px;"
        "}"
        "QListWidget::item:hover {"
        "    background-color: #353535;"
        "    border-color: #505050;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #404040;"
        "    border-color: #0078d4;"
        "}"
    );

    // Function to create parameter item widget
    auto createParameterItem = [](const QString& name, const QString& value,
                                 const QString& unit, const QString& status) -> QWidget* {
        QWidget* itemWidget = new QWidget();
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
        itemLayout->setContentsMargins(0, 0, 0, 0);
        itemLayout->setSpacing(8);

        // Status indicator (alert icon)
        QLabel* statusIcon = new QLabel();
        statusIcon->setFixedSize(16, 16);
        statusIcon->setAlignment(Qt::AlignCenter);

        QString iconStyle;
        if (status == "critical") {
            iconStyle = "background-color: #dc3545; border-radius: 8px;"; // Red
        } else if (status == "warning") {
            iconStyle = "background-color: #ffc107; border-radius: 8px;"; // Amber
        } else {
            iconStyle = "background-color: #28a745; border-radius: 8px;"; // Green
        }
        statusIcon->setStyleSheet(iconStyle);
        statusIcon->setText("●");
        statusIcon->setStyleSheet(statusIcon->styleSheet() + " color: white; font-weight: bold;");

        // Parameter info
        QVBoxLayout* infoLayout = new QVBoxLayout();
        infoLayout->setContentsMargins(0, 0, 0, 0);
        infoLayout->setSpacing(2);

        QLabel* nameLabel = new QLabel(name);
        nameLabel->setStyleSheet("color: #cccccc; font-size: 10px; font-weight: normal;");

        QLabel* valueLabel = new QLabel(value + " " + unit);
        valueLabel->setStyleSheet("color: white; font-size: 12px; font-weight: bold;");

        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(valueLabel);

        itemLayout->addWidget(statusIcon);
        itemLayout->addLayout(infoLayout);
        itemLayout->addStretch();

        return itemWidget;
    };

    // Function to populate parameters based on selected component
    auto populateParameters = [=](const QString& component) {
        parametersList->clear();

        if (component.startsWith("ME")) {
            // Main Engine parameters
            QStringList parameters = {
                "Fuel Consumption|285.5|kg/h|normal",
                "Engine Load|78.2|%|normal",
                "Engine RPM|127|rpm|normal",
                "Exhaust Gas Temp (High)|387|°C|warning",
                "Exhaust Gas Temp (Low)|365|°C|normal",
                "1st Cylinder Temp|412|°C|critical",
                "2nd Cylinder Temp|398|°C|normal",
                "3rd Cylinder Temp|401|°C|normal",
                "4th Cylinder Temp|395|°C|normal",
                "Lube Oil Pressure|4.2|bar|normal",
                "Lube Oil Temperature|65|°C|normal",
                "Cooling Water Temp|82|°C|normal",
                "Turbocharger RPM|15420|rpm|normal",
                "Scavenge Air Pressure|2.8|bar|normal",
                "Fuel Pressure|8.5|bar|normal"
            };

            for (const QString& param : parameters) {
                QStringList parts = param.split("|");
                QListWidgetItem* item = new QListWidgetItem(parametersList);
                item->setSizeHint(QSize(0, 50));
                parametersList->addItem(item);
                parametersList->setItemWidget(item, createParameterItem(
                    parts[0], parts[1], parts[2], parts[3]));
            }

        } else if (component.startsWith("GB")) {
            // Gearbox parameters
            QStringList parameters = {
                "Input RPM|127|rpm|normal",
                "Output RPM|89|rpm|normal",
                "Oil Pressure|3.8|bar|normal",
                "Oil Temperature|58|°C|normal",
                "Bearing Temperature|45|°C|normal",
                "Vibration Level|2.1|mm/s|warning",
                "Torque|12500|Nm|normal"
            };

            for (const QString& param : parameters) {
                QStringList parts = param.split("|");
                QListWidgetItem* item = new QListWidgetItem(parametersList);
                item->setSizeHint(QSize(0, 50));
                parametersList->addItem(item);
                parametersList->setItemWidget(item, createParameterItem(
                    parts[0], parts[1], parts[2], parts[3]));
            }

        } else if (component == "Shaft System") {
            // Shaft parameters
            QStringList parameters = {
                "RPM|89.2|rpm|normal",
                "Thrust|145|kN|normal",
                "Forward Bearing Temp|42|°C|normal",
                "Aft Bearing Temp|38|°C|normal",
                "Vibration (Horizontal)|1.8|mm/s|normal",
                "Vibration (Vertical)|2.3|mm/s|warning",
                "Alignment|0.15|mm|normal"
            };

            for (const QString& param : parameters) {
                QStringList parts = param.split("|");
                QListWidgetItem* item = new QListWidgetItem(parametersList);
                item->setSizeHint(QSize(0, 50));
                parametersList->addItem(item);
                parametersList->setItemWidget(item, createParameterItem(
                    parts[0], parts[1], parts[2], parts[3]));
            }

        } else if (component == "Sterntube Bearing") {
            // Sterntube bearing parameters
            QStringList parameters = {
                "Forward Bearing Temp|41|°C|normal",
                "Aft Bearing Temp|44|°C|normal",
                "Oil Flow Rate|12.5|l/min|normal",
                "Oil Temperature|52|°C|warning",
                "Seal Pressure|0.8|bar|normal",
                "Wear Rate|0.02|mm/year|normal"
            };

            for (const QString& param : parameters) {
                QStringList parts = param.split("|");
                QListWidgetItem* item = new QListWidgetItem(parametersList);
                item->setSizeHint(QSize(0, 50));
                parametersList->addItem(item);
                parametersList->setItemWidget(item, createParameterItem(
                    parts[0], parts[1], parts[2], parts[3]));
            }

        } else if (component == "Propeller") {
            // Propeller parameters
            QStringList parameters = {
                "RPM|89.2|rpm|normal",
                "Pitch|75|%|normal",
                "Thrust|145|kN|normal",
                "Torque|1850|kNm|normal",
                "Efficiency|68.5|%|warning",
                "Cavitation Index|0.8|-|normal",
                "Blade Angle|23.5|degrees|normal"
            };

            for (const QString& param : parameters) {
                QStringList parts = param.split("|");
                QListWidgetItem* item = new QListWidgetItem(parametersList);
                item->setSizeHint(QSize(0, 50));
                parametersList->addItem(item);
                parametersList->setItemWidget(item, createParameterItem(
                    parts[0], parts[1], parts[2], parts[3]));
            }
        }
    };

    // Initially populate with ME 1 data
    populateParameters("ME 1 - Main Engine");

    // Connect combo box selection change
    QObject::connect(componentSelector, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
                     [populateParameters](const QString& text) {
                         populateParameters(text);
                     });

    scrollArea->setWidget(parametersList);
    mainLayout->addWidget(scrollArea);

    // Set stretch factor to make the scroll area expand
    mainLayout->setStretchFactor(scrollArea, 1);

    m_mainSidebar->addWidget(page);
}

void TechnicalPage::createPropulsionSystemPageContent_topContent()
{
    QWidget* page = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(page);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(12);

    // Left section - Speedometer
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(4);

    QLabel* titleLabel = new QLabel("Balancing/Efficiency");
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "}"
    );

    // Container for speedometer to control height
    QWidget* speedometerContainer = new QWidget();
    speedometerContainer->setMaximumHeight(210); // Use maximum height to match Summary
    QVBoxLayout* speedometerLayout = new QVBoxLayout(speedometerContainer);
    speedometerLayout->setContentsMargins(0, 0, 0, 0);
    speedometerLayout->setAlignment(Qt::AlignTop); // Align to top instead of center

    SpeedometerWidget* speedometer = new SpeedometerWidget();
    speedometer->setRange(0, 60);
    speedometer->setValue(42.8);
    speedometer->setTitle("Total Thermal Efficiency");
    speedometer->setUnit("%");

    speedometerLayout->addWidget(speedometer);

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(speedometerContainer);
    leftLayout->addStretch();

    // Middle section - Engine Status (removed title)
    QVBoxLayout* middleLayout = new QVBoxLayout();
    middleLayout->setSpacing(4);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setFixedWidth(200);
    scrollArea->setMaximumHeight(210); // Use maximum height instead of fixed height
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Hide vertical scroll bar
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    background-color: transparent;" // Back to transparent
        "    border: none;"
        "}"
    );

    QWidget* enginesWidget = new QWidget();
    enginesWidget->setStyleSheet("QWidget { background-color: transparent; }"); // Transparent background
    QVBoxLayout* enginesLayout = new QVBoxLayout(enginesWidget);
    enginesLayout->setContentsMargins(0, 0, 0, 0);
    enginesLayout->setSpacing(4);

    // Add engine status widgets
    EngineStatusWidget* me1 = new EngineStatusWidget("ME 1");
    me1->setEfficiency(42.8);
    me1->setRunningHours(18.5);
    me1->setSFOC(186);

    EngineStatusWidget* me2 = new EngineStatusWidget("ME 2");
    me2->setEfficiency(38.2);
    me2->setRunningHours(19.2);
    me2->setSFOC(195);

    EngineStatusWidget* me3 = new EngineStatusWidget("ME 3");
    me3->setEfficiency(45.1);
    me3->setRunningHours(17.8);
    me3->setSFOC(178);

    enginesLayout->addWidget(me1);
    enginesLayout->addWidget(me2);
    enginesLayout->addWidget(me3);
    enginesLayout->addStretch();

    scrollArea->setWidget(enginesWidget);

    middleLayout->addWidget(scrollArea);

    // Right section - QtCharts Line Chart
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(4);

    // Create QtCharts chart
    QChart* chart = new QChart();
    chart->setTheme(QChart::ChartThemeDark);
    chart->setBackgroundBrush(QBrush(QColor(45, 45, 45)));
    chart->setTitleBrush(QBrush(Qt::white));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight); // Legend on the right side
    chart->legend()->setLabelColor(Qt::white);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setTitle(""); // Remove title

    // Create chart view
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(200);
    chartView->setStyleSheet("background-color: transparent; border: none;"); // Remove frame

    // Function to create sample data for different chart types
    auto createSampleData = [](const QString& type) -> QLineSeries* {
        QLineSeries* series = new QLineSeries();

        // Generate sample data for the last 7 days
        QDateTime currentDate = QDateTime::currentDateTime();

        if (type == "Power (kW)") {
            series->setName("Engine Power");
            series->setColor(QColor(40, 167, 69)); // Green
            for (int i = 6; i >= 0; --i) {
                QDateTime date = currentDate.addDays(-i);
                double power = 12.0 + (QRandomGenerator::global()->bounded(2000) - 1000) / 1000.0; // 11-13 (in thousands kW)
                series->append(date.toMSecsSinceEpoch(), power);
            }
        } else if (type == "RPM") {
            series->setName("Engine RPM");
            series->setColor(QColor(0, 120, 212)); // Blue
            for (int i = 6; i >= 0; --i) {
                QDateTime date = currentDate.addDays(-i);
                double rpm = 125 + QRandomGenerator::global()->bounded(10) - 5; // 120-130 RPM
                series->append(date.toMSecsSinceEpoch(), rpm);
            }
        } else { // Efficiency (%)
            series->setName("Thermal Efficiency");
            series->setColor(QColor(255, 193, 7)); // Yellow
            for (int i = 6; i >= 0; --i) {
                QDateTime date = currentDate.addDays(-i);
                double efficiency = 40 + QRandomGenerator::global()->bounded(8); // 40-48%
                series->append(date.toMSecsSinceEpoch(), efficiency);
            }
        }

        return series;
    };

    // Function to update chart data
    auto updateChart = [=](const QString& type) {
        chart->removeAllSeries();

        // Add three series for ME1, ME2, ME3
        QLineSeries* me1Series = createSampleData(type);
        QLineSeries* me2Series = createSampleData(type);
        QLineSeries* me3Series = createSampleData(type);

        // Adjust names and colors for different engines
        me1Series->setName("ME 1");
        me1Series->setColor(QColor(40, 167, 69)); // Green

        me2Series->setName("ME 2");
        me2Series->setColor(QColor(0, 120, 212)); // Blue
        // Slightly offset ME2 data
        for (int i = 0; i < me2Series->count(); ++i) {
            QPointF point = me2Series->at(i);
            if (type == "Power (kW)") {
                me2Series->replace(i, point.x(), point.y() - 0.5);
            } else if (type == "RPM") {
                me2Series->replace(i, point.x(), point.y() - 2);
            } else {
                me2Series->replace(i, point.x(), point.y() - 3);
            }
        }

        me3Series->setName("ME 3");
        me3Series->setColor(QColor(255, 193, 7)); // Yellow
        // Slightly offset ME3 data
        for (int i = 0; i < me3Series->count(); ++i) {
            QPointF point = me3Series->at(i);
            if (type == "Power (kW)") {
                me3Series->replace(i, point.x(), point.y() + 0.3);
            } else if (type == "RPM") {
                me3Series->replace(i, point.x(), point.y() + 1);
            } else {
                me3Series->replace(i, point.x(), point.y() + 2);
            }
        }

        chart->addSeries(me1Series);
        chart->addSeries(me2Series);
        chart->addSeries(me3Series);

        // Create axes
        QDateTimeAxis* axisX = new QDateTimeAxis();
        axisX->setTitleText(""); // Remove X axis title
        axisX->setTitleBrush(QBrush(Qt::white));
        axisX->setLabelsBrush(QBrush(QColor(200, 200, 200)));
        axisX->setLabelsColor(QColor(200, 200, 200));
        axisX->setGridLineColor(QColor(100, 100, 100));
        axisX->setFormat("MM/dd");
        axisX->setRange(QDateTime::currentDateTime().addDays(-6), QDateTime::currentDateTime());

        QValueAxis* axisY = new QValueAxis();
        axisY->setTitleText(""); // Remove Y axis title
        axisY->setTitleBrush(QBrush(Qt::white));
        axisY->setLabelsBrush(QBrush(QColor(200, 200, 200)));
        axisY->setLabelsColor(QColor(200, 200, 200));
        axisY->setGridLineColor(QColor(100, 100, 100));

        // Set Y-axis range and format based on data type
        if (type == "Power (kW)") {
            axisY->setRange(10.0, 14.0);
            axisY->setLabelFormat("%.1f kW"); // Show values with kW unit
        } else if (type == "RPM") {
            axisY->setRange(115, 135);
            axisY->setLabelFormat("%.0f RPM"); // Show values with RPM unit
        } else { // Efficiency
            axisY->setRange(35, 50);
            axisY->setLabelFormat("%.0f%%"); // Show values with % unit
        }

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        // Attach series to axes
        me1Series->attachAxis(axisX);
        me1Series->attachAxis(axisY);
        me2Series->attachAxis(axisX);
        me2Series->attachAxis(axisY);
        me3Series->attachAxis(axisX);
        me3Series->attachAxis(axisY);
    };

    // Initialize chart with Power data (fixed to Power only since combo box is removed)
    updateChart("Power (kW)");

    rightLayout->addWidget(chartView);

    // Add all sections to main layout
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(rightLayout, 1); // Give chart more space

    m_topContent->addWidget(page);
}

void TechnicalPage::createPropulsionSystemPageContent_mainContent()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(8);

    // Title
    QLabel* titleLabel = new QLabel("P&ID Diagram");
    titleLabel->setStyleSheet("color: white; font-weight: bold; font-size: 14px;");
    titleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(titleLabel);

    // Create the P&ID diagram widget
    PropulsionPIDWidget* pidWidget = new PropulsionPIDWidget();
    layout->addWidget(pidWidget, 1);

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
