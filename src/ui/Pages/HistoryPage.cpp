#include "HistoryPage.h"
#include "ui_HistoryPage.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDir>

HistoryPage::HistoryPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryPage)
    , m_updateTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Apply global dark theme to the entire widget
    QString globalStyleSheet = R"(
        HistoryPage {
            background-color: #2B2B2B;
            color: white;
        }
        QWidget {
            background-color: #2B2B2B;
            color: white;
        }
        QFrame {
            background-color: #2B2B2B;
            color: white;
        }
        QListWidget {
            background-color: #333333;
            border: none;
            outline: 0;
            color: white;
        }
        QListWidget::item {
            background-color: #4F4F4F;
            color: white;
            padding: 10px;
            margin: 6px 10px;
            border-radius: 8px;
            font-size: 12pt;
        }
        QListWidget::item:hover {
            background-color: #616161;
        }
        QListWidget::item:selected {
            background-color: #4F4F4F;
            color: white;
            border: 2px solid #34A853;
        }
        QComboBox {
            background-color: #4F4F4F;
            color: white;
            border: 1px solid #666666;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 11pt;
            min-height: 20px;
        }
        QComboBox:hover {
            background-color: #616161;
            border-color: #888888;
        }
        QComboBox:focus {
            border-color: #34A853;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 20px;
            border-left: 1px solid #666666;
            background-color: #4F4F4F;
        }
        QComboBox::drop-down:hover {
            background-color: #616161;
        }
        QComboBox::down-arrow {
            image: none;
            width: 0;
            height: 0;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid white;
        }
        QComboBox QAbstractItemView {
            background-color: #4F4F4F;
            color: white;
            selection-background-color: #34A853;
            selection-color: white;
            border: 1px solid #666666;
            outline: 0;
        }
        QComboBox QAbstractItemView::item {
            padding: 8px;
            border: none;
        }
        QComboBox QAbstractItemView::item:hover {
            background-color: #616161;
        }
        QLabel {
            color: white;
            font-size: 11pt;
            background-color: transparent;
        }
        QScrollArea {
            background-color: #2B2B2B;
            border: 1px solid #555555;
        }
        QScrollArea > QWidget > QWidget {
            background-color: #2B2B2B;
        }
        QScrollBar:vertical {
            background-color: #4F4F4F;
            width: 12px;
            border: none;
        }
        QScrollBar::handle:vertical {
            background-color: #666666;
            border-radius: 6px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #888888;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            border: none;
            background: none;
        }
        QScrollBar:horizontal {
            background-color: #4F4F4F;
            height: 12px;
            border: none;
        }
        QScrollBar::handle:horizontal {
            background-color: #666666;
            border-radius: 6px;
            min-width: 20px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #888888;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            border: none;
            background: none;
        }
        QPushButton {
            background-color: #4F4F4F;
            color: white;
            border: 1px solid #666666;
            border-radius: 4px;
            padding: 6px 12px;
            font-size: 11pt;
        }
        QPushButton:hover {
            background-color: #616161;
        }
        QPushButton:pressed {
            background-color: #333333;
        }
        QTextEdit {
            background-color: #4F4F4F;
            color: white;
            border: 1px solid #666666;
            selection-background-color: #34A853;
        }
        QDialog {
            background-color: #333333;
            color: white;
        }
        QStackedWidget {
            background-color: #2B2B2B;
        }
    )";

    this->setStyleSheet(globalStyleSheet);
    ui->listWidget->setIconSize(QSize(48, 48));

    // Add navigation items
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/home.png"), "DCS"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/folders.png"), "Historical"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/tags.png"), "Report"));

    if (ui->listWidget->count() > 0) {
        ui->listWidget->setCurrentRow(0);
    }

    setupNavigationAndContent();

    // Connect navigation selection
    connect(ui->listWidget, &QListWidget::currentRowChanged,
            this, &HistoryPage::onNavigationSelectionChanged);

    // Setup timer for DCS updates
    connect(m_updateTimer, &QTimer::timeout, this, &HistoryPage::updateDCSChart);
    m_updateTimer->start(5000); // Update every 5 seconds
}

HistoryPage::~HistoryPage()
{
    delete ui;
}

