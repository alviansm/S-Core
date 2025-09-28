#ifndef VOYAGEPLANNINGPAGE_H
#define VOYAGEPLANNINGPAGE_H

#include <QWidget>
#include <MapboxWidgetSimple.h>
#include <QGraphicsScene>
#include "Components/DialogAddPortRotation.h"
#include "Components/DialogAddCargo.h"
#include "Components/DialogAddFuelPlan.h"
#include "Components/DialogAddNewVoyage.h"

// ==== Data Structs ====
struct PortRotation {
    QString from;
    QString to;
    QString eta;
    QString etc;
};

struct Cargo {
    QString name;
    QString loadingPort;
    QString dischargingPort;
    QString quantity;
    QString units;
};

struct FuelPlan {
    QString type;
    QString quantity;
    QString speed;
    QString units;
};

struct Voyage {
    QString name;
    QString from;
    QString to;
    QString etd;
    QString eta;
    int fromPortId;
    int toPortId;

    QVector<PortRotation> portRotations;
    QVector<Cargo> cargos;
    QVector<FuelPlan> fuelPlans;
};

namespace Ui {
class VoyagePlanningPage;
}

class VoyagePlanningPage : public QWidget
{
    Q_OBJECT

public:
    explicit VoyagePlanningPage(QWidget *parent = nullptr);
    ~VoyagePlanningPage();

    void initializeVoyageList();

    const Voyage* getCurrentlySelectedVoyage() const;
    void selectVoyage(int index);

private slots:
    void on_pushButtonPortRotation_clicked();
    void on_pushButtonCargo_clicked();
    void on_pushButtonFuel_clicked();
    void on_pushButton_clicked();

    void onVoyageSelectionChanged(int currentRow);

    void onMapReady();
    void onRouteDisplayed();
    void onRouteError(const QString &error);

    void on_portRotationDelete_clicked();

    void on_cargoDelete_clicked();

    void on_fuelPlanDelete_clicked();

public slots:
    void loadRouteFromPorts(int originPortId, int destPortId);
    void loadRouteFromSelection();
    void setOriginPort(int portId);
    void setDestinationPort(int portId);
    void clearRoute();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::VoyagePlanningPage *ui;

    MapboxWidgetSimple *m_mapboxWidget;

    // Route
    int m_originPortId = -1;
    int m_destPortId = -1;

    // All voyages
    QVector<Voyage> m_voyages;

    // Graphics
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_pixmapItem;

    // Dialogs
    DialogAddNewVoyage* m_dialogAddVoyage;
    DialogAddPortRotation* m_dialogPortRotation;
    DialogAddCargo* m_dialogAddCargo;
    DialogAddFuelPlan* m_dialogAddFuelPlan;

    // Helpers
    void refreshPortRotationTable(const QVector<PortRotation>& rotations);
    void refreshCargoTable(const QVector<Cargo>& cargos);
    void refreshFuelPlanTable(const QVector<FuelPlan>& fuels);
};

#endif // VOYAGEPLANNINGPAGE_H
