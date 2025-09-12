#ifndef NAVIGATIONSIDEBAR_H
#define NAVIGATIONSIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE
class Ui_NavigationSidebar;
QT_END_NAMESPACE

class NavigationSidebar : public QWidget
{
    Q_OBJECT

public:
    NavigationSidebar(QWidget *parent = nullptr);
    ~NavigationSidebar();

    enum NavigationPage {
        Dashboard = 0,
        Technical,
        Savings,
        VoyagePlanning,
        History,
        Settings
    };

signals:
    void pageRequested(NavigationPage page);

private slots:
    void onNavigationButtonClicked();

private:
    Ui_NavigationSidebar *ui;
    QButtonGroup *navigationGroup;
    
    void setupNavigationButtons();
    void updateButtonStyles();
    QPushButton* createNavigationButton(const QString &text, const QString &icon, NavigationPage page);
};

#endif // NAVIGATIONSIDEBAR_H