void HistoryPage::setupNavigationAndContent()
{
    // Clear existing pages
    while (ui->stackedWidget->count() > 0) {
        QWidget *widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }

    // Add pages
    ui->stackedWidget->addWidget(createDCSPage());
    ui->stackedWidget->addWidget(createHistoricalPage());
    ui->stackedWidget->addWidget(createReportPage());

    ui->stackedWidget->setCurrentIndex(0);
}

QWidget* HistoryPage::createDCSPage()
{
    QWidget *dcsPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(dcsPage);

    // Title with enhanced dark styling
    QLabel *title = new QLabel("Data Collection System");
    title->setStyleSheet("font-size: 16pt; font-weight: bold; color: white; margin-bottom: 10px; background-color: transparent;");
    layout->addWidget(title);

    // Component selection with proper spacing
    QHBoxLayout *selectionLayout = new QHBoxLayout;
    selectionLayout->setSpacing(10);

    QLabel *componentLabel = new QLabel("Select Component:");
    componentLabel->setStyleSheet("color: white; font-size: 11pt; background-color: transparent; min-width: 120px;");

    m_dcsComponentCombo = new QComboBox;
    m_dcsComponentCombo->setMinimumWidth(300);
    m_dcsComponentCombo->addItem("Main Engine (ME) - Exhaust Temperature");
    m_dcsComponentCombo->addItem("Main Engine (ME) - Cylinder Temperature");
    m_dcsComponentCombo->addItem("Auxiliary Engine (AE) - Exhaust Temperature");
    // m_dcsComponentCombo->addItem("Boiler - Steam Temperature");
    // m_dcsComponentCombo->addItem("Turbocharger - Inlet Temperature");

    selectionLayout->addWidget(componentLabel);
    selectionLayout->addWidget(m_dcsComponentCombo);
    selectionLayout->addStretch();
    layout->addLayout(selectionLayout);

    // Chart
    m_chart = new QChart();
    m_chart->setTitle("Real-time Monitoring Data");
    m_chart->setTitleBrush(QBrush(Qt::white));
    m_chart->setBackgroundBrush(QBrush(QColor(43, 43, 43)));
    m_chart->legend()->setLabelColor(Qt::white);

    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setStyleSheet("background-color: #2B2B2B; border: 1px solid #555555;");

    layout->addWidget(m_chartView, 1);

    connect(m_dcsComponentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &HistoryPage::onDCSComponentChanged);

    // Generate initial data
    generateDummyChartData();

    return dcsPage;
}

QWidget* HistoryPage::createHistoricalPage()
{
    QWidget *historicalPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(historicalPage);

    // Title with dark theme styling
    QLabel *title = new QLabel("Historical Data - Carbon Emission Heat Map");
    title->setStyleSheet("font-size: 16pt; font-weight: bold; color: white; margin-bottom: 10px; background-color: transparent;");
    layout->addWidget(title);

    // Legend with better spacing and dark theme
    QHBoxLayout *legendLayout = new QHBoxLayout;
    legendLayout->setSpacing(5);

    QLabel *legendLabel = new QLabel("Emission Level:");
    legendLabel->setStyleSheet("color: white; margin-right: 10px; background-color: transparent; font-size: 11pt;");
    legendLayout->addWidget(legendLabel);

    // Create legend items
    QStringList legendItems = {"Low", "Medium", "High", "Critical"};
    QStringList legendColors = {"#2E7D32", "#FBC02D", "#F57C00", "#D32F2F"};

    for (int i = 0; i < legendItems.size(); ++i) {
        QFrame *colorBox = new QFrame;
        colorBox->setFixedSize(20, 20);
        colorBox->setStyleSheet(QString("background-color: %1; border: 1px solid #666666;").arg(legendColors[i]));
        QLabel *legendText = new QLabel(legendItems[i]);
        legendText->setStyleSheet("color: white; margin-right: 15px; background-color: transparent; font-size: 10pt;");

        legendLayout->addWidget(colorBox);
        legendLayout->addWidget(legendText);
    }
    legendLayout->addStretch();
    layout->addLayout(legendLayout);

    // Heat map scroll area
    m_heatMapScrollArea = new QScrollArea;
    m_heatMapScrollArea->setWidgetResizable(true);
    m_heatMapScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_heatMapScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    createHeatMapGrid();

    layout->addWidget(m_heatMapScrollArea, 1);

    return historicalPage;
}

