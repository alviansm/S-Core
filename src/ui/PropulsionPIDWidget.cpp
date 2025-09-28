#include "PropulsionPIDWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QDebug>
#include <cmath>
#include <QPainterPath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PropulsionPIDWidget::PropulsionPIDWidget(QWidget *parent)
    : QWidget(parent)
    , m_selectedComponent(None)
    , m_hoveredComponent(None)
    , m_animationStep(0)
    , m_detailPopup(nullptr)
    , m_popupAnimation(nullptr)
{
    setMinimumSize(800, 400);
    setMouseTracking(true);

    // Setup animation timer
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &PropulsionPIDWidget::updateAnimation);
    m_animationTimer->start(100); // 100ms interval for smooth animation

    setupComponents();
    setupFlowLines();
}

void PropulsionPIDWidget::setupComponents()
{
    m_components.clear();

    // Fuel Tank (top left)
    ComponentRect fuelTank;
    fuelTank.rect = QRect(50, 50, 100, 80);
    fuelTank.type = FuelTank;
    fuelTank.name = "Fuel Tank";
    fuelTank.status = "85% Full";
    fuelTank.statusColor = QColor(0, 191, 99); // Green
    fuelTank.isActive = true;
    fuelTank.isHovered = false;
    m_components.append(fuelTank);

    // Main Engines (center, vertically stacked)
    QStringList engineNames = {"ME 1", "ME 2", "ME 3"};
    QList<QColor> engineColors = {
        QColor(0, 191, 99),   // Green - Active
        QColor(241, 196, 15), // Yellow - Warning
        QColor(0, 191, 99)    // Green - Active
    };

    for (int i = 0; i < 3; ++i) {
        ComponentRect engine;
        engine.rect = QRect(250, 80 + i * 100, 120, 60);
        engine.type = static_cast<ComponentType>(MainEngine1 + i);
        engine.name = engineNames[i];
        engine.status = i == 1 ? "High Temp" : "Normal";
        engine.statusColor = engineColors[i];
        engine.isActive = i != 1; // ME2 has issues
        engine.isHovered = false;
        m_components.append(engine);
    }

    // Gearbox (right of engines)
    ComponentRect gearbox;
    gearbox.rect = QRect(420, 150, 100, 80);
    gearbox.type = Gearbox;
    gearbox.name = "Gearbox";
    gearbox.status = "Normal";
    gearbox.statusColor = QColor(0, 191, 99);
    gearbox.isActive = true;
    gearbox.isHovered = false;
    m_components.append(gearbox);

    // Shaft Line (horizontal line after gearbox)
    ComponentRect shaft;
    shaft.rect = QRect(540, 175, 150, 42);
    shaft.type = ShaftLine;
    shaft.name = "Drive Shaft";
    shaft.status = "89 RPM";
    shaft.statusColor = QColor(0, 191, 99);
    shaft.isActive = true;
    shaft.isHovered = false;
    m_components.append(shaft);

    // Propeller (end of shaft)
    ComponentRect propeller;
    propeller.rect = QRect(710, 150, 80, 80);
    propeller.type = Propeller;
    propeller.name = "Propeller";
    propeller.status = "75% Pitch";
    propeller.statusColor = QColor(0, 191, 99);
    propeller.isActive = true;
    propeller.isHovered = false;
    m_components.append(propeller);
}

