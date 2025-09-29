#ifndef MAPBOXWIDGET_H
#define MAPBOXWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVariantList>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QWebEngineView;
class QFrame;
QT_END_NAMESPACE

struct PendingRouteData {
    QVariantList routeCoordinates;
    QVariantList markerData;
};

struct PendingIconData {
    QString iconName;
    QString resourcePath;
};

class MapboxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapboxWidget(QWidget *parent = nullptr);
    ~MapboxWidget();

    void reloadMap();
    void returnToInitialView();

public slots:
    void setSeaRoute(const QVariantList &routeCoordinates, const QVariantList &markerData);
    void setShipPosition(double longitude, double latitude, double bearing = 0.0);
    void loadIconFromResource(const QString& iconNameInJs, const QString& resourcePath);

private slots:
    void onPageLoaded(bool ok);
    void checkMapReady();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void mapReady();
    void routeDisplayed();
    void iconLoaded(const QString& iconName);

private:
    QWebEngineView *m_view;
    bool m_isPageLoaded = false;
    bool m_isMapReady = false;
    QFrame *m_weatherBox = nullptr;
    QLabel *m_weatherLabel = nullptr;

    // Pending data storage
    PendingRouteData m_pendingRouteData;
    PendingIconData m_pendingIconData;
    bool m_hasPendingRouteData = false;
    bool m_hasPendingIconData = false;

    // Timer for checking map readiness
    QTimer *m_mapReadyTimer = nullptr;

    // Private helper functions
    void processPendingData();
};

#endif // MAPBOXWIDGET_H
