#include "DashboardPage.h"
#include "ui_DashboardPage.h"
#include "MapboxWidget.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

#include "CircleProgressBar.h"
#include "../../service/MockApiService.h"

#include "AlertAndRecomendationFrame.h"
#include "KPIOverviewFrame.h"
#include "WeatherFrame.h"

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardPage)
    , m_mapboxWidget(nullptr)
    , m_routeSegmentIndex(0)
    , m_shipProgress(0.0)
    , m_shipUpdateTimer(nullptr)
    , m_currentRouteIndex(0)
{
    ui->setupUi(this);

    // Setup layouts
    QVBoxLayout *mapLayout = new QVBoxLayout(ui->widgetMap);

    // Create MapboxWidget
    m_mapboxWidget = new MapboxWidget(ui->widgetMap);
    mapLayout->addWidget(m_mapboxWidget);

    qDebug() << "MapboxWidget created, connecting signals...";

    // Connect signal untuk menunggu map ready
    connect(m_mapboxWidget, &MapboxWidget::mapReady, this, [this](){
        qDebug() << "=== MAP READY SIGNAL RECEIVED ===";

        // Load icon first
        m_mapboxWidget->loadIconFromResource("ship-icon", ":/icons/ship-icon.png");
    });

    // Connect signal untuk icon loaded
    connect(m_mapboxWidget, &MapboxWidget::iconLoaded, this, [this](const QString &iconName){
        qDebug() << "=== ICON LOADED SIGNAL RECEIVED ===" << iconName;

        // Small delay then setup route
        QTimer::singleShot(300, this, &DashboardPage::setupInitialMapRoute);
    });

    // Connect signal untuk konfirmasi route displayed
    connect(m_mapboxWidget, &MapboxWidget::routeDisplayed, this, [this](){
        qDebug() << "=== ROUTE DISPLAYED SIGNAL RECEIVED ===";

        // Setup ship movement simulation
        QTimer::singleShot(1000, this, &DashboardPage::setupShipMovementSimulation);
    });


    connect(MockApiService::instance(), &MockApiService::dataUpdated,
            this, &DashboardPage::onDataUpdated);

    MockApiService::instance()->startPolling(1, 5000);


    // KPI Animation Timer
    // QTimer *kpiTimer = new QTimer(this);
    // connect(kpiTimer, &QTimer::timeout, [=](){
    //     auto animate = [](CircleProgressBar *bar) {
    //         int newValue = rand() % 100;
    //         QPropertyAnimation *anim = new QPropertyAnimation(bar, "value");
    //         anim->setDuration(1000);
    //         anim->setStartValue(bar->value());
    //         anim->setEndValue(newValue);
    //         anim->setEasingCurve(QEasingCurve::InOutCubic);
    //         anim->start(QAbstractAnimation::DeleteWhenStopped);
    //     };
    //     animate(circleBar1);
    // });
    // kpiTimer->start(3000);

    qDebug() << "DashboardPage constructor completed";

    // Initialize ship simulation variables
    m_currentRouteIndex = 0;
    m_shipProgress = 0.0;

    // Define the route coordinates for ship movement
    m_routeCoordinates.clear();
    m_routeCoordinates << QPointF(112.73, -7.20)   // Surabaya
                       << QPointF(112.0, -5.5)     // Waypoint 1
                       << QPointF(108.5, -3.0)     // Waypoint 2
                       << QPointF(105.5, 0.0)      // Waypoint 3
                       << QPointF(103.82, 1.26);   // Singapore

    createWidgetFrameOverlay();
    positionOverlay();
}

