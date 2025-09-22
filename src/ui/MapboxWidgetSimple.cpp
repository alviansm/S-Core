// MapboxWidgetSimple.cpp
#include "MapboxWidgetSimple.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

MapboxWidgetSimple::MapboxWidgetSimple(QWidget *parent)
    : QWidget(parent)
    , m_view(nullptr)
    , m_isPageLoaded(false)
    , m_isMapReady(false)
    , m_hasPendingData(false)
{
    // Create web view
    m_view = new QWebEngineView(this);
    connect(m_view->page(), &QWebEnginePage::loadFinished, this, &MapboxWidgetSimple::onPageLoaded);

    // Load the simple HTML file
    m_view->setUrl(QUrl("qrc:/html/map-simple.html"));

    // Setup layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);
    setLayout(mainLayout);

    // Setup timer for checking map readiness
    m_mapReadyTimer = new QTimer(this);
    m_mapReadyTimer->setSingleShot(true);
    connect(m_mapReadyTimer, &QTimer::timeout, this, &MapboxWidgetSimple::checkMapReady);

    qDebug() << "MapboxWidgetSimple created";
}

MapboxWidgetSimple::~MapboxWidgetSimple()
{
    if (m_mapReadyTimer) {
        m_mapReadyTimer->stop();
    }
}

void MapboxWidgetSimple::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void MapboxWidgetSimple::onPageLoaded(bool ok)
{
    qDebug() << "Simple map page loaded:" << ok;

    if (ok) {
        m_isPageLoaded = true;
        // Start checking if map is ready
        m_mapReadyTimer->start(100);
    } else {
        qWarning() << "Failed to load simple map HTML page";
    }
}

void MapboxWidgetSimple::checkMapReady()
{
    if (!m_isPageLoaded) {
        m_mapReadyTimer->start(500);
        return;
    }

    QString script = R"(
        (function() {
            try {
                if (typeof map !== 'undefined' &&
                    map.isStyleLoaded &&
                    map.isStyleLoaded()) {
                    return 'MAP_READY';
                } else {
                    return 'MAP_NOT_READY';
                }
            } catch(e) {
                return 'MAP_ERROR: ' + e.message;
            }
        })();
    )";

    m_view->page()->runJavaScript(script, [this](const QVariant &result) {
        QString resultStr = result.toString();

        if (resultStr == "MAP_READY") {
            m_isMapReady = true;
            qDebug() << "Simple map is ready!";
            emit mapReady();
            processPendingData();
        } else if (resultStr.startsWith("MAP_ERROR")) {
            qWarning() << "Simple map error:" << resultStr;
            m_mapReadyTimer->start(2000);
        } else {
            // Map not ready yet, try again
            m_mapReadyTimer->start(500);
        }
    });
}

void MapboxWidgetSimple::processPendingData()
{
    if (m_hasPendingData) {
        qDebug() << "Processing pending simple map data";
        setSeaRoute(m_pendingRouteData, m_pendingMarkerData);
        m_hasPendingData = false;
    }
}

void MapboxWidgetSimple::setSeaRoute(const QVariantList &routeCoordinates, const QVariantList &markerData)
{
    qDebug() << "setSeaRoute called - Simple map ready:" << m_isMapReady;

    if (!m_isPageLoaded || !m_isMapReady) {
        // Store data for later processing
        qDebug() << "Storing simple route data for later processing";
        m_pendingRouteData = routeCoordinates;
        m_pendingMarkerData = markerData;
        m_hasPendingData = true;
        return;
    }

    // Clear pending data flag since we're processing now
    m_hasPendingData = false;

    // Convert to JSON
    QJsonArray routeJson = QJsonArray::fromVariantList(routeCoordinates);
    QJsonArray markersJson = QJsonArray::fromVariantList(markerData);

    QString routeString = QString::fromUtf8(QJsonDocument(routeJson).toJson(QJsonDocument::Compact));
    QString markersString = QString::fromUtf8(QJsonDocument(markersJson).toJson(QJsonDocument::Compact));

    // Create JavaScript command
    QString script = QString("updateRoute(%1, %2);").arg(routeString).arg(markersString);
    qDebug() << "Executing simple route script";

    // Execute JavaScript with callback
    m_view->page()->runJavaScript(script, [this](const QVariant &result) {
        qDebug() << "Simple route update result:" << result;
        emit routeDisplayed();
    });
}
