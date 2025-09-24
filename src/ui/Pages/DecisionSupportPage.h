#ifndef DECISIONSUPPORTPAGE_H
#define DECISIONSUPPORTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>

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
        CIIPerformanceComparisonGraph,
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
    void createPageWidgets(int pageIndex, const QString& pageName);

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
};



#endif // DECISIONSUPPORTPAGE_H
