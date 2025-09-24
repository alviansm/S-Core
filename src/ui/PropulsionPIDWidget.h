#ifndef PROPULSIONPIDWIDGET_H
#define PROPULSIONPIDWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

class PropulsionPIDWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropulsionPIDWidget(QWidget *parent = nullptr);

    enum ComponentType {
        None = 0,
        MainEngine1,
        MainEngine2,
        MainEngine3,
        Gearbox,
        ShaftLine,
        Propeller,
        FuelTank,
        FuelSupplyLine
    };

    enum FlowType {
        FuelFlow = 0,
        ExhaustFlow,
        PowerFlow
    };

    struct ComponentRect {
        QRect rect;
        ComponentType type;
        QString name;
        QString status;
        QColor statusColor;
        bool isActive;
        bool isHovered;
    };

    struct FlowLine {
        QPoint start;
        QPoint end;
        FlowType type;
        QColor color;
        bool isAnimated;
        int animationOffset;
    };

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void updateAnimation();
    void showComponentDetails(ComponentType component);

private:
    void setupComponents();
    void setupFlowLines();
    void drawComponent(QPainter &painter, const ComponentRect &comp);
    void drawFlowLine(QPainter &painter, const FlowLine &line);
    void drawFuelTank(QPainter &painter, const QRect &rect, const QString &label);
    void drawEngine(QPainter &painter, const QRect &rect, const QString &label, bool isActive);
    void drawGearbox(QPainter &painter, const QRect &rect, const QString &label);
    void drawShaftLine(QPainter &painter, const QRect &rect);
    void drawPropeller(QPainter &painter, const QRect &rect);
    void drawFlowIndicator(QPainter &painter, const QPoint &pos, FlowType type);
    void drawLegend(QPainter &painter);

    ComponentType getComponentAtPosition(const QPoint &pos);
    void updateHoverState(const QPoint &pos);
    void createDetailPopup(ComponentType component);

    QList<ComponentRect> m_components;
    QList<FlowLine> m_flowLines;
    QTimer* m_animationTimer;
    ComponentType m_selectedComponent;
    ComponentType m_hoveredComponent;
    int m_animationStep;

    // Detail popup
    QWidget* m_detailPopup;
    QPropertyAnimation* m_popupAnimation;
};

#endif // PROPULSIONPIDWIDGET_H