void DashboardPage::setupInitialMapRoute()
{
    if (!m_mapboxWidget) {
        qWarning() << "MapboxWidget is null in setupInitialMapRoute!";
        return;
    }

    qDebug() << "=== SETTING UP INITIAL MAP ROUTE ===";

    // DEBUGGING: Test QVariantList creation
    QVariantList testCoord;
    testCoord.append(112.73);
    testCoord.append(-7.20);

    // PERBAIKAN: Create route coordinates dengan method berbeda
    QVariantList routePoints;

    // Method 1: Using append()
    QVariantList coord1;
    coord1.append(QVariant(112.73));
    coord1.append(QVariant(-7.20));
    routePoints.append(QVariant(coord1));
    qDebug() << "Coord1 created:" << coord1 << "Added to routePoints, size now:" << routePoints.size();

    QVariantList coord2;
    coord2.append(QVariant(112.0));
    coord2.append(QVariant(-5.5));
    routePoints.append(QVariant(coord2));

    QVariantList coord3;
    coord3.append(QVariant(108.5));
    coord3.append(QVariant(-3.0));
    routePoints.append(QVariant(coord3));

    QVariantList coord4;
    coord4.append(QVariant(105.5));
    coord4.append(QVariant(0.0));
    routePoints.append(QVariant(coord4));

    QVariantList coord5;
    coord5.append(QVariant(103.82));
    coord5.append(QVariant(1.26));
    routePoints.append(QVariant(coord5));

    qDebug() << "Final routePoints size:" << routePoints.size();
    qDebug() << "First element type:" << routePoints[0].type() << "Value:" << routePoints[0];

    // Define markers dengan cara yang sama
    QVariantList markers;

    QVariantMap surabayaMarker;
    QVariantList surabayaCoords;
    surabayaCoords.append(QVariant(112.73));
    surabayaCoords.append(QVariant(-7.20));
    surabayaMarker["coords"] = QVariant(surabayaCoords);
    surabayaMarker["title"] = QVariant(QString("Pelabuhan Surabaya"));
    surabayaMarker["color"] = QVariant(QString("#FF4500"));
    markers.append(QVariant(surabayaMarker));

    QVariantMap singaporeMarker;
    QVariantList singaporeCoords;
    singaporeCoords.append(QVariant(103.82));
    singaporeCoords.append(QVariant(1.26));
    singaporeMarker["coords"] = QVariant(singaporeCoords);
    singaporeMarker["title"] = QVariant(QString("Pelabuhan Singapura"));
    singaporeMarker["color"] = QVariant(QString("#32CD32"));
    markers.append(QVariant(singaporeMarker));

    // Debug output yang lebih sederhana
    qDebug() << "Route points count:" << routePoints.size();
    qDebug() << "Markers count:" << markers.size();

    // Test conversion SEBELUM mengirim
    for (int i = 0; i < qMin(routePoints.size(), 3); ++i) { // Only first 3 for debugging
        QVariant item = routePoints[i];
        qDebug() << "Item" << i << "type:" << item.type() << "canConvert to List:" << item.canConvert<QVariantList>();
        if (item.canConvert<QVariantList>()) {
            QVariantList coord = item.toList();
            qDebug() << "  Successfully converted. Size:" << coord.size();
            if (coord.size() >= 2) {
                qDebug() << "  Coord values:" << coord[0].toDouble() << coord[1].toDouble();
            }
        }
    }

    // Send to mapbox
    qDebug() << "Sending data to MapboxWidget...";
    m_mapboxWidget->setSeaRoute(routePoints, markers);
    qDebug() << "Route data sent to MapboxWidget";
}

void DashboardPage::setupShipMovementSimulation()
{
    qDebug() << "Setting up ship movement simulation...";

    // Create timer for ship position updates (update every second)
    m_shipUpdateTimer = new QTimer(this);
    connect(m_shipUpdateTimer, &QTimer::timeout, this, &DashboardPage::updateShipPosition);

    // Start the simulation after a small delay to ensure map is ready
    QTimer::singleShot(1000, this, [this](){
        qDebug() << "Starting ship movement simulation";
        m_shipUpdateTimer->start(1000); // Update every 1000ms = 1 second
    });
}