void PropulsionPIDWidget::setupFlowLines()
{
    m_flowLines.clear();

    // Fuel lines from tank to engines
    for (int i = 0; i < 3; ++i) {
        FlowLine fuelLine;
        fuelLine.start = QPoint(150, 90); // From fuel tank
        fuelLine.end = QPoint(250, 110 + i * 100); // To each engine
        fuelLine.type = FuelFlow;
        fuelLine.color = QColor(52, 152, 219); // Blue for fuel
        fuelLine.isAnimated = true;
        fuelLine.animationOffset = i * 10; // Offset for visual variety
        m_flowLines.append(fuelLine);
    }

    // Power transmission lines (engines to gearbox)
    for (int i = 0; i < 3; ++i) {
        FlowLine powerLine;
        powerLine.start = QPoint(370, 110 + i * 100); // From each engine
        powerLine.end = QPoint(420, 190); // To gearbox center
        powerLine.type = PowerFlow;
        powerLine.color = QColor(46, 204, 113); // Green for power
        powerLine.isAnimated = true;
        powerLine.animationOffset = i * 15;
        m_flowLines.append(powerLine);
    }

    // Gearbox to shaft
    FlowLine gearToShaft;
    gearToShaft.start = QPoint(520, 190);
    gearToShaft.end = QPoint(540, 190);
    gearToShaft.type = PowerFlow;
    gearToShaft.color = QColor(46, 204, 113);
    gearToShaft.isAnimated = true;
    gearToShaft.animationOffset = 0;
    m_flowLines.append(gearToShaft);

    // Shaft to propeller
    FlowLine shaftToProp;
    shaftToProp.start = QPoint(690, 190);
    shaftToProp.end = QPoint(710, 190);
    shaftToProp.type = PowerFlow;
    shaftToProp.color = QColor(46, 204, 113);
    shaftToProp.isAnimated = true;
    shaftToProp.animationOffset = 5;
    m_flowLines.append(shaftToProp);

    // Exhaust lines from engines (upward)
    for (int i = 0; i < 3; ++i) {
        FlowLine exhaustLine;
        exhaustLine.start = QPoint(310, 80 + i * 100); // From top of each engine
        exhaustLine.end = QPoint(310, 30); // Upward to exhaust
        exhaustLine.type = ExhaustFlow;
        exhaustLine.color = QColor(231, 76, 60); // Red for exhaust
        exhaustLine.isAnimated = true;
        exhaustLine.animationOffset = i * 8;
        m_flowLines.append(exhaustLine);
    }
}

void PropulsionPIDWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // Draw background
    painter.fillRect(rect(), QColor(30, 30, 30));

    // Draw flow lines first (behind components)
    for (const FlowLine &line : m_flowLines) {
        drawFlowLine(painter, line);
    }

    // Draw components
    for (const ComponentRect &comp : m_components) {
        drawComponent(painter, comp);
    }

    // Draw legend
    drawLegend(painter);
}

void PropulsionPIDWidget::drawComponent(QPainter &painter, const ComponentRect &comp)
{
    painter.save();

    // Apply hover/selection effects
    if (comp.type == m_hoveredComponent || comp.type == m_selectedComponent) {
        painter.setPen(QPen(QColor(0, 120, 212), 3)); // Blue highlight

        // Add glow effect for hovered items
        if (comp.type == m_hoveredComponent) {
            QColor glowColor = QColor(0, 120, 212, 60);
            painter.setBrush(glowColor);
            painter.drawRoundedRect(comp.rect.adjusted(-5, -5, 5, 5), 8, 8);
        }
    } else {
        painter.setPen(QPen(comp.statusColor, 2));
    }

    // Draw component based on type
    switch (comp.type) {
        case FuelTank:
            drawFuelTank(painter, comp.rect, comp.name);
            break;
        case MainEngine1:
        case MainEngine2:
        case MainEngine3:
            drawEngine(painter, comp.rect, comp.name, comp.isActive);
            break;
        case Gearbox:
            drawGearbox(painter, comp.rect, comp.name);
            break;
        case ShaftLine:
            drawShaftLine(painter, comp.rect);
            break;
        case Propeller:
            drawPropeller(painter, comp.rect);
            break;
        default:
            break;
    }

    painter.restore();
}

void PropulsionPIDWidget::drawFuelTank(QPainter &painter, const QRect &rect, const QString &label)
{
    // Tank body
    painter.setBrush(QColor(45, 45, 45));
    painter.drawRoundedRect(rect, 5, 5);

    // Fuel level indicator
    QRect fuelLevel = rect.adjusted(5, rect.height() * 0.15, -5, -5);
    painter.setBrush(QColor(52, 152, 219, 150));
    painter.drawRect(fuelLevel);

    // Label
    painter.setPen(Qt::white);
    painter.drawText(rect.adjusted(0, -20, 0, 0), Qt::AlignCenter, label);
    painter.drawText(rect, Qt::AlignCenter, "85%");

    // Fuel outlet
    painter.setBrush(QColor(100, 100, 100));
    painter.drawEllipse(rect.right() - 5, rect.center().y() - 3, 10, 6);
}

void PropulsionPIDWidget::drawEngine(QPainter &painter, const QRect &rect, const QString &label, bool isActive)
{
    // Engine block
    QColor engineColor = isActive ? QColor(60, 60, 60) : QColor(80, 40, 40);
    painter.setBrush(engineColor);
    painter.drawRoundedRect(rect, 8, 8);

    // Engine details (cylinders)
    painter.setBrush(QColor(80, 80, 80));
    for (int i = 0; i < 4; ++i) {
        int cylinderX = rect.x() + 10 + i * 25;
        painter.drawRect(cylinderX, rect.y() + 10, 20, 15);
    }

    // Exhaust manifold (top)
    painter.setBrush(QColor(120, 60, 60));
    painter.drawRect(rect.x() + 5, rect.y() - 5, rect.width() - 10, 8);

    // Label and status
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect.adjusted(0, 12, 0, 0), Qt::AlignCenter, label);

    // Status indicator
    QColor statusColor = isActive ? QColor(0, 191, 99) : QColor(231, 76, 60);
    painter.setBrush(statusColor);
    painter.drawEllipse(rect.right() - 15, rect.y() + 5, 10, 10);
}

