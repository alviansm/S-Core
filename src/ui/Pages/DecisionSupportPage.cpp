#include "DecisionSupportPage.h"
#include "ui_DecisionSupportPage.h"

DecisionSupportPage::DecisionSupportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DecisionSupportPage)
    , m_splitter(nullptr)
    , m_sidebarContainer(nullptr)
    , m_topSidebar(nullptr)
    , m_mainSidebar(nullptr)
    , m_contentContainer(nullptr)
    , m_topContent(nullptr)
    , m_mainContent(nullptr)
    , m_currentPage(CIIPerformanceComparisonGraph)
{
    ui->setupUi(this);

    setupWidget();
    createPageContent();
    setupStylesheet();

    // Set initial page
    setCurrentPage(CIIPerformanceComparisonGraph);
}

DecisionSupportPage::~DecisionSupportPage()
{
    delete ui;
}

void DecisionSupportPage::setCurrentPage(int pageIndex)
{
    if (pageIndex >= 0 && pageIndex < 5) // We have 5 pages
    {
        m_currentPage = pageIndex;

        // Update all the main content stacked widget to show the same page index
        m_mainContent->setCurrentIndex(pageIndex);

        qInfo() << "Current page changed to:" << pageIndex;
    }
}

void DecisionSupportPage::createPageContent()
{

}

void DecisionSupportPage::createPageWidgets(int pageIndex, const QString &pageName)
{

}

void DecisionSupportPage::setupWidget()
{
    // --- Splitter ---
    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(Qt::Horizontal);

    // --- Sidebar container ---
    QWidget *sidebarWidget = new QWidget(m_splitter);
    m_sidebarContainer = new QVBoxLayout(sidebarWidget);
    m_sidebarContainer->setContentsMargins(0, 0, 0, 0);
    m_sidebarContainer->setSpacing(6);

    // Create stacked widgets instead of regular widgets
    m_topSidebar = new QStackedWidget(sidebarWidget);
    m_mainSidebar = new QStackedWidget(sidebarWidget);

    // Ratio 1:3 vertically in the sidebar
    m_sidebarContainer->addWidget(m_topSidebar, 1);  // stretch factor 1
    m_sidebarContainer->addWidget(m_mainSidebar, 3); // stretch factor 3

    // --- Content container ---
    QWidget *contentWidget = new QWidget(m_splitter);
    m_contentContainer = new QVBoxLayout(contentWidget);
    m_contentContainer->setContentsMargins(0, 0, 0, 0);
    m_contentContainer->setSpacing(6);

    // Create stacked widgets instead of regular widgets
    m_topContent = new QStackedWidget(contentWidget);
    m_mainContent = new QStackedWidget(contentWidget);

    // Ratio 1:3 vertically in the content
    m_contentContainer->addWidget(m_topContent, 1);  // stretch factor 1
    m_contentContainer->addWidget(m_mainContent, 3); // stretch factor 3

    // --- Add to splitter ---
    // Sidebar first, content second
    m_splitter->addWidget(sidebarWidget);
    m_splitter->addWidget(contentWidget);

    // --- Sidebar sizing rules ---
    sidebarWidget->setMinimumWidth(240);
    sidebarWidget->setMaximumWidth(280);

    // Allow splitter to resize content dynamically
    QList<int> sizes;
    sizes << 260 << 700; // initial sizes, sidebar ~260px
    m_splitter->setSizes(sizes);

    // --- Top-level layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_splitter);
    this->setLayout(mainLayout);
}

void DecisionSupportPage::setupStylesheet()
{
    QString basicStyleSheet = "QWidget { background-color: #212121; }";
    m_topSidebar->setStyleSheet(basicStyleSheet );
    m_mainSidebar->setStyleSheet(basicStyleSheet );
    m_topContent->setStyleSheet(basicStyleSheet );

    QString stackedWidgetStyleSheet = "QStackedWidget { background-color: #212121; }";
    m_mainContent->setStyleSheet(stackedWidgetStyleSheet);

    m_splitter->setStyleSheet(
        "QSplitter::handle { background: transparent; }"
    );
}
