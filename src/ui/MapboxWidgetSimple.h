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

    void setSeaRoute(const QVariantList &routeCoordinates, const QVariantList &markerData);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onPageLoaded(bool ok);
    void checkMapReady();

signals:
    void mapReady();
    void routeDisplayed();

private:
    QWebEngineView *m_view;
    bool m_isPageLoaded = false;
    bool m_isMapReady = false;
    QTimer *m_mapReadyTimer = nullptr;

    // Pending data storage
    QVariantList m_pendingRouteData;
    QVariantList m_pendingMarkerData;
    bool m_hasPendingData = false;

    void processPendingData();
};

#endif // MAPBOXWIDGETSIMPLE_H