QWidget* HistoryPage::createReportPage()
{
    QWidget *reportPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(reportPage);

    // Title with dark theme
    QLabel *title = new QLabel("CII Compliance Reports");
    title->setStyleSheet("font-size: 16pt; font-weight: bold; color: white; margin-bottom: 10px; background-color: transparent;");
    layout->addWidget(title);

    // Report selection with proper sizing
    QHBoxLayout *selectionLayout = new QHBoxLayout;
    selectionLayout->setSpacing(10);

    QLabel *reportLabel = new QLabel("Select Report:");
    reportLabel->setStyleSheet("color: white; font-size: 11pt; background-color: transparent; min-width: 100px;");

    m_reportCombo = new QComboBox;
    m_reportCombo->setMinimumWidth(400);
    m_reportCombo->addItem("SHIP MANAGEMENT PLAN TO IMPROVE ENERGY EFFICIENCY (PART I OF THE SEEMP)");
    m_reportCombo->addItem("SHIP FUEL OIL CONSUMPTION DATA COLLECTION PLAN (PART II OF THE SEEMP)");
    m_reportCombo->addItem("SHIP OPERATIONAL CARBON INTENSITY PLAN (PART III OF THE SEEMP)");
    m_reportCombo->addItem("STANDARDIZED DATA REPORTING FORMAT FOR THE DATA COLLECTION SYSTEM");
    m_reportCombo->addItem("STANDARDIZED DATA REPORTING FORMAT FOR THE PARAMETERS TO CALCULATE TRIAL CII");

    selectionLayout->addWidget(reportLabel);
    selectionLayout->addWidget(m_reportCombo);
    selectionLayout->addStretch();
    layout->addLayout(selectionLayout);

    // PDF viewer
    m_pdfDocument = new QPdfDocument(this);
    m_pdfView = new QPdfView;
    m_pdfView->setDocument(m_pdfDocument);
    m_pdfView->setStyleSheet("background-color: #2B2B2B; border: 1px solid #555555;");

    layout->addWidget(m_pdfView, 1);

    connect(m_reportCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &HistoryPage::onReportSelectionChanged);

    // Load first report
    onReportSelectionChanged();

    return reportPage;
}

void HistoryPage::createHeatMapGrid()
{
    QWidget *heatMapWidget = new QWidget;
    m_heatMapLayout = new QGridLayout(heatMapWidget);
    m_heatMapLayout->setSpacing(2);

    // Create time labels (hours)
    QStringList timeLabels;
    for (int hour = 0; hour < 24; ++hour) {
        timeLabels << QString("%1:00").arg(hour, 2, 10, QChar('0'));
    }

    // Create date labels (7 days)
    QStringList dateLabels;
    QDate currentDate = QDate::currentDate().addDays(-6);
    for (int day = 0; day < 7; ++day) {
        dateLabels << currentDate.addDays(day).toString("d MMM yy");
    }

    // Add time header with dark theme styling
    for (int col = 0; col < timeLabels.size(); ++col) {
        QLabel *timeLabel = new QLabel(timeLabels[col]);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setStyleSheet("color: white; font-size: 10pt; padding: 5px; background-color: #333333; border: 1px solid #555555;");
        timeLabel->setMinimumHeight(25);
        m_heatMapLayout->addWidget(timeLabel, 0, col + 1);
    }

    // Add heat map cells with enhanced styling
    for (int row = 0; row < dateLabels.size(); ++row) {
        // Date label with consistent styling
        QLabel *dateLabel = new QLabel(dateLabels[row]);
        dateLabel->setAlignment(Qt::AlignCenter);
        dateLabel->setStyleSheet("color: white; font-size: 10pt; padding: 5px; background-color: #333333; border: 1px solid #555555;");
        dateLabel->setMinimumSize(80, 25);
        m_heatMapLayout->addWidget(dateLabel, row + 1, 0);

        // Heat map cells for each hour
        for (int col = 0; col < timeLabels.size(); ++col) {
            // Generate random emission data
            double emission = 50.0 + QRandomGenerator::global()->generateDouble() * (100.0 - 50.0);
            double power    = 800.0 + QRandomGenerator::global()->generateDouble() * (900.0 - 800.0);
            double saving   = 0.0 + QRandomGenerator::global()->generateDouble() * (15.0 - 0.0);

            HeatMapCell *cell = new HeatMapCell(timeLabels[col], dateLabels[row],
                                              emission, power, saving);
            cell->setMinimumSize(35, 25);
            cell->setStyleSheet("border: 1px solid #555555;");
            m_heatMapLayout->addWidget(cell, row + 1, col + 1);
        }
    }

    m_heatMapScrollArea->setWidget(heatMapWidget);
}

