#include "mapboxwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWebEngineView>
#include <QFrame>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MapboxWidget::MapboxWidget(QWidget *parent)
    : QWidget(parent)
{
    m_view = new QWebEngineView(this);

    connect(m_view->page(), &QWebEnginePage::loadFinished, this, &MapboxWidget::onPageLoaded);

    m_view->setUrl(QUrl("qrc:/html/map.html"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);

    setLayout(mainLayout);
}

void MapboxWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_weatherBox) {
        m_weatherBox->move(width() - m_weatherBox->width() - 20, 20);
    }
}

void MapboxWidget::onPageLoaded(bool ok)
{
    if (ok) {
        m_isPageLoaded = true;
        // Tunggu hingga map benar-benar ready
        checkMapReady();
    }
}

void MapboxWidget::checkMapReady()
{
    // Periksa apakah map sudah ready dengan JavaScript
    QString script = R"(
        if (typeof map !== 'undefined' && map.isStyleLoaded()) {
            'MAP_READY';
        } else {
            'MAP_NOT_READY';
        }
    )";

    m_view->page()->runJavaScript(script, [this](const QVariant &result) {
        if (result.toString() == "MAP_READY") {
            m_isMapReady = true;
            emit mapReady();
        } else {
            // Coba lagi setelah 500ms
            QTimer::singleShot(500, this, &MapboxWidget::checkMapReady);
        }
    });
}

void MapboxWidget::setSeaRoute(const QVariantList &routeCoordinates, const QVariantList &markerData)
{
    if (!m_isPageLoaded || !m_isMapReady) {
        // Simpan data untuk diproses nanti
        m_pendingRouteData = {routeCoordinates, markerData};
        return;
    }

    QJsonArray routeJson = QJsonArray::fromVariantList(routeCoordinates);
    QJsonArray markersJson = QJsonArray::fromVariantList(markerData);

    QString routeString = QString::fromUtf8(QJsonDocument(routeJson).toJson(QJsonDocument::Compact));
    QString markersString = QString::fromUtf8(QJsonDocument(markersJson).toJson(QJsonDocument::Compact));

    QString script = QString("updateRoute(%1, %2);").arg(routeString).arg(markersString);

    // Jalankan dengan callback untuk memastikan berhasil
    m_view->page()->runJavaScript(script, [this](const QVariant &result) {
        emit routeDisplayed();
        qDebug() << "Route update result:" << result;
    });
}

void MapboxWidget::setShipPosition(double longitude, double latitude, double bearing)
{
    if (!m_isPageLoaded) return;

    // Buat array koordinat JSON
    QJsonArray coordsJson;
    coordsJson.append(longitude);
    coordsJson.append(latitude);

    QString coordsString = QString::fromUtf8(QJsonDocument(coordsJson).toJson(QJsonDocument::Compact));

    QString script = QString("updateShipPosition(%1, %2);").arg(coordsString).arg(bearing);

    m_view->page()->runJavaScript(script);
}

void MapboxWidget::loadIconFromResource(const QString& iconNameInJs, const QString& resourcePath)
{
    qDebug() << "loadIconFromResource called:" << iconNameInJs << resourcePath;

    if (!m_isPageLoaded || !m_isMapReady) {
        // Store data for later processing
        qDebug() << "Storing icon data for later processing";
        m_pendingIconData.iconName = iconNameInJs;
        m_pendingIconData.resourcePath = resourcePath;
        m_hasPendingIconData = true;
        return;
    }

    // Clear pending data flag since we're processing now
    m_hasPendingIconData = false;

    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open icon resource:" << resourcePath;

        // Create fallback circular ship marker
        QString fallbackScript = QString(R"(
            try {
                if (typeof createFallbackShipLayer === 'function') {
                    createFallbackShipLayer();
                } else {
                    // Inline fallback creation
                    if (!map.getSource('ship-source')) {
                        map.addSource('ship-source', {
                            'type': 'geojson',
                            'data': {
                                'type': 'Feature',
                                'geometry': {
                                    'type': 'Point',
                                    'coordinates': [0, 0]
                                }
                            }
                        });
                    }
                    if (!map.getLayer('ship-layer')) {
                        map.addLayer({
                            'id': 'ship-layer',
                            'type': 'circle',
                            'source': 'ship-source',
                            'paint': {
                                'circle-radius': 8,
                                'circle-color': '#FF0000',
                                'circle-stroke-width': 2,
                                'circle-stroke-color': '#FFFFFF'
                            }
                        });
                    }
                    shipIconLoaded = true;
                    'FALLBACK_CREATED';
                }
            } catch(e) {
                'FALLBACK_ERROR: ' + e.message;
            }
        )");

        m_view->page()->runJavaScript(fallbackScript, [this, iconNameInJs](const QVariant &result) {
            qDebug() << "Fallback ship creation result:" << result;
            emit iconLoaded(iconNameInJs);
        });
        return;
    }

    // Read image data
    QByteArray imageData = file.readAll();
    file.close();

    if (imageData.isEmpty()) {
        qWarning() << "Icon file is empty:" << resourcePath;
        return;
    }

    qDebug() << "Icon file loaded, size:" << imageData.size() << "bytes";

    // Determine MIME type
    QString mimeType = "image/png";
    if (resourcePath.endsWith(".jpg", Qt::CaseInsensitive) ||
        resourcePath.endsWith(".jpeg", Qt::CaseInsensitive)) {
        mimeType = "image/jpeg";
    } else if (resourcePath.endsWith(".svg", Qt::CaseInsensitive)) {
        mimeType = "image/svg+xml";
    }

    // Convert to base64
    QByteArray base64Data = imageData.toBase64();
    QString dataUrl = QString("data:%1;base64,%2").arg(mimeType).arg(QString::fromLatin1(base64Data));

    // Create JavaScript command
    QString script = QString("loadCustomImageFromData('%1', '%2');").arg(iconNameInJs).arg(dataUrl);

    // Execute JavaScript
    m_view->page()->runJavaScript(script, [this, iconNameInJs](const QVariant &result) {
        qDebug() << "Icon load result for" << iconNameInJs << ":" << result;
        emit iconLoaded(iconNameInJs);
    });
}

MapboxWidget::~MapboxWidget()
{
}

void MapboxWidget::reloadMap()
{
    if (!m_isPageLoaded) return;

    m_isMapReady = false;
    m_view->reload();

    // Setelah reload, tunggu hingga map ready lagi
    connect(this, &MapboxWidget::mapReady, this, [this]() {
        // Proses data yang tertunda jika ada
        if (!m_pendingRouteData.routeCoordinates.isEmpty() || !m_pendingRouteData.markerData.isEmpty()) {
            setSeaRoute(m_pendingRouteData.routeCoordinates, m_pendingRouteData.markerData);
            m_pendingRouteData = {}; // Clear pending data
        }
        if (m_hasPendingIconData) {
            loadIconFromResource(m_pendingIconData.iconName, m_pendingIconData.resourcePath);
            m_pendingIconData = {};
            m_hasPendingIconData = false;
        }
    });
}

void MapboxWidget::returnToInitialView()
{
    if (!m_isPageLoaded) return;

    QString script = "resetMapView();";
    m_view->page()->runJavaScript(script);
}
