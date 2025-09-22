#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QTimer>
#include <QPointF>
#include <QList>
#include <QDebug>
#include <QtMath>
#include "MapboxWidget.h"

namespace Ui {
class DashboardPage;
}

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

public slots:
    // Ship simulation control functions
    void startShipSimulation();
    void stopShipSimulation();
    void resetShipSimulation();

private slots:
    void updateShipPosition();

    void setupIcon();

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
};

#endif // DASHBOARDPAGE_H