void HistoryPage::generateDummyChartData()
{
    m_chart->removeAllSeries();

    QString componentType = m_dcsComponentCombo->currentText();

    if (componentType.contains("Main Engine")) {
        // Create multiple series for different ME units
        for (int me = 1; me <= 3; ++me) {
            QLineSeries *series = new QLineSeries();
            series->setName(QString("ME-%1").arg(me));

            QDateTime baseTime = QDateTime::currentDateTime().addSecs(-3600); // Last hour
            for (int i = 0; i < 60; ++i) {
                QDateTime time = baseTime.addSecs(i * 60); // Every minute
                double temperature = 450 + me * 10 + QRandomGenerator::global()->bounded(-20, 20);
                series->append(time.toMSecsSinceEpoch(), temperature);
            }

            m_chart->addSeries(series);
        }
        m_chart->setTitle("Main Engine Temperature Monitoring");
    }
    else if (componentType.contains("Auxiliary Engine")) {
        QLineSeries *series = new QLineSeries();
        series->setName("AE-1");

        QDateTime baseTime = QDateTime::currentDateTime().addSecs(-3600);
        for (int i = 0; i < 60; ++i) {
            QDateTime time = baseTime.addSecs(i * 60);
            double temperature = 380 + QRandomGenerator::global()->bounded(-15, 15);
            series->append(time.toMSecsSinceEpoch(), temperature);
        }

        m_chart->addSeries(series);
        m_chart->setTitle("Auxiliary Engine Temperature Monitoring");
    }

    // Setup axes
    m_chart->createDefaultAxes();

    // Configure X-axis as DateTime
    QDateTimeAxis *xAxis = new QDateTimeAxis;
    xAxis->setFormat("hh:mm");
    xAxis->setTitleText("Time");
    xAxis->setTitleBrush(QBrush(Qt::white));
    xAxis->setLabelsBrush(QBrush(Qt::white));
    xAxis->setGridLineColor(QColor(100, 100, 100));

    // Configure Y-axis
    QValueAxis *yAxis = new QValueAxis;
    yAxis->setTitleText("Temperature (°C)");
    yAxis->setTitleBrush(QBrush(Qt::white));
    yAxis->setLabelsBrush(QBrush(Qt::white));
    yAxis->setGridLineColor(QColor(100, 100, 100));

    m_chart->setAxisX(xAxis, m_chart->series().first());
    m_chart->setAxisY(yAxis, m_chart->series().first());
}