void PropulsionPIDWidget::drawGearbox(QPainter &painter, const QRect &rect, const QString &label)
{
    // Gearbox housing
    painter.setBrush(QColor(70, 70, 70));
    painter.drawRoundedRect(rect, 10, 10);

    // Gear teeth representation
    painter.setBrush(QColor(100, 100, 100));
    painter.drawEllipse(rect.center().x() - 20, rect.center().y() - 20, 40, 40);

    // Gear teeth
    painter.setPen(QPen(QColor(150, 150, 150), 2));
    for (int i = 0; i < 12; ++i) {
        double angle = i * 30 * M_PI / 180.0;
        int x1 = rect.center().x() + 18 * cos(angle);
        int y1 = rect.center().y() + 18 * sin(angle);
        int x2 = rect.center().x() + 22 * cos(angle);
        int y2 = rect.center().y() + 22 * sin(angle);
        painter.drawLine(x1, y1, x2, y2);
    }

    // Label
    painter.setPen(Qt::white);
    painter.drawText(rect.adjusted(0, 52, 0, 0), Qt::AlignCenter, label);
}

void PropulsionPIDWidget::drawShaftLine(QPainter &painter, const QRect &rect)
{
    // Main shaft
    painter.setBrush(QColor(150, 150, 150));
    painter.drawRect(rect.x(), rect.center().y() - 8, rect.width(), 16);

    // Shaft segments
    painter.setPen(QPen(QColor(100, 100, 100), 1));
    for (int i = 0; i < rect.width(); i += 20) {
        painter.drawLine(rect.x() + i, rect.center().y() - 8,
                        rect.x() + i, rect.center().y() + 8);
    }

    // Bearings
    painter.setBrush(QColor(80, 80, 80));
    painter.drawEllipse(rect.x() + 30, rect.center().y() - 12, 24, 24);
    painter.drawEllipse(rect.x() + rect.width() - 54, rect.center().y() - 12, 24, 24);

    // Label
    painter.setPen(Qt::white);
    painter.drawText(rect.adjusted(0, 25, 0, 0), Qt::AlignCenter, "Drive Shaft");
}

void PropulsionPIDWidget::drawPropeller(QPainter &painter, const QRect &rect)
{
    painter.save();

    // Move to propeller center
    painter.translate(rect.center());

    // Hub
    painter.setBrush(QColor(100, 100, 100));
    painter.drawEllipse(-15, -15, 30, 30);

    // Propeller blades
    painter.setBrush(QColor(80, 120, 160));
    for (int i = 0; i < 4; ++i) {
        painter.save();
        painter.rotate(i * 90 + m_animationStep * 2); // Slow rotation animation

        // Blade shape (elliptical)
        QPainterPath blade;
        blade.addEllipse(-35, -8, 70, 16);
        painter.drawPath(blade);

        painter.restore();
    }

    painter.restore();

    // Label
    painter.setPen(Qt::white);
    painter.drawText(rect.adjusted(0, 90, 0, 0), Qt::AlignCenter, "Propeller");
    painter.drawText(rect.adjusted(0, 105, 0, 0), Qt::AlignCenter, "75% Pitch");
}

void PropulsionPIDWidget::drawFlowLine(QPainter &painter, const FlowLine &line)
{
    painter.save();

    // Set up pen for flow line
    QPen flowPen(line.color, 3);
    if (line.isAnimated) {
        // Create dashed line pattern for animation
        QVector<qreal> dashPattern;
        dashPattern << 10 << 5;
        flowPen.setDashPattern(dashPattern);
        flowPen.setDashOffset(line.animationOffset + m_animationStep);
    }
    painter.setPen(flowPen);

    // Draw the main line
    painter.drawLine(line.start, line.end);

    // Draw flow direction indicators
    drawFlowIndicator(painter, line.end, line.type);

    painter.restore();
}