void DashboardPage::updateShipPosition()
{
    if (!m_mapboxWidget || m_routeCoordinates.empty()) {
        return;
    }

    // Check if we've reached the end of the route
    if (m_currentRouteIndex >= m_routeCoordinates.size() - 1) {
        qDebug() << "Ship reached destination, stopping simulation";
        m_shipUpdateTimer->stop();
        return;
    }

    // Get current and next waypoints
    QPointF currentPoint = m_routeCoordinates[m_currentRouteIndex];
    QPointF nextPoint = m_routeCoordinates[m_currentRouteIndex + 1];

    // Calculate interpolated position
    double lat = currentPoint.y() + (nextPoint.y() - currentPoint.y()) * m_shipProgress;
    double lon = currentPoint.x() + (nextPoint.x() - currentPoint.x()) * m_shipProgress;

    // Calculate bearing (direction the ship is facing)
    double bearing = calculateBearing(currentPoint, nextPoint);

    // Update ship position on map
    m_mapboxWidget->setShipPosition(lon, lat, bearing);

    // Debug output (every 10 updates to avoid spam)
    static int updateCount = 0;
    if (++updateCount % 10 == 0) {
        qDebug() << "Ship position updated:" << QString("Segment %1, Progress %2%, Position [%3, %4], Bearing %5°")
                                                    .arg(m_currentRouteIndex)
                                                    .arg(m_shipProgress * 100, 0, 'f', 1)
                                                    .arg(lon, 0, 'f', 4)
                                                    .arg(lat, 0, 'f', 4)
                                                    .arg(bearing, 0, 'f', 1);
    }

    // Increment progress
    m_shipProgress += 0.02; // Move 2% of segment per second (50 seconds per segment)

    // Move to next segment when current is complete
    if (m_shipProgress >= 1.0) {
        m_currentRouteIndex++;
        m_shipProgress = 0.0;
        qDebug() << "Ship moving to next segment:" << m_currentRouteIndex;
    }
}