void HistoryPage::onNavigationSelectionChanged()
{
    int currentIndex = ui->listWidget->currentRow();
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void HistoryPage::onDCSComponentChanged()
{
    generateDummyChartData();
}

void HistoryPage::onReportSelectionChanged()
{
    // For now, load a dummy PDF - you'll need to add actual PDFs to your resources
    QString reportPath = ":/reports/trial_cii_report.pdf";
    m_pdfDocument->load(reportPath);
}

void HistoryPage::updateDCSChart()
{
    // Add new data point to existing series
    for (QAbstractSeries *series : m_chart->series()) {
        QLineSeries *lineSeries = qobject_cast<QLineSeries*>(series);
        if (lineSeries) {
            // Remove oldest point if we have too many
            if (lineSeries->count() > 60) {
                lineSeries->remove(0);
            }

            // Add new point
            QDateTime currentTime = QDateTime::currentDateTime();
            QString seriesName = lineSeries->name();
            double baseTemp = seriesName.contains("ME-1") ? 460 :
                             seriesName.contains("ME-2") ? 470 :
                             seriesName.contains("ME-3") ? 480 : 380;
            double newTemp = baseTemp + QRandomGenerator::global()->bounded(-20, 20);

            lineSeries->append(currentTime.toMSecsSinceEpoch(), newTemp);
        }
    }
}

// HeatMapCell implementation
HeatMapCell::HeatMapCell(const QString &time, const QString &date,
                        double emission, double power, double saving, QWidget *parent)
    : QFrame(parent), m_time(time), m_date(date), m_emission(emission),
      m_power(power), m_saving(saving)
{
    setFrameStyle(QFrame::Box);
    setLineWidth(1);
    setCursor(Qt::PointingHandCursor);

    QString tooltip = QString("Time: %1\nDate: %2\nEmission: %3 MWh\nPower: %4 MW\nSaving: %5%")
                     .arg(m_time).arg(m_date).arg(m_emission, 0, 'f', 1)
                     .arg(m_power, 0, 'f', 1).arg(m_saving, 0, 'f', 1);
    setToolTip(tooltip);
}

void HeatMapCell::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor = getColorForEmission(m_emission);
    painter.fillRect(rect(), bgColor);

    QFrame::paintEvent(event);
}

void HeatMapCell::enterEvent(QEnterEvent *event)
{
    setStyleSheet("border: 2px solid #34A853;");
    QFrame::enterEvent(event);
}

void HeatMapCell::leaveEvent(QEvent *event)
{
    setStyleSheet("border: 1px solid #555555;");
    QFrame::leaveEvent(event);
}

void HeatMapCell::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        showDetails();
    }
    QFrame::mousePressEvent(event);
}

void HeatMapCell::showDetails()
{
    EmissionDetailsDialog dialog(m_time, m_date, m_emission, m_power, m_saving, this);
    dialog.exec();
}

QColor HeatMapCell::getColorForEmission(double emission)
{
    if (emission < 60) return QColor(46, 125, 50);      // Green (Low)
    else if (emission < 75) return QColor(251, 192, 45); // Yellow (Medium)
    else if (emission < 90) return QColor(245, 124, 0);  // Orange (High)
    else return QColor(211, 47, 47);                     // Red (Critical)
}

// EmissionDetailsDialog implementation
EmissionDetailsDialog::EmissionDetailsDialog(const QString &time, const QString &date,
                                           double emission, double power, double saving,
                                           QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Emission Details");
    setFixedSize(400, 300);
    setStyleSheet("background-color: #333333; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Carbon Emission Details");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(titleLabel);

    QTextEdit *detailsText = new QTextEdit;
    detailsText->setReadOnly(true);
    detailsText->setStyleSheet("background-color: #4F4F4F; border: 1px solid #666666; padding: 10px;");

    QString details = QString(
        "<b>Time:</b> %1<br>"
        "<b>Date:</b> %2<br><br>"
        "<b>Carbon Emission:</b> %3 MWh<br>"
        "<b>Power Consumption:</b> %4 MW<br>"
        "<b>Saving Impact:</b> %5%<br><br>"
        "<b>Planned Target:</b> 82.0 MWh<br>"
        "<b>Emission Limit:</b> 95.0 MWh<br>"
        "<b>Status:</b> %6"
    ).arg(time).arg(date)
     .arg(emission, 0, 'f', 1)
     .arg(power, 0, 'f', 1)
     .arg(saving, 0, 'f', 1)
     .arg(emission > 95 ? "<span style='color: #D32F2F;'>Over Limit</span>" :
          emission > 82 ? "<span style='color: #F57C00;'>Above Target</span>" :
          "<span style='color: #2E7D32;'>Within Target</span>");

    detailsText->setHtml(details);
    layout->addWidget(detailsText);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    buttonBox->setStyleSheet("QPushButton { background-color: #4F4F4F; color: white; border: 1px solid #666666; border-radius: 4px; padding: 6px 20px; }");
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(buttonBox);
}

#include "HistoryPage.moc"
