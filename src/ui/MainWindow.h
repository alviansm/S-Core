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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_4_clicked();

private:
    void setupDockManager();
    void createContents();

    ads::CDockWidget* createWelcomePage();
    ads::CDockWidget* createMapboxPage();

private:
    Ui_MainWindow *ui;
    ads::CDockManager* m_DockManager;
};

#endif // MAINWINDOW_H
