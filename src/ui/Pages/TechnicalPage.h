#ifndef TECHNICALPAGE_H
#define TECHNICALPAGE_H

#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "../../service/MockApiService.h"
#include "../EngineStatusWidget.h"
#include "../PropulsionPIDWidget.h"

namespace Ui {
class TechnicalPage;
}

class TechnicalPage : public QWidget
{
    Q_OBJECT

public:
    explicit TechnicalPage(QWidget *parent = nullptr);
    ~TechnicalPage();

    enum Page {
        PropulsionSystemPage = 0,
        ElectricalSystemPage,
        FuelManagementSystemPage,
        BallastSystemPage,
        HotelLoadPage
    };

// Slots from toolbar
public slots:
    void IODevice_toggled(bool checked = true);
    void PageSelector_currentIndex_changed(int index);
    void setCurrentPage(int pageIndex);

    void toggleHideShow_topSidebar(bool checked);
    void toggleHideShow_mainSidebar(bool checked);
    void toggleHideShow_topContent(bool checked);

private slots:
    void onDataUpdated(const VoyageLogs& data);

private:
    void setupWidget();
    void setupStylesheet();
    void createPageContent();
    void createPageWidgets(int pageIndex, const QString& pageName);

    // --- Propulsion System Page Contents ---
    void createPropulsionSystemPageContent();
    void createPropulsionSystemPageContent_topSidebar();
    void createPropulsionSystemPageContent_mainSidebar();
    void createPropulsionSystemPageContent_topContent();
    void createPropulsionSystemPageContent_mainContent();

    // --- Electrical System Page Content ---
    void createElectricalSystemPageContent();
    void createElectricalSystemPageContent_topSidebar();
    void createElectricalSystemPageContent_mainSidebar();
    void createElectricalSystemPageContent_topContent();
    void createElectricalSystemPageContent_mainContent();

    // --- Fuel Management System Page Content ---
    void createFuelManagementSystemPageContent();
    void createFuelManagementSystemPageContent_topSidebar();
    void createFuelManagementSystemPageContent_mainSidebar();
    void createFuelManagementSystemPageContent_topContent();
    void createFuelManagementSystemPageContent_mainContent();

    // --- Ballast System Page Content ---
    void createBallastSystemPageContent();
    void createBallastSystemPageContent_topSidebar();
    void createBallastSystemPageContent_mainSidebar();
    void createBallastSystemPageContent_topContent();
    void createBallastSystemPageContent_mainContent();

    // --- Hotel Load Page Content ---
    void createHotelLoadSystemPageContent();
    void createHotelLoadSystemPageContent_topSidebar();
    void createHotelLoadSystemPageContent_mainSidebar();
    void createHotelLoadSystemPageContent_topContent();
    void createHotelLoadSystemPageContent_mainContent();

private:
    Ui::TechnicalPage *ui;

    // Layout widget
    QSplitter* m_splitter;

    QVBoxLayout* m_sidebarContainer;
    QStackedWidget* m_topSidebar;
    QStackedWidget* m_mainSidebar;

    QVBoxLayout* m_contentContainer;
    QStackedWidget* m_topContent;
    QStackedWidget* m_mainContent;

    EngineStatusWidget* m_me1;
    EngineStatusWidget* m_me2;
    EngineStatusWidget* m_me3;

    PropulsionPIDWidget* m_pidWidget;

    int m_currentPage;
};

#endif // TECHNICALPAGE_H