void DashboardPage::setupIcon()
{
    QString iconFuel = ":/icons/ribbon/gas-station.png";
    QString iconEngine = ":/icons/ribbon/engine.png";
    QString iconCII = ":/icons/ribbon/co2.png";
    QString iconWind = ":/icons/ribbon/wind.png";
    QString iconHumidity = ":/icons/ribbon/humidity.png";
    QString iconTemp = ":/icons/ribbon/thermometer.png";
    QString iconPressure = ":/icons/ribbon/barometer.png";


    // // Setup to Graphics View
    // // Fuel
    QGraphicsScene *sceneFuel = new QGraphicsScene(this);
    sceneFuel->addPixmap(QPixmap(iconFuel).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_KPIOverviewFrame->graphicsViewFOConsumption()->setScene(sceneFuel);
    m_KPIOverviewFrame->graphicsViewFOConsumption()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewFuel->setScene(sceneFuel);
    // ui->graphicsViewFuel->setStyleSheet("background: transparent; border: none;");

    // // Engine
    QGraphicsScene *sceneEngine = new QGraphicsScene(this);
    sceneEngine->addPixmap(QPixmap(iconEngine).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_KPIOverviewFrame->graphicsViewEnergyEfficiency()->setScene(sceneEngine);
    m_KPIOverviewFrame->graphicsViewEnergyEfficiency()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewEngine->setScene(sceneEngine);
    // ui->graphicsViewEngine->setStyleSheet("background: transparent; border: none;");

    // // CII
    QGraphicsScene *sceneCII = new QGraphicsScene(this);
    sceneCII->addPixmap(QPixmap(iconCII).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_KPIOverviewFrame->graphicsViewVoyageCII()->setScene(sceneCII);
    m_KPIOverviewFrame->graphicsViewVoyageCII()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewCII->setScene(sceneCII);
    // ui->graphicsViewCII->setStyleSheet("background: transparent; border: none;");

    // // Wind
    QGraphicsScene *sceneWind = new QGraphicsScene(this);
    sceneWind->addPixmap(QPixmap(iconWind).scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_WeatherFrame->graphicsViewWindSpeed()->setScene(sceneWind);
    m_WeatherFrame->graphicsViewWindSpeed()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewWind->setScene(sceneWind);
    // ui->graphicsViewWind->setStyleSheet("background: transparent; border: none;");

    // // Humidity
    QGraphicsScene *sceneHumidity = new QGraphicsScene(this);
    sceneHumidity->addPixmap(QPixmap(iconHumidity).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_WeatherFrame->graphicsViewHumidity()->setScene(sceneHumidity);
    m_WeatherFrame->graphicsViewHumidity()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewHumidity->setScene(sceneHumidity);
    // ui->graphicsViewHumidity->setStyleSheet("background: transparent; border: none;");

    // // Temp
    QGraphicsScene *sceneTemp = new QGraphicsScene(this);
    sceneTemp->addPixmap(QPixmap(iconTemp).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_WeatherFrame->graphicsViewTemperature()->setScene(sceneTemp);
    m_WeatherFrame->graphicsViewTemperature()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewTemperature->setScene(sceneTemp);
    // ui->graphicsViewTemperature->setStyleSheet("background: transparent; border: none;");

    // // Pressure
    QGraphicsScene *scenePressure = new QGraphicsScene(this);
    scenePressure->addPixmap(QPixmap(iconPressure).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_WeatherFrame->graphicsViewPressure()->setScene(scenePressure);
    m_WeatherFrame->graphicsViewPressure()->setStyleSheet("background: transparent; border: none;");
    // ui->graphicsViewPressure->setScene(scenePressure);
    // ui->graphicsViewPressure->setStyleSheet("background: transparent; border: none;");
}

void DashboardPage::onDataUpdated(const VoyageLogs &data)
{
    // Print received data for debugging

    // m_KPIOverviewFrame->labelFOConsumption()->setText(QString::number(data.propulsion_logs[0].fuel_consumption_rate, 'f', 2));
    // m_KPIOverviewFrame->labelEnergyEfficiency()->setText(QString::number(data.propulsion_logs[0].engine_load, 'f', 2));
    m_KPIOverviewFrame->progressBarFOConsumption()->setValue(static_cast<int>(data.propulsion_logs[0].fuel_consumption_rate));
    m_KPIOverviewFrame->progressBarEnergyEfficiency()->setValue(static_cast<int>(data.propulsion_logs[0].engine_load));
    m_WeatherFrame->labelWindSpeedValue()->setText(QString::number(data.wind_speed, 'i', 0));

    // ui->fuelConsumption->setText(QString::number(data.propulsion_logs[0].fuel_consumption_rate));
    // ui->engineEfficiencyPercentage->setText(QString::number(data.propulsion_logs[0].engine_load));
    // ui->windSpeed->setText(QString::number(data.wind_speed));
}

void DashboardPage::createWidgetFrameOverlay()
{
    m_AlertAndRecomendationFrame = new AlertAndRecomendationFrame(this);
    m_KPIOverviewFrame = new KPIOverviewFrame(this);
    m_WeatherFrame = new WeatherFrame(this);

    m_AlertAndRecomendationFrame->hide();
    m_KPIOverviewFrame->hide();
    m_WeatherFrame->hide();

    // Style for alert list
    QString styleSheet = R"(
        QListWidget {
            background-color: #333333;
            border: none;
            outline: 0;
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
    )";

    m_AlertAndRecomendationFrame->listWidget()->setStyleSheet(styleSheet);
    m_AlertAndRecomendationFrame->listWidget()->setSelectionMode(QAbstractItemView::NoSelection);

    addAlertRecomendation();

    setupIcon();
}

double DashboardPage::calculateBearing(const QPointF &from, const QPointF &to)
{
    // Convert to radians
    double lat1 = from.y() * M_PI / 180.0;
    double lat2 = to.y() * M_PI / 180.0;
    double deltaLon = (to.x() - from.x()) * M_PI / 180.0;

    // Calculate bearing
    double y = sin(deltaLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(deltaLon);
    double bearing = atan2(y, x);

    // Convert to degrees and normalize to 0-360
    bearing = bearing * 180.0 / M_PI;
    if (bearing < 0) {
        bearing += 360.0;
    }

    return bearing;
}

void DashboardPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_mapboxWidget->setGeometry(0, 0, width(), height()); // resize map too
    positionOverlay();
}

void DashboardPage::positionOverlay()
{
    // m_AlertAndRecomendationFrame widget overlay (top-left)
    int margin = 9; // distance from edge
    int x = margin;
    int y = margin;
    m_AlertAndRecomendationFrame->move(x, y);

    // m_WeatherFrame widget overlay (bottom-center)
    x = (width() - m_WeatherFrame->width()) / 2;
    y = height() - m_WeatherFrame->height() - margin;
    m_WeatherFrame->move(x, y);

    // m_KPIOverviewFrame widget overlay (top left under m_AlertAndRecommendationFrame)
    x = margin;
    y = m_AlertAndRecomendationFrame->y() + m_AlertAndRecomendationFrame->height() + margin;
    m_KPIOverviewFrame->move(x, y);
}

// Function untuk start/stop simulation (bisa dipanggil dari UI)
void DashboardPage::startShipSimulation()
{
    if (m_shipUpdateTimer && !m_shipUpdateTimer->isActive()) {
        m_currentRouteIndex = 0;
        m_shipProgress = 0.0;
        m_shipUpdateTimer->start(1000);
        qDebug() << "Ship simulation started";
    }
}

void DashboardPage::stopShipSimulation()
{
    if (m_shipUpdateTimer && m_shipUpdateTimer->isActive()) {
        m_shipUpdateTimer->stop();
        qDebug() << "Ship simulation stopped";
    }
}

void DashboardPage::resetShipSimulation()
{
    stopShipSimulation();
    m_currentRouteIndex = 0;
    m_shipProgress = 0.0;

    // Reset ship to starting position
    if (m_mapboxWidget && !m_routeCoordinates.empty()) {
        QPointF startPoint = m_routeCoordinates.first();
        m_mapboxWidget->setShipPosition(startPoint.x(), startPoint.y(), 0.0);
        qDebug() << "Ship simulation reset to starting position";
    }
}

DashboardPage::~DashboardPage()
{
    if (m_shipUpdateTimer) {
        m_shipUpdateTimer->stop();
    }
    delete ui;
}

void DashboardPage::addAlertRecomendation()
{
    m_AlertAndRecomendationFrame->listWidget()->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/alert.png"), "Alert: Low-speed"));
}

void DashboardPage::setCurrentMapStyle(int index)
{

}

void DashboardPage::WeatherToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    // Clean up existing dialog if it exists
    if (m_weatherHintBox) {
        m_weatherHintBox->deleteLater();
        m_weatherHintBox = nullptr;
    }

    // Always create a new instance
    m_weatherHintBox = new HintboxDialog(this);
    m_weatherHintBox->setTitle("Weather Feature");

    // Make dialog modal and uninterruptable
    m_weatherHintBox->setModal(true);
    m_weatherHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    // Set dialog title (if you have a method to set title)
    // m_weatherHintBox->setWindowTitle("Weather Map Guide");

    // Page 1: Understanding Wind Direction Basics
    QString page1Description =
        "Wind direction on weather maps indicates where the wind is coming FROM, not where it's going. "
        "For example, a 'north wind' means the wind is blowing FROM the north TO the south. "
        "\n\n"
        "Wind direction is typically shown using arrows or barbs on weather maps. The arrow points "
        "in the direction the wind is blowing TO, while the wind name describes where it comes FROM. "
        "\n\n"
        "Understanding wind direction helps predict weather changes, as winds often bring the weather "
        "conditions from their source region.";

    m_weatherHintBox->addNewPage(":/hintboxes/weather.jpg", page1Description);

    // Page 2: Reading Wind Patterns and Symbols
    QString page2Description =
        "Weather maps use various symbols to show wind information: "
        "\n\n"
        "• Wind Barbs: Show both direction and speed. The barb points into the wind direction, "
        "with flags and lines indicating speed (each full barb = 10 knots, half barb = 5 knots). "
        "\n\n"
        "• Arrows: Simple directional indicators showing wind flow patterns across regions. "
        "\n\n"
        "• Isobars: Lines of equal pressure that help identify wind patterns - wind flows parallel "
        "to isobars, with low pressure to the left in the Northern Hemisphere. "
        "\n\n"
        "Pay attention to wind convergence and divergence zones, as these often indicate areas "
        "of weather activity and potential storm development.";

    m_weatherHintBox->addNewPage(":/hintboxes/weather.jpg", page2Description);

    // Apply dark theme styling
    m_weatherHintBox->applyStylesheet_dark();

    // Connect to accepted signal (OK button clicked) for cleanup
    connect(m_weatherHintBox, &QDialog::accepted, this, [this]() {
        if (m_weatherHintBox) {
            m_weatherHintBox->deleteLater();
            m_weatherHintBox = nullptr;
        }
    });

    // Also connect to rejected signal (in case ESC is pressed, though it shouldn't work with modal)
    connect(m_weatherHintBox, &QDialog::rejected, this, [this]() {
        if (m_weatherHintBox) {
            m_weatherHintBox->deleteLater();
            m_weatherHintBox = nullptr;
        }
    });

    // Show the modal dialog
    m_weatherHintBox->exec();
}

// ======================= BATHYMETER =======================
void DashboardPage::BathymeterTiggkeAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_bathymeterHintBox) {
        m_bathymeterHintBox->deleteLater();
        m_bathymeterHintBox = nullptr;
    }

    m_bathymeterHintBox = new HintboxDialog(this);
    m_bathymeterHintBox->setTitle("Bathymeter Feature");
    m_bathymeterHintBox->setModal(true);
    m_bathymeterHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "Bathymetry is the study of underwater depth of ocean floors, lakes, and rivers. "
        "It provides essential data for safe navigation, identifying hazards, and planning routes.";
    m_bathymeterHintBox->addNewPage(":/hintboxes/bathymeter.jpeg", page1Description);

    QString page2Description =
        "Bathymetric maps use contour lines, colors, and soundings to represent depth. "
        "Darker shades = deeper areas, lighter = shallower. "
        "Contour lines show seafloor shape, while soundings mark exact depth values.";
    m_bathymeterHintBox->addNewPage(":/hintboxes/bathymeter.jpeg", page2Description);

    m_bathymeterHintBox->applyStylesheet_dark();

    connect(m_bathymeterHintBox, &QDialog::accepted, this, [this]() {
        m_bathymeterHintBox->deleteLater(); m_bathymeterHintBox = nullptr;
    });
    connect(m_bathymeterHintBox, &QDialog::rejected, this, [this]() {
        m_bathymeterHintBox->deleteLater(); m_bathymeterHintBox = nullptr;
    });

    m_bathymeterHintBox->exec();
}


// ======================= PORTS =======================
void DashboardPage::PortsToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_portsHintBox) {
        m_portsHintBox->deleteLater();
        m_portsHintBox = nullptr;
    }

    m_portsHintBox = new HintboxDialog(this);
    m_portsHintBox->setTitle("Ports Feature");
    m_portsHintBox->setModal(true);
    m_portsHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "Port information layers highlight key harbors, terminals, and docking facilities. "
        "They provide navigators with details about available services, capacity, and restrictions.";
    m_portsHintBox->addNewPage(":/hintboxes/port.png", page1Description);

    QString page2Description =
        "Ports are marked with symbols and identifiers. "
        "Zooming in reveals detailed layouts, entry channels, anchorage points, and pilot boarding areas.";
    m_portsHintBox->addNewPage(":/hintboxes/port.png", page2Description);

    m_portsHintBox->applyStylesheet_dark();

    connect(m_portsHintBox, &QDialog::accepted, this, [this]() {
        m_portsHintBox->deleteLater(); m_portsHintBox = nullptr;
    });
    connect(m_portsHintBox, &QDialog::rejected, this, [this]() {
        m_portsHintBox->deleteLater(); m_portsHintBox = nullptr;
    });

    m_portsHintBox->exec();
}


// ======================= OCEAN ROUTE =======================
void DashboardPage::OceanRouteToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_oceanRouteHintBox) {
        m_oceanRouteHintBox->deleteLater();
        m_oceanRouteHintBox = nullptr;
    }

    m_oceanRouteHintBox = new HintboxDialog(this);
    m_oceanRouteHintBox->setTitle("Ocean Route Feature");
    m_oceanRouteHintBox->setModal(true);
    m_oceanRouteHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "Ocean Routes are recommended navigation paths across seas and oceans. "
        "They are designed to optimize safety, distance, and fuel efficiency.";
    m_oceanRouteHintBox->addNewPage(":/hintboxes/searoute.png", page1Description);

    QString page2Description =
        "These routes consider currents, prevailing winds, restricted zones, and seasonal conditions. "
        "They help vessels avoid unnecessary risks and improve voyage planning.";
    m_oceanRouteHintBox->addNewPage(":/hintboxes/searoute.png", page2Description);

    m_oceanRouteHintBox->applyStylesheet_dark();

    connect(m_oceanRouteHintBox, &QDialog::accepted, this, [this]() {
        m_oceanRouteHintBox->deleteLater(); m_oceanRouteHintBox = nullptr;
    });
    connect(m_oceanRouteHintBox, &QDialog::rejected, this, [this]() {
        m_oceanRouteHintBox->deleteLater(); m_oceanRouteHintBox = nullptr;
    });

    m_oceanRouteHintBox->exec();
}


