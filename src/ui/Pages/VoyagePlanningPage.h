#ifndef VOYAGEPLANNINGPAGE_H
#define VOYAGEPLANNINGPAGE_H

#include <QWidget>
#include <MapboxWidgetSimple.h>
#include <QGraphicsScene>

struct Voyage {
    QString name;
    QString from;
    QString to;
    QString etd;
    QString eta;
    int fromPortId;
    int toPortId;
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

    void initializePortRotationList();
    void initializeCargoList();
    void initializFuelPlanList();

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

    void loadRouteExample();

    // Member variables for storing selected port IDs
    int m_originPortId = -1;
    int m_destPortId = -1;

    QVector<Voyage> m_voyages;

    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_pixmapItem;
};

#endif // VOYAGEPLANNINGPAGE_H
