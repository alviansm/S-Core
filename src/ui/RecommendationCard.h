#ifndef RECOMMENDATIONCARD_H
#define RECOMMENDATIONCARD_H

#include <QFrame>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QStyle>
#include <QButtonGroup>
#include <QPushButton>
#include <QMouseEvent>

struct RecommendationItem {
    enum Type {
        Warning,
        Recommendation,
        Info,
        Critical,
        Maintenance
    };

    Type type;
    QString title;
    QString description;
    QString value; // e.g., "8.2% fuel savings", "4.2 hrs saved"
    int priority; // 1-5, 1 being highest
    bool isActionable;
};

class RecommendationCard : public QFrame {
    Q_OBJECT

public:
    RecommendationCard(const RecommendationItem& item, QWidget* parent = nullptr);

private slots:
    void onCardClicked();

signals:
    void recommendationClicked(const RecommendationItem& item);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void setupUI();

    void setupAnimation() {
        m_hoverAnimation = new QPropertyAnimation(this, "styleSheet");
        m_hoverAnimation->setDuration(150);
        m_hoverAnimation->setStartValue(getCardStyleSheet());
        m_hoverAnimation->setEndValue(getHoverStyleSheet());
    }

    QString getCardStyleSheet() const {
        return QString(
            "RecommendationCard {"
            "    background-color: %1;"
            "    border: 1px solid %2;"
            "    border-left: 4px solid %3;"
            "    border-radius: 8px;"
            "}"
        ).arg(getBackgroundColor(), getBorderColor(), getAccentColor());
    }

    QString getHoverStyleSheet() const {
        return QString(
            "RecommendationCard {"
            "    background-color: %1;"
            "    border: 1px solid %2;"
            "    border-left: 4px solid %3;"
            "    border-radius: 8px;"
            "}"
        ).arg(getHoverBackgroundColor(), getHoverBorderColor(), getAccentColor());
    }

    QIcon getIcon() const {
        switch (m_item.type) {
            case RecommendationItem::Warning:
                return style()->standardIcon(QStyle::SP_MessageBoxWarning);
            case RecommendationItem::Critical:
                return style()->standardIcon(QStyle::SP_MessageBoxCritical);
            case RecommendationItem::Info:
                return style()->standardIcon(QStyle::SP_MessageBoxInformation);
            case RecommendationItem::Maintenance:
                return style()->standardIcon(QStyle::SP_ComputerIcon);
            case RecommendationItem::Recommendation:
            default:
                return style()->standardIcon(QStyle::SP_DialogApplyButton);
        }
    }

    QString getBackgroundColor() const {
        switch (m_item.type) {
            case RecommendationItem::Warning: return "#2d2619";
            case RecommendationItem::Critical: return "#2d1919";
            case RecommendationItem::Info: return "#19242d";
            case RecommendationItem::Maintenance: return "#1d1d2d";
            case RecommendationItem::Recommendation:
            default: return "#1d2d19";
        }
    }

    QString getHoverBackgroundColor() const {
        switch (m_item.type) {
            case RecommendationItem::Warning: return "#3d3629";
            case RecommendationItem::Critical: return "#3d2929";
            case RecommendationItem::Info: return "#29343d";
            case RecommendationItem::Maintenance: return "#2d2d3d";
            case RecommendationItem::Recommendation:
            default: return "#2d3d29";
        }
    }

    QString getBorderColor() const {
        return "#404040";
    }

    QString getHoverBorderColor() const {
        return "#505050";
    }

    QString getAccentColor() const {
        switch (m_item.type) {
            case RecommendationItem::Warning: return "#ffc107";
            case RecommendationItem::Critical: return "#dc3545";
            case RecommendationItem::Info: return "#17a2b8";
            case RecommendationItem::Maintenance: return "#6f42c1";
            case RecommendationItem::Recommendation:
            default: return "#28a745";
        }
    }

    QString getPriorityColor() const {
        switch (m_item.priority) {
            case 1: return "#dc3545"; // High priority - red
            case 2: return "#fd7e14"; // Medium-high - orange
            case 3: return "#ffc107"; // Medium - yellow
            case 4: return "#28a745"; // Low-medium - green
            case 5: return "#6c757d"; // Low - gray
            default: return "#6c757d";
        }
    }

    RecommendationItem m_item;
    QPropertyAnimation* m_hoverAnimation;
};

#endif // RECOMMENDATIONCARD_H