// ======================= ECA =======================
void DashboardPage::ECAToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_ecaHintBox) {
        m_ecaHintBox->deleteLater();
        m_ecaHintBox = nullptr;
    }

    m_ecaHintBox = new HintboxDialog(this);
    m_ecaHintBox->setTitle("Emission Control Area (ECA)");
    m_ecaHintBox->setModal(true);
    m_ecaHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "ECA zones are maritime regions where stricter controls on emissions apply. "
        "They are established to reduce air pollution from ships, particularly SOx and NOx.";
    m_ecaHintBox->addNewPage(":/hintboxes/emission.png", page1Description);

    QString page2Description =
        "When entering ECAs, vessels must switch to compliant fuel or apply emission reduction technology. "
        "Planning routes around ECAs helps manage costs and ensures regulatory compliance.";
    m_ecaHintBox->addNewPage(":/hintboxes/emission.png", page2Description);

    m_ecaHintBox->applyStylesheet_dark();

    connect(m_ecaHintBox, &QDialog::accepted, this, [this]() {
        m_ecaHintBox->deleteLater(); m_ecaHintBox = nullptr;
    });
    connect(m_ecaHintBox, &QDialog::rejected, this, [this]() {
        m_ecaHintBox->deleteLater(); m_ecaHintBox = nullptr;
    });

    m_ecaHintBox->exec();
}


