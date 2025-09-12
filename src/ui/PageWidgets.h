#ifndef PAGEWIDGETS_H
#define PAGEWIDGETS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Ui_DashboardPage;
class Ui_TechnicalPage;
class Ui_SavingsPage;
class Ui_VoyagePlanningPage;
class Ui_HistoryPage;
class Ui_SettingsPage;
QT_END_NAMESPACE

// Dashboard Page
class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

private:
    Ui_DashboardPage *ui;
};

// Technical Page
class TechnicalPage : public QWidget
{
    Q_OBJECT

public:
    explicit TechnicalPage(QWidget *parent = nullptr);
    ~TechnicalPage();

private:
    Ui_TechnicalPage *ui;
};

// Savings Page
class SavingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SavingsPage(QWidget *parent = nullptr);
    ~SavingsPage();

private:
    Ui_SavingsPage *ui;
};

// Voyage Planning Page
class VoyagePlanningPage : public QWidget
{
    Q_OBJECT

public:
    explicit VoyagePlanningPage(QWidget *parent = nullptr);
    ~VoyagePlanningPage();

private:
    Ui_VoyagePlanningPage *ui;
};

// History Page
class HistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPage(QWidget *parent = nullptr);
    ~HistoryPage();

private:
    Ui_HistoryPage *ui;
};

// Settings Page
class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

private:
    Ui_SettingsPage *ui;
};

#endif // PAGEWIDGETS_H