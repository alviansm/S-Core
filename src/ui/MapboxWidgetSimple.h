#ifndef MAPBOXWIDGETSIMPLE_H
#define MAPBOXWIDGETSIMPLE_H

#include <QWidget>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QVariantList>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QWebEngineView;
QT_END_NAMESPACE

class MapboxWidgetSimple : public QWidget
{
    Q_OBJECT

public:
    explicit MapboxWidgetSimple(QWidget *parent = nullptr);
    ~MapboxWidgetSimple();

    // New method using port IDs (recommended)
    void setSeaRouteByPorts(int originPortId, int destPortId);

    // Backward compatibility method (deprecated)
    void setSeaRoute(const QVariantList &routeCoordinates, const QVariantList &markerData);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onPageLoaded(bool ok);
    void checkMapReady();

signals:
    void mapReady();
    void routeDisplayed();
    void routeError(const QString &error);

private:
    QWebEngineView *m_view;
    bool m_isPageLoaded = false;
    bool m_isMapReady = false;
    QTimer *m_mapReadyTimer = nullptr;

    // Pending data storage for new port-based method
    int m_pendingOriginPortId = -1;
    int m_pendingDestPortId = -1;
    bool m_hasPendingData = false;

    // Backward compatibility storage (deprecated)
    QVariantList m_pendingRouteData;
    QVariantList m_pendingMarkerData;

    void processPendingData();
};

#endif // MAPBOXWIDGETSIMPLE_H
