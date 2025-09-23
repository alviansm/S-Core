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
    , m_pendingOriginPortId(-1)
    , m_pendingDestPortId(-1)
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

    // Setup JavaScript callback for route displayed event
    connect(m_view->page(), &QWebEnginePage::loadFinished, this, [this](bool ok) {
        if (ok) {
            QString script = R"(
                window.routeDisplayed = function() {
                    // This will be called from JavaScript when route is successfully displayed
                    console.log('Route displayed callback triggered');
                };
            )";
            m_view->page()->runJavaScript(script);
        }
    });

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
    if (m_hasPendingData && m_pendingOriginPortId > 0 && m_pendingDestPortId > 0) {
        qDebug() << "Processing pending simple map data - Origin:" << m_pendingOriginPortId << "Dest:" << m_pendingDestPortId;
        setSeaRouteByPorts(m_pendingOriginPortId, m_pendingDestPortId);
        m_hasPendingData = false;
    }
}

void MapboxWidgetSimple::setSeaRouteByPorts(int originPortId, int destPortId)
{
    qDebug() << "setSeaRouteByPorts called - Origin:" << originPortId << "Dest:" << destPortId << "Map ready:" << m_isMapReady;

    if (originPortId <= 0 || destPortId <= 0) {
        qWarning() << "Invalid port IDs provided - Origin:" << originPortId << "Dest:" << destPortId;
        return;
    }

    if (!m_isPageLoaded || !m_isMapReady) {
        // Store data for later processing
        qDebug() << "Storing port IDs for later processing";
        m_pendingOriginPortId = originPortId;
        m_pendingDestPortId = destPortId;
        m_hasPendingData = true;
        return;
    }

    // Clear pending data flag since we're processing now
    m_hasPendingData = false;

    // Create JavaScript command to call the API-based function
    QString script = QString("updateRouteByPorts(%1, %2);").arg(originPortId).arg(destPortId);
    qDebug() << "Executing route script with port IDs:" << script;

    // Execute JavaScript with callback
    m_view->page()->runJavaScript(script, [this, originPortId, destPortId](const QVariant &result) {
        qDebug() << "Route update result for ports" << originPortId << "to" << destPortId << ":" << result;

        // Check if there was an error
        QString resultStr = result.toString();
        if (!resultStr.isEmpty() && resultStr.contains("error", Qt::CaseInsensitive)) {
            qWarning() << "JavaScript execution error:" << resultStr;
            emit routeError(resultStr);
        } else {
            emit routeDisplayed();
        }
    });
}

// Backward compatibility method - now deprecated
void MapboxWidgetSimple::setSeaRoute(const QVariantList &routeCoordinates, const QVariantList &markerData)
{
    qWarning() << "setSeaRoute with coordinates is deprecated. Use setSeaRouteByPorts instead.";

    // For backward compatibility, we can still support this but log a warning
    if (!m_isPageLoaded || !m_isMapReady) {
        // Store data for later processing
        qDebug() << "Storing simple route data for later processing (deprecated method)";
        m_pendingRouteData = routeCoordinates;
        m_pendingMarkerData = markerData;
        m_hasPendingData = true;
        return;
    }

    // Convert to JSON
    QJsonArray routeJson = QJsonArray::fromVariantList(routeCoordinates);
    QJsonArray markersJson = QJsonArray::fromVariantList(markerData);

    QString routeString = QString::fromUtf8(QJsonDocument(routeJson).toJson(QJsonDocument::Compact));
    QString markersString = QString::fromUtf8(QJsonDocument(markersJson).toJson(QJsonDocument::Compact));

    // Create JavaScript command
    QString script = QString("updateRoute(%1, %2);").arg(routeString).arg(markersString);
    qDebug() << "Executing simple route script (deprecated method)";

    // Execute JavaScript with callback
    m_view->page()->runJavaScript(script, [this](const QVariant &result) {
        qDebug() << "Simple route update result (deprecated method):" << result;
        emit routeDisplayed();
    });
}
