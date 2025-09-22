#ifndef VOYAGEPLANNINGPAGE_H
#define VOYAGEPLANNINGPAGE_H

#include <QWidget>
#include <MapboxWidgetSimple.h>

struct Voyage {
    QString name;
    QString from;
    QString to;
    QString etd;
    QString eta;
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

private slots:
    void on_pushButtonPortRotation_clicked();

    void on_pushButtonCargo_clicked();

    void on_pushButtonFuel_clicked();

    void on_pushButton_clicked();

private:
    Ui::VoyagePlanningPage *ui;

    MapboxWidgetSimple *m_mapboxWidget;
};

#endif // VOYAGEPLANNINGPAGE_H