void PropulsionPIDWidget::drawFlowIndicator(QPainter &painter, const QPoint &pos, FlowType type)
{
    painter.save();

    QColor indicatorColor;
    switch (type) {
        case FuelFlow:
            indicatorColor = QColor(52, 152, 219);
            break;
        case ExhaustFlow:
            indicatorColor = QColor(231, 76, 60);
            break;
        case PowerFlow:
            indicatorColor = QColor(46, 204, 113);
            break;
    }

    painter.setBrush(indicatorColor);
    painter.setPen(indicatorColor);

    // Draw arrow head
    QPolygon arrow;
    arrow << QPoint(pos.x() - 8, pos.y() - 4)
          << QPoint(pos.x(), pos.y())
          << QPoint(pos.x() - 8, pos.y() + 4);
    painter.drawPolygon(arrow);

    painter.restore();
}

void PropulsionPIDWidget::drawLegend(QPainter &painter)
{
    painter.save();

    // Legend background
    QRect legendRect(width() - 180, 10, 170, 120);
    painter.setBrush(QColor(40, 40, 40, 200));
    painter.setPen(QColor(100, 100, 100));
    painter.drawRoundedRect(legendRect, 5, 5);

    // Legend title
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(legendRect.x() + 10, legendRect.y() + 20, "Legend");

    // Legend items
    font.setBold(false);
    font.setPointSize(8);
    painter.setFont(font);

    QStringList legendItems = {
        "Fuel Flow", "Power Flow", "Exhaust Flow"
    };
    QList<QColor> legendColors = {
        QColor(52, 152, 219),  // Blue
        QColor(46, 204, 113),  // Green
        QColor(231, 76, 60)    // Red
    };

    for (int i = 0; i < legendItems.size(); ++i) {
        int y = legendRect.y() + 40 + i * 25;

        // Color indicator
        painter.setBrush(legendColors[i]);
        painter.drawRect(legendRect.x() + 15, y - 8, 15, 8);

        // Text
        painter.setPen(Qt::white);
        painter.drawText(legendRect.x() + 40, y, legendItems[i]);
    }

    painter.restore();
}

void PropulsionPIDWidget::mousePressEvent(QMouseEvent *event)
{
    ComponentType clickedComponent = getComponentAtPosition(event->pos());

    if (clickedComponent != None) {
        m_selectedComponent = clickedComponent;
        showComponentDetails(clickedComponent);
        update();
    } else {
        m_selectedComponent = None;
        if (m_detailPopup) {
            m_detailPopup->hide();
        }
        update();
    }
}

void PropulsionPIDWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateHoverState(event->pos());
}

void PropulsionPIDWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (m_hoveredComponent != None) {
        m_hoveredComponent = None;
        update();
    }
}

void PropulsionPIDWidget::updateAnimation()
{
    m_animationStep = (m_animationStep + 1) % 100;
    update();
}

PropulsionPIDWidget::ComponentType PropulsionPIDWidget::getComponentAtPosition(const QPoint &pos)
{
    for (const ComponentRect &comp : m_components) {
        if (comp.rect.contains(pos)) {
            return comp.type;
        }
    }
    return None;
}

