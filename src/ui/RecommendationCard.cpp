#include "RecommendationCard.h"

RecommendationCard::RecommendationCard(const RecommendationItem &item, QWidget *parent)
    : QFrame(parent), m_item(item) {
    setupUI();
    setupAnimation();
}

void RecommendationCard::onCardClicked() {
    emit recommendationClicked(m_item);
}

void RecommendationCard::enterEvent(QEnterEvent *event) {
    // Apply hover style directly instead of using animation
    setStyleSheet(getHoverStyleSheet());
    setCursor(Qt::PointingHandCursor);
    QFrame::enterEvent(event);
}

void RecommendationCard::leaveEvent(QEvent *event) {
    // Apply normal style directly instead of using animation
    setStyleSheet(getCardStyleSheet());
    setCursor(Qt::ArrowCursor);
    QFrame::leaveEvent(event);
}

void RecommendationCard::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        onCardClicked();
    }
    QFrame::mousePressEvent(event);
}

void RecommendationCard::setupUI() {
    setFixedHeight(80);
    setStyleSheet(getCardStyleSheet());

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(12, 8, 12, 8);
    mainLayout->setSpacing(8); // Reduced spacing

    // Icon label
    QLabel* iconLabel = new QLabel();
    iconLabel->setFixedSize(24, 24);
    iconLabel->setPixmap(getIcon().pixmap(24, 24));
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(iconLabel);

    // Content layout
    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->setSpacing(1); // Reduced spacing between title and description
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Title
    QLabel* titleLabel = new QLabel(m_item.title);
    titleLabel->setStyleSheet("color: white; font-weight: bold; font-size: 12px; background: transparent;");
    titleLabel->setWordWrap(false);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLabel->setText(titleLabel->fontMetrics().elidedText(m_item.title, Qt::ElideRight, 220));
    contentLayout->addWidget(titleLabel);

    // Description
    QLabel* descLabel = new QLabel(m_item.description);
    descLabel->setStyleSheet("color: #cccccc; font-size: 10px; background: transparent;");
    descLabel->setWordWrap(false);
    descLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    descLabel->setText(descLabel->fontMetrics().elidedText(m_item.description, Qt::ElideRight, 220));
    contentLayout->addWidget(descLabel);

    // Value/benefit (moved to bottom)
    if (!m_item.value.isEmpty()) {
        QLabel* valueLabel = new QLabel(m_item.value);
        valueLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 10px; background: transparent;").arg(getAccentColor()));
        valueLabel->setWordWrap(false);
        valueLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        contentLayout->addWidget(valueLabel);
    }

    mainLayout->addLayout(contentLayout, 1);

    // Priority indicator (small colored dot)
    QLabel* priorityDot = new QLabel();
    priorityDot->setFixedSize(8, 8);
    priorityDot->setStyleSheet(QString(
                                   "background-color: %1; border-radius: 4px;"
                                   ).arg(getPriorityColor()));
    priorityDot->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(priorityDot);
}
