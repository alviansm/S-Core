#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPointer>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "DockManager.h"

QT_BEGIN_NAMESPACE
class Ui_MainWindow;
QT_END_NAMESPACE

// ──────────────────────────────────────────────
// NOTE: Some styling, such as QMenu in QMenuBar,
//       is configured directly in the .ui file
//       (not in this header).
//
// NOTE: Standard window size in .ui -> 1024 x 637
// ──────────────────────────────────────────────

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum Page {
        Dashboard,
        Technical,
        DecisionSupport,
        VoyagePlanning,
        History,
        Setting
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @note main pages
     */
    void on_pushButton_DashboardPage_clicked(bool checked);
    void on_pushButton_TechnicalPage_clicked(bool checked);
    void on_pushButton_DecisionSupportPage_clicked(bool checked);
    void on_pushButton_RoutePlanningPage_clicked(bool checked);
    void on_pushButton_HistoryPage_clicked(bool checked);
    void on_pushButton_SettingPage_clicked(bool checked);

private:
    void addDockWidgetWithDockManager(QPushButton *actionSource, bool checked, ads::DockWidgetArea area, MainWindow::Page page);

private:
    void setupDockManager();
    void createContents();
    void createActions(); // Create toolbar actions

    ads::CDockWidget* createWelcomePage();
    ads::CDockWidget* createMapboxPage();
    ads::CDockWidget* createDashboardPage();
    ads::CDockWidget* createTechnicalPage();
    ads::CDockWidget* createDecisionSupportPage();
    ads::CDockWidget* createVoyagePlanningPage();
    ads::CDockWidget* createHistoryPage();
    ads::CDockWidget* createSettingPage();

private:
    Ui_MainWindow *ui;
    ads::CDockManager* m_DockManager;

    ads::CDockAreaWidget* m_availableDockArea;
};

#endif // MAINWINDOW_H
