#include "HintboxDialog.h"
#include "ui_HintboxDialog.h"

HintboxDialog::HintboxDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HintboxDialog)
{
    ui->setupUi(this);
    setupWidget();
}

HintboxDialog::~HintboxDialog()
{
    delete ui;
}

void HintboxDialog::setTitle(const QString &title)
{
    m_labelHintboxTitle->setText(title);
}

void HintboxDialog::addNewPage(QString imgPath, QString text)
{
    // Create a new widget to hold the page content
    QWidget *pageWidget = new QWidget();
    QVBoxLayout *pageLayout = new QVBoxLayout(pageWidget);

    // Create image label with dynamic height based on aspect ratio
    QLabel *imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(false); // Don't scale contents, we'll handle scaling manually

    // Load image from resources
    QPixmap pixmap(imgPath);
    if (!pixmap.isNull()) {
        // Scale pixmap to width 320 while maintaining aspect ratio
        QPixmap scaledPixmap = pixmap.scaledToWidth(320, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);

        // Set the label size to match the scaled pixmap
        imageLabel->setFixedSize(scaledPixmap.size());
    } else {
        // Fallback size if image fails to load
        imageLabel->setFixedSize(320, 240);
    }

    // Create text label with dark mode styling
    QLabel *textLabel = new QLabel(text);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Apply dark mode text styling
    textLabel->setStyleSheet(
        "QLabel {"
        "color: #ffffff;"
        "font-size: 11px;"
        "line-height: 1.16;"
        "padding: 3px;"
        "background-color: transparent;"
        "}"
    );

    // Add labels to layout
    pageLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);
    pageLayout->addWidget(textLabel, 1);
    pageLayout->setSpacing(10);
    pageLayout->setContentsMargins(10, 10, 10, 10);

    // Add page to stacked widget
    m_hitboxStackedWidget->addWidget(pageWidget);

    // Update pagination circles
    updatePaginationCircles();
}

void HintboxDialog::removePageAtIndex(int index)
{
    if (index < 0 || index >= m_hitboxStackedWidget->count()) {
        return; // Invalid index
    }

    // Get the widget at the specified index
    QWidget *widgetToRemove = m_hitboxStackedWidget->widget(index);

    if (widgetToRemove) {
        // Remove from stacked widget
        m_hitboxStackedWidget->removeWidget(widgetToRemove);

        // Delete the widget
        widgetToRemove->deleteLater();

        // Adjust current index if necessary
        int currentIndex = m_hitboxStackedWidget->currentIndex();
        if (currentIndex >= m_hitboxStackedWidget->count() && m_hitboxStackedWidget->count() > 0) {
            m_hitboxStackedWidget->setCurrentIndex(m_hitboxStackedWidget->count() - 1);
        }

        // Update pagination circles
        updatePaginationCircles();
    }
}

void HintboxDialog::setupWidget()
{
    m_hitboxStackedWidget = ui->hitboxStackedWidget;
    m_labelHintboxTitle = ui->labelHintboxTitle;

    m_pushButtonPrevious = ui->pushButtonPrevious;
    m_paginationCircles = ui->paginationCircles;
    m_pushButtonNext = ui->pushButtonNext;

    m_buttonBox = ui->buttonBox;

    setupSignals();
    applyStylesheet_dark();
}

void HintboxDialog::setupSignals()
{
    connect(m_pushButtonNext, &QPushButton::clicked, this, &HintboxDialog::gotoNextPage);
    connect(m_pushButtonPrevious, &QPushButton::clicked, this, &HintboxDialog::gotoPreviousPage);
}

