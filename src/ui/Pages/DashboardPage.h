#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QTimer>
#include <QPointF>
#include <QList>
#include <QDebug>
#include <QtMath>
#include <QPointer>
#include "MapboxWidget.h"
#include "../../service/MockApiService.h"

#include "HintboxDialog.h"

// Frame overlays forward-declaration
class AlertAndRecomendationFrame;
class KPIOverviewFrame;
class WeatherFrame;

namespace Ui {
class DashboardPage;
}

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

    enum MapView {
        TwoDimension,
        ThreeDimension
    };

    enum MapStyle {
        Dark,
        Light,
        Balanced
    };

    void addAlertRecomendation();

// Toolbar slots from MainWindow initialization
public slots:
    void setCurrentMapStyle(int index);

    // Map-Related Slots
    void WeatherToggleAction_toggled();
    void BathymeterTiggkeAction_toggled();
    void PortsToggleAction_toggled();
    void OceanRouteToggleAction_toggled();
    void ECAToggleAction_toggled();
    void PiracyZoneToggleAction_toggled();

    void PlannedRouteToggleAction_toggled();
    void ActualRouteToggleAction_toggled();

    // Frame Overlay Slots
    void AlretAndRecomendationFrame_toggled(bool checked);
    void KPIOverviewFrame_toggled(bool checked);
    void WeatherFrame_toggled(bool checked);

    void RefreshMapToggleAction_toggled();
    void ReturnToggleAction_toggled();

    void ThreeDimensionMapToggleAction(bool checked);
    void TwoDimensionMapToggleAction(bool checked);

public slots:
    // Ship simulation control functions
    void startShipSimulation();
    void stopShipSimulation();
    void resetShipSimulation();

private slots:
    void updateShipPosition();

    void setupIcon();

    void onDataUpdated(const VoyageLogs& data);

    void createWidgetFrameOverlay();

private:
    void setupInitialMapRoute();
    void setupShipMovementSimulation();
    double calculateBearing(const QPointF &from, const QPointF &to);

    Ui::DashboardPage *ui;
    MapboxWidget *m_mapboxWidget;

    // Ship simulation variables
    QTimer *m_shipUpdateTimer = nullptr;
    QList<QPointF> m_routeCoordinates;
    int m_currentRouteIndex = 0;
    double m_shipProgress = 0.0;

    // Legacy variables
    QVariantList m_routePoints;
    int m_routeSegmentIndex;

    // Overlay Widgets
    QPointer<AlertAndRecomendationFrame> m_AlertAndRecomendationFrame;
    QPointer<KPIOverviewFrame> m_KPIOverviewFrame;
    QPointer<WeatherFrame> m_WeatherFrame;

    MapStyle m_currentMapTheme;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void positionOverlay();

// Hintboxes for demo purpose
private:
    QPointer<HintboxDialog> m_weatherHintBox;
    QPointer<HintboxDialog> m_bathymeterHintBox;
    QPointer<HintboxDialog> m_portsHintBox;
    QPointer<HintboxDialog> m_oceanRouteHintBox;
    QPointer<HintboxDialog> m_ecaHintBox;
    QPointer<HintboxDialog> m_piracyZoneHintBox;
    QPointer<HintboxDialog> m_plannedRouteHintBox;
    QPointer<HintboxDialog> m_actualRouteHintBox;
    QPointer<HintboxDialog> m_refreshMapHintBox;
    QPointer<HintboxDialog> m_returnHintBox;
    QPointer<HintboxDialog> m_3dMapHintBox;
    QPointer<HintboxDialog> m_2dMapHintBox;

};

#endif // DASHBOARDPAGE_H