// ======================= PIRACY ZONE =======================
void DashboardPage::PiracyZoneToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_piracyZoneHintBox) {
        m_piracyZoneHintBox->deleteLater();
        m_piracyZoneHintBox = nullptr;
    }

    m_piracyZoneHintBox = new HintboxDialog(this);
    m_piracyZoneHintBox->setTitle("Piracy Zone Feature");
    m_piracyZoneHintBox->setModal(true);
    m_piracyZoneHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "Piracy Zones highlight regions where piracy incidents are more frequent. "
        "Awareness of these areas is critical for vessel security and crew safety.";
    m_piracyZoneHintBox->addNewPage(":/hintboxes/piracy-zone.jpg", page1Description);

    QString page2Description =
        "Ships transiting piracy-prone waters often adopt security measures, such as higher speed, "
        "armed guards, or rerouting. Navigation systems mark these zones for risk assessment.";
    m_piracyZoneHintBox->addNewPage(":/hintboxes/piracy-zone.jpg", page2Description);

    m_piracyZoneHintBox->applyStylesheet_dark();

    connect(m_piracyZoneHintBox, &QDialog::accepted, this, [this]() {
        m_piracyZoneHintBox->deleteLater(); m_piracyZoneHintBox = nullptr;
    });
    connect(m_piracyZoneHintBox, &QDialog::rejected, this, [this]() {
        m_piracyZoneHintBox->deleteLater(); m_piracyZoneHintBox = nullptr;
    });

    m_piracyZoneHintBox->exec();
}


