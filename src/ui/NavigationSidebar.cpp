#include "NavigationSidebar.h"
#include "ui_NavigationSidebar.h"
#include <QApplication>

NavigationSidebar::NavigationSidebar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_NavigationSidebar)
    , navigationGroup(new QButtonGroup(this))
{
    ui->setupUi(this);
    setupNavigationButtons();
    
    // Set default selection to Dashboard
    if (!navigationGroup->buttons().isEmpty()) {
        navigationGroup->buttons().first()->setChecked(true);
        updateButtonStyles();
    }
}

NavigationSidebar::~NavigationSidebar()
{
    delete ui;
}

void NavigationSidebar::setupNavigationButtons()
{
    // Clear existing buttons
    navigationGroup->setExclusive(true);
    
    // Main navigation buttons
    auto dashboardBtn = createNavigationButton("Dashboard", "📊", Dashboard);
    auto technicalBtn = createNavigationButton("Technical", "⚙️", Technical);
    auto savingsBtn = createNavigationButton("Savings", "💰", Savings);
    auto voyageBtn = createNavigationButton("Voyage Planning", "🗺️", VoyagePlanning);
    auto historyBtn = createNavigationButton("History", "📈", History);
    
    // Add main buttons to navigation area
    ui->navigationLayout->addWidget(dashboardBtn);
    ui->navigationLayout->addWidget(technicalBtn);
    ui->navigationLayout->addWidget(savingsBtn);
    ui->navigationLayout->addWidget(voyageBtn);
    ui->navigationLayout->addWidget(historyBtn);
    
    // Add spacer to push settings to bottom
    ui->navigationLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    // Settings button in footer
    auto settingsBtn = createNavigationButton("Settings", "⚙️", Settings);
    ui->footerLayout->addWidget(settingsBtn);
    
    // Connect button group signal
    connect(navigationGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &NavigationSidebar::onNavigationButtonClicked);
}

QPushButton* NavigationSidebar::createNavigationButton(const QString &text, const QString &icon, NavigationPage page)
{
    QPushButton *button = new QPushButton(this);
    button->setText(QString("%1  %2").arg(icon, text));
    button->setCheckable(true);
    button->setProperty("navigationPage", static_cast<int>(page));
    button->setObjectName(QString("navButton_%1").arg(static_cast<int>(page)));
    
    // Set consistent button properties
    button->setMinimumHeight(40);
    button->setStyleSheet(R"(
        QPushButton {
            text-align: left;
            padding-left: 15px;
            padding-right: 10px;
            border: none;
            border-radius: 6px;
            background-color: transparent;
            color: #B0B0B0;
            font-size: 13px;
            font-weight: normal;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.05);
            color: #FFFFFF;
        }
        QPushButton:checked {
            background-color: #2D5AA0;
            color: #FFFFFF;
            font-weight: bold;
        }
        QPushButton:checked:hover {
            background-color: #3D6AB0;
        }
    )");
    
    navigationGroup->addButton(button, static_cast<int>(page));
    return button;
}

void NavigationSidebar::onNavigationButtonClicked()
{
    QAbstractButton *clickedButton = navigationGroup->checkedButton();
    if (clickedButton) {
        int pageIndex = clickedButton->property("navigationPage").toInt();
        updateButtonStyles();
        emit pageRequested(static_cast<NavigationPage>(pageIndex));
    }
}

void NavigationSidebar::updateButtonStyles()
{
    // Styles are already handled in the button stylesheet
    // This method can be used for additional style updates if needed
}