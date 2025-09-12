#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "DockManager.h"
#include "PageWidgets.h"

QT_BEGIN_NAMESPACE
class Ui_MainWindow;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

private:
    void setupDockManager();
    
private:
    Ui_MainWindow *ui;
    ads::CDockManager* m_dockManager;
    QStackedWidget* m_centralPages;
    
    // Page widgets
    DashboardPage* m_dashboardPage;
    TechnicalPage* m_technicalPage;
    SavingsPage* m_savingsPage;
    VoyagePlanningPage* m_voyagePlanningPage;
    HistoryPage* m_historyPage;
    SettingsPage* m_settingsPage;
};

#endif // MAINWINDOW_H