// ======================= PLANNED ROUTE =======================
void DashboardPage::PlannedRouteToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_plannedRouteHintBox) {
        m_plannedRouteHintBox->deleteLater();
        m_plannedRouteHintBox = nullptr;
    }

    m_plannedRouteHintBox = new HintboxDialog(this);
    m_plannedRouteHintBox->setTitle("Planned Route Feature");
    m_plannedRouteHintBox->setModal(true);
    m_plannedRouteHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "Planned Route shows the intended voyage path prepared before departure. "
        "It considers ports of call, safety zones, weather, and regulations.";
    m_plannedRouteHintBox->addNewPage(":/hintboxes/searoute.png", page1Description);

    QString page2Description =
        "The planned route serves as a reference for navigators and is compared against the actual route. "
        "It helps ensure compliance with voyage planning standards (IMO/ISM).";
    m_plannedRouteHintBox->addNewPage(":/hintboxes/searoute.png", page2Description);

    m_plannedRouteHintBox->applyStylesheet_dark();

    connect(m_plannedRouteHintBox, &QDialog::accepted, this, [this]() {
        m_plannedRouteHintBox->deleteLater(); m_plannedRouteHintBox = nullptr;
    });
    connect(m_plannedRouteHintBox, &QDialog::rejected, this, [this]() {
        m_plannedRouteHintBox->deleteLater(); m_plannedRouteHintBox = nullptr;
    });

    m_plannedRouteHintBox->exec();
}