void PropulsionPIDWidget::updateHoverState(const QPoint &pos)
{
    ComponentType hoveredComponent = getComponentAtPosition(pos);

    if (hoveredComponent != m_hoveredComponent) {
        m_hoveredComponent = hoveredComponent;

        // Update cursor
        if (hoveredComponent != None) {
            setCursor(Qt::PointingHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }

        update();
    }
}

void PropulsionPIDWidget::showComponentDetails(ComponentType component)
{
    // Hide existing popup
    if (m_detailPopup) {
        m_detailPopup->deleteLater();
        m_detailPopup = nullptr;
    }

    // Find the component data
    ComponentRect selectedComp;
    for (const ComponentRect &comp : m_components) {
        if (comp.type == component) {
            selectedComp = comp;
            break;
        }
    }

    // Create detail popup
    createDetailPopup(component);
}

void PropulsionPIDWidget::createDetailPopup(ComponentType component)
{
    m_detailPopup = new QWidget(this);
    m_detailPopup->setStyleSheet(
        "QWidget {"
        "    background-color: #2d2d2d;"
        "    border: 2px solid #0078d4;"
        "    border-radius: 8px;"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: white;"
        "    padding: 2px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(m_detailPopup);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(6);

    // Component-specific details
    switch (component) {
        case MainEngine1:
        case MainEngine2:
        case MainEngine3: {
            int idx = component - MainEngine1;
            const EngineData& e = m_engineData[idx];

            QLabel* title = new QLabel(e.name);
            title->setStyleSheet("font-weight: bold; font-size: 12px; color: #0078d4;");
            layout->addWidget(title);

            layout->addWidget(new QLabel(QString("Power: %1 MW").arg(e.powerMW)));
            layout->addWidget(new QLabel(QString("RPM: %1").arg(e.rpm)));
            layout->addWidget(new QLabel(QString("Fuel Rate: %1 kg/h").arg(e.fuelRateKgH)));
            layout->addWidget(new QLabel(QString("Efficiency: %1%").arg(e.efficiency)));

            if (e.highExhaustTemp) {
                QLabel* warning = new QLabel("⚠ High exhaust temperature");
                warning->setStyleSheet("color: #ffc107;");
                layout->addWidget(warning);
            }
            break;
        }

        case Propeller: {
            QLabel* title = new QLabel("Controllable Pitch Propeller");
            title->setStyleSheet("font-weight: bold; font-size: 12px; color: #0078d4;");
            layout->addWidget(title);

            layout->addWidget(new QLabel(QString("RPM: %1").arg(m_propellerData.rpm)));
            layout->addWidget(new QLabel(QString("Pitch: %1%").arg(m_propellerData.pitchPercent)));
            layout->addWidget(new QLabel(QString("Thrust: %1 kN").arg(m_propellerData.thrustKN)));
            layout->addWidget(new QLabel(QString("Efficiency: %1%").arg(m_propellerData.efficiency)));
            break;
        }

        case Gearbox: {
            QLabel* title = new QLabel("Main Gearbox");
            title->setStyleSheet("font-weight: bold; font-size: 12px; color: #0078d4;");
            layout->addWidget(title);

            layout->addWidget(new QLabel(QString("Input RPM: %1").arg(m_gearboxData.inputRPM)));
            layout->addWidget(new QLabel(QString("Output RPM: %1").arg(m_gearboxData.outputRPM)));
            layout->addWidget(new QLabel(QString("Oil Pressure: %1 bar").arg(m_gearboxData.oilPressureBar)));
            layout->addWidget(new QLabel(QString("Temperature: %1°C").arg(m_gearboxData.temperatureC)));
            break;
        }

        case FuelTank: {
            QLabel* title = new QLabel("Fuel Oil Tanks");
            title->setStyleSheet("font-weight: bold; font-size: 12px; color: #0078d4;");
            layout->addWidget(title);

            layout->addWidget(new QLabel(QString("FO Tank 1 (P): %1").arg(m_fuelTankData.tank1P)));
            layout->addWidget(new QLabel(QString("FO Tank 1 (S): %1").arg(m_fuelTankData.tank1S)));
            layout->addWidget(new QLabel(QString("FO Tank 2 (P): %1").arg(m_fuelTankData.tank2P)));
            layout->addWidget(new QLabel(QString("FO Tank 2 (S): %1").arg(m_fuelTankData.tank2S)));
            break;
        }

        default:
            break;
    }

    // Position popup near the component
    ComponentRect selectedComp;
    for (const ComponentRect &comp : m_components) {
        if (comp.type == component) {
            selectedComp = comp;
            break;
        }
    }

    // Calculate popup position
    m_detailPopup->adjustSize();
    QPoint popupPos;

    // Position popup to the right of component if there's space, otherwise to the left
    if (selectedComp.rect.right() + m_detailPopup->width() + 20 < width()) {
        popupPos.setX(selectedComp.rect.right() + 10);
    } else {
        popupPos.setX(selectedComp.rect.left() - m_detailPopup->width() - 10);
    }

    // Center vertically on component
    popupPos.setY(selectedComp.rect.center().y() - m_detailPopup->height() / 2);

    // Ensure popup stays within widget bounds
    if (popupPos.y() < 0) popupPos.setY(10);
    if (popupPos.y() + m_detailPopup->height() > height()) {
        popupPos.setY(height() - m_detailPopup->height() - 10);
    }

    m_detailPopup->move(popupPos);
    m_detailPopup->show();

    // Add drop shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setOffset(2, 2);
    m_detailPopup->setGraphicsEffect(shadow);

    // Animate popup appearance
    if (m_popupAnimation) {
        m_popupAnimation->stop();
        m_popupAnimation->deleteLater();
    }

    m_popupAnimation = new QPropertyAnimation(m_detailPopup, "windowOpacity");
    m_popupAnimation->setDuration(200);
    m_popupAnimation->setStartValue(0.0);
    m_popupAnimation->setEndValue(1.0);
    m_popupAnimation->setEasingCurve(QEasingCurve::OutQuad);
    m_popupAnimation->start();
}