void HintboxDialog::applyStylesheet_dark()
{
    // Main dialog styling
    this->setStyleSheet(
        "HintboxDialog {"
        "background-color: #2b2b2b;"
        "color: #ffffff;"
        "border: 1px solid #444444;"
        "border-radius: 8px;"
        "}"

        // Title label styling
        "QLabel#labelHintboxTitle {"
        "color: #ffffff;"
        "font-size: 18px;"
        "font-weight: bold;"
        "padding: 10px;"
        "background-color: transparent;"
        "}"

        // Stacked widget styling
        "QStackedWidget {"
        "background-color: #3c3c3c;"
        "border: 1px solid #555555;"
        "border-radius: 6px;"
        "padding: 5px;"
        "}"

        // Navigation buttons styling
        "QPushButton {"
        "background-color: #4a4a4a;"
        "color: #ffffff;"
        "border: 1px solid #666666;"
        "}"

        "QPushButton:hover {"
        "background-color: #5a5a5a;"
        "border-color: #777777;"
        "}"

        "QPushButton:pressed {"
        "background-color: #3a3a3a;"
        "border-color: #555555;"
        "}"

        "QPushButton:disabled {"
        "background-color: #333333;"
        "color: #666666;"
        "border-color: #444444;"
        "}"

        // Dialog button box styling
        "QDialogButtonBox {"
        "background-color: transparent;"
        "}"

        "QDialogButtonBox QPushButton {"
        "background-color: #0d7377;"
        "color: #ffffff;"
        "border: 1px solid #14a085;"
        "border-radius: 6px;"
        "padding: 3px 6px;"
        "font-size: 11px;"
        "min-width: 36px;"
        "}"

        "QDialogButtonBox QPushButton:hover {"
        "background-color: #14a085;"
        "border-color: #17c3a3;"
        "}"

        "QDialogButtonBox QPushButton:pressed {"
        "background-color: #0a5d61;"
        "border-color: #0d7377;"
        "}"

        // Pagination circles container
        "QWidget#paginationCircles {"
        "background-color: transparent;"
        "}"
    );
}

void HintboxDialog::updatePaginationCircles()
{
    // Clear existing circles
    QLayout *existingLayout = m_paginationCircles->layout();
    if (existingLayout) {
        QLayoutItem *item;
        while ((item = existingLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete existingLayout;
    }

    int totalPages = m_hitboxStackedWidget->count();
    int currentPage = m_hitboxStackedWidget->currentIndex();

    if (totalPages <= 0) {
        return;
    }

    // Create horizontal layout for circles
    QHBoxLayout *circleLayout = new QHBoxLayout(m_paginationCircles);
    circleLayout->setAlignment(Qt::AlignCenter);
    circleLayout->setSpacing(8);
    circleLayout->setContentsMargins(0, 0, 0, 0);

    // Create circles for each page
    for (int i = 0; i < totalPages; ++i) {
        QLabel *circle = new QLabel();
        circle->setFixedSize(12, 12);
        circle->setAlignment(Qt::AlignCenter);

        // Style the circle based on whether it's the current page
        QString styleSheet;
        if (i == currentPage) {
            // Current page circle - filled
            styleSheet = "QLabel {"
                        "border-radius: 6px;"
                        "background-color: #ffffff;"
                        "border: 2px solid #ffffff;"
                        "}";
        } else {
            // Other page circles - outline only
            styleSheet = "QLabel {"
                        "border-radius: 6px;"
                        "background-color: transparent;"
                        "border: 2px solid #666666;"
                        "}";
        }

        circle->setStyleSheet(styleSheet);
        circleLayout->addWidget(circle);
    }
}

void HintboxDialog::gotoNextPage()
{
    int currentIndex = m_hitboxStackedWidget->currentIndex();
    int totalPages = m_hitboxStackedWidget->count();

    if (totalPages > 0) {
        int nextIndex = (currentIndex + 1) % totalPages; // Wrap around to first page
        m_hitboxStackedWidget->setCurrentIndex(nextIndex);
    }

    updatePaginationCircles();
}

void HintboxDialog::gotoPreviousPage()
{
    int currentIndex = m_hitboxStackedWidget->currentIndex();
    int totalPages = m_hitboxStackedWidget->count();

    if (totalPages > 0) {
        int prevIndex = (currentIndex - 1 + totalPages) % totalPages; // Wrap around to last page
        m_hitboxStackedWidget->setCurrentIndex(prevIndex);
    }

    updatePaginationCircles();
}
