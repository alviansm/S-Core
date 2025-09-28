#ifndef DECISIONSUPPORTPAGE_H
#define DECISIONSUPPORTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>

#include "RecommendationCard.h"
#include "CarbonIntensityGauge.h"

namespace Ui {
class DecisionSupportPage;
}

class DecisionSupportPage : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionSupportPage(QWidget *parent = nullptr);
    ~DecisionSupportPage();

    enum MainContentPage {
        CIIPerformanceComparisonGraph = 0,
        ThreeYearImplementationSummary,
        SelfEvaluationFeedbackLoop,
        CorrectiveActionPlan
    };

public slots:
    void setCurrentPage(int pageIndex);

private:
    void setupWidget();
    void setupStylesheet();
    void createPageContent();

    void createPageContent_topSidebar();
    void createPageContent_mainSidebar();
    void createPageContent_topContent(); // NOTE: Skipped

    void createMainContent_CIIPerformanceComparisonGraph();
    void createMainContent_ThreeYearImplementationSummary();
    void createMainContent_SelfEvaluationFeedbackLoop();
    void createMainContent_CorrectiveActionPlan();

    void onRecommendationClicked(const RecommendationItem& item);

private:
    Ui::DecisionSupportPage *ui;

    // Layout widget
    QSplitter* m_splitter;

    QVBoxLayout* m_sidebarContainer;
    QWidget* m_topSidebar;
    QWidget* m_mainSidebar;

    QVBoxLayout* m_contentContainer;
    QWidget* m_topContent;
    QStackedWidget* m_mainContent;

    int m_currentPage;

    CarbonIntensityGauge* m_carbonIntensityGauge;
};



#endif // DECISIONSUPPORTPAGE_H