// ======================= ACTUAL ROUTE =======================
void DashboardPage::ActualRouteToggleAction_toggled()
{
    qInfo() << __FUNCSIG__ << __LINE__;

    if (m_actualRouteHintBox) {
        m_actualRouteHintBox->deleteLater();
        m_actualRouteHintBox = nullptr;
    }

    m_actualRouteHintBox = new HintboxDialog(this);
    m_actualRouteHintBox->setTitle("Actual Route Feature");
    m_actualRouteHintBox->setModal(true);
    m_actualRouteHintBox->setWindowFlag(Qt::WindowCloseButtonHint, false);

    QString page1Description =
        "The Actual Route represents the real track of the vessel recorded during the voyage. "
        "It often differs from the planned route due to weather, traffic, or operational decisions.";
    m_actualRouteHintBox->addNewPage(":/hintboxes/searoute.png", page1Description);

    QString page2Description =
        "Comparing the actual route with the planned route helps identify deviations, "
        "optimize future voyages, and ensure safety compliance.";
    m_actualRouteHintBox->addNewPage(":/hintboxes/searoute.png", page2Description);

    m_actualRouteHintBox->applyStylesheet_dark();

    connect(m_actualRouteHintBox, &QDialog::accepted, this, [this]() {
        m_actualRouteHintBox->deleteLater(); m_actualRouteHintBox = nullptr;
    });
    connect(m_actualRouteHintBox, &QDialog::rejected, this, [this]() {
        m_actualRouteHintBox->deleteLater(); m_actualRouteHintBox = nullptr;
    });

    m_actualRouteHintBox->exec();
}


void DashboardPage::AlretAndRecomendationFrame_toggled(bool checked)
{
    if (!m_AlertAndRecomendationFrame) return;

    if (checked) {
        m_AlertAndRecomendationFrame->show();
    } else {
        m_AlertAndRecomendationFrame->hide();
    }
}

void DashboardPage::KPIOverviewFrame_toggled(bool checked)
{
    if (!m_KPIOverviewFrame) return;

    if (checked) {
        m_KPIOverviewFrame->show();
    } else {
        m_KPIOverviewFrame->hide();
    }
}

void DashboardPage::WeatherFrame_toggled(bool checked)
{
    if (!m_WeatherFrame) return;

    if (checked) {
        m_WeatherFrame->show();
    } else {
        m_WeatherFrame->hide();
    }
}

void DashboardPage::RefreshMapToggleAction_toggled(bool checked)
{
    if (m_mapboxWidget) {
        m_mapboxWidget->reloadMap();
    }
}

void DashboardPage::ReturnToggleAction_toggled(bool checked)
{
    if (m_mapboxWidget) {
        m_mapboxWidget->returnToInitialView();
    }
}

void DashboardPage::ThreeDimensionMapToggleAction(bool checked)
{
    QMessageBox::information(this, "Preview", "This button is a preview-only");
}

void DashboardPage::TwoDimensionMapToggleAction(bool checked)
{
    QMessageBox::information(this, "Preview", "This button is a preview-only");
}
