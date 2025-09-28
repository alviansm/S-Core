#include "CarbonIntensityGauge.h"


CarbonIntensityGauge::CarbonIntensityGauge(QWidget *parent)
    : QWidget(parent)
    , m_currentValue(250)  // Current CI value (gCO2/kWh)
    , m_targetValue(250)
    , m_animatedValue(250)
    , m_animationTimer(new QTimer(this))
{
    setFixedSize(200, 220); // Adjusted height to 220px

    // Setup animation
    connect(m_animationTimer, &QTimer::timeout, this, &CarbonIntensityGauge::updateAnimation);
    m_animationTimer->setInterval(16); // ~60fps
}

void CarbonIntensityGauge::setValue(double value) {
    m_targetValue = qBound(0.0, value, 800.0);
    if (!m_animationTimer->isActive()) {
        m_animationTimer->start();
    }
}

double CarbonIntensityGauge::value() const { return m_currentValue; }

void CarbonIntensityGauge::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Adjust rect to account for the new height while keeping the gauge circular
    QRectF rect = QRectF(10, 10, 180, 180); // Keep square for circular gauge
    QPointF center = rect.center();
    double radius = qMin(rect.width(), rect.height()) / 2.0;

    // Draw background circle
    painter.setBrush(QBrush(QColor(45, 45, 45)));
    painter.setPen(QPen(QColor(70, 70, 70), 2));
    painter.drawEllipse(center, radius, radius);

    // Draw intensity zones (colored arcs)
    drawIntensityZones(painter, center, radius);

    // Draw scale marks and labels
    drawScale(painter, center, radius);

    // Draw needle
    drawNeedle(painter, center, radius * 0.8);

    // Draw center information (moved to left side)
    drawCenterInfo(painter, center);
}

void CarbonIntensityGauge::updateAnimation() {
    const double animationSpeed = 0.05;
    double diff = m_targetValue - m_animatedValue;

    if (qAbs(diff) < 0.5) {
        m_animatedValue = m_targetValue;
        m_currentValue = m_targetValue;
        m_animationTimer->stop();
    } else {
        m_animatedValue += diff * animationSpeed;
        m_currentValue = m_animatedValue;
    }

    update();
}

void CarbonIntensityGauge::drawIntensityZones(QPainter &painter, const QPointF &center, double radius) {
    // Define zones based on actual thresholds: Low (0-250), Medium (250-400), High (400-600), Very High (600-800)
    const QColor zoneColors[] = {
        QColor(40, 167, 69),   // Low - Green (0-250)
        QColor(255, 193, 7),   // Medium - Yellow (250-400)
        QColor(253, 126, 20),  // High - Orange (400-600)
        QColor(220, 53, 69)    // Very High - Red (600-800)
    };

    const double startAngle = 225; // Start from bottom-left
    const double totalSpan = 270;  // 270 degrees total

    // Zone boundaries: 0, 250, 400, 600, 800
    const double zoneBoundaries[] = {0, 250, 400, 600, 800};

    for (int i = 0; i < 4; ++i) {
        double startValue = zoneBoundaries[i];
        double endValue = zoneBoundaries[i + 1];

        // Calculate angles based on actual value ranges
        double startNormalized = startValue / 800.0;
        double endNormalized = endValue / 800.0;

        double angle = startAngle + (startNormalized * totalSpan);
        double zoneSpan = (endNormalized - startNormalized) * totalSpan;

        QPen pen(zoneColors[i], 8);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        QRectF arcRect(center.x() - radius + 15, center.y() - radius + 15,
                       2 * (radius - 15), 2 * (radius - 15));
        painter.drawArc(arcRect, angle * 16, zoneSpan * 16);
    }
}

void CarbonIntensityGauge::drawScale(QPainter &painter, const QPointF &center, double radius) {
    painter.setPen(QPen(QColor(200, 200, 200), 1));

    const double startAngle = 225;
    const double totalSpan = 270;

    // Scale values that align with zone boundaries
    const int scaleValues[] = {0, 250, 400, 600, 800};
    const int numLabels = 5;

    // Major ticks and labels
    for (int i = 0; i < numLabels; ++i) {
        double normalizedValue = scaleValues[i] / 800.0;
        double angle = startAngle + (normalizedValue * totalSpan);
        double radian = qDegreesToRadians(angle);

        QPointF tickStart(center.x() + (radius - 25) * cos(radian),
                          center.y() + (radius - 25) * sin(radian));
        QPointF tickEnd(center.x() + (radius - 15) * cos(radian),
                        center.y() + (radius - 15) * sin(radian));

        painter.setPen(QPen(QColor(200, 200, 200), 2));
        painter.drawLine(tickStart, tickEnd);

        // Labels
        QPointF labelPos(center.x() + (radius - 35) * cos(radian),
                         center.y() + (radius - 35) * sin(radian));

        painter.setPen(QPen(QColor(180, 180, 180)));
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        painter.drawText(QRectF(labelPos.x() - 15, labelPos.y() - 8, 30, 16),
                         Qt::AlignCenter, QString::number(scaleValues[i]));
    }

    // Minor ticks between major values
    painter.setPen(QPen(QColor(120, 120, 120), 1));
    for (int i = 0; i < numLabels - 1; ++i) {
        double startVal = scaleValues[i];
        double endVal = scaleValues[i + 1];
        double stepSize = (endVal - startVal) / 5; // 4 minor ticks between major ones

        for (int j = 1; j < 5; ++j) {
            double tickValue = startVal + (j * stepSize);
            double normalizedValue = tickValue / 800.0;
            double angle = startAngle + (normalizedValue * totalSpan);
            double radian = qDegreesToRadians(angle);

            QPointF tickStart(center.x() + (radius - 22) * cos(radian),
                              center.y() + (radius - 22) * sin(radian));
            QPointF tickEnd(center.x() + (radius - 18) * cos(radian),
                            center.y() + (radius - 18) * sin(radian));

            painter.drawLine(tickStart, tickEnd);
        }
    }
}

void CarbonIntensityGauge::drawNeedle(QPainter &painter, const QPointF &center, double radius) {
    // Calculate needle angle based on current value
    // The scale goes from 0 to 800, mapped across 270 degrees starting at 225 degrees
    double normalizedValue = qBound(0.0, m_currentValue / 800.0, 1.0);

    // Our scale spans from 225° to 495° (225 + 270)
    // 225° is bottom-left, 495° (= 135°) is top-right
    double needleAngle = 225 + (normalizedValue * 270);

    // Convert to radians for the actual needle tip calculation
    double radian = qDegreesToRadians(needleAngle);

    // Draw needle shadow
    painter.save();
    painter.translate(center.x() + 1, center.y() + 1);

    // Create needle shadow path pointing from center to the correct angle
    QPainterPath shadowPath;
    QPointF needleTip(radius * 0.8 * cos(radian), radius * 0.8 * sin(radian));
    shadowPath.moveTo(0, 0);  // Center
    shadowPath.lineTo(needleTip.x(), needleTip.y());  // Tip

    QPen shadowPen(QColor(0, 0, 0, 100), 4);
    shadowPen.setCapStyle(Qt::RoundCap);
    painter.setPen(shadowPen);
    painter.drawPath(shadowPath);
    painter.restore();

    // Draw main needle
    painter.save();
    painter.translate(center);

    // Create needle path pointing from center to the correct angle
    QPainterPath needlePath;
    QPointF needleTip2(radius * 0.8 * cos(radian), radius * 0.8 * sin(radian));
    needlePath.moveTo(0, 0);  // Center
    needlePath.lineTo(needleTip2.x(), needleTip2.y());  // Tip

    QPen needlePen(QColor(255, 255, 255), 3);
    needlePen.setCapStyle(Qt::RoundCap);
    painter.setPen(needlePen);
    painter.drawPath(needlePath);

    // Draw needle outline
    QPen outlinePen(QColor(100, 100, 100), 1);
    painter.setPen(outlinePen);
    painter.drawPath(needlePath);

    painter.restore();

    // Draw center hub
    QRadialGradient hubGradient(center, 8);
    hubGradient.setColorAt(0, QColor(240, 240, 240));
    hubGradient.setColorAt(1, QColor(180, 180, 180));

    painter.setBrush(QBrush(hubGradient));
    painter.setPen(QPen(QColor(120, 120, 120), 1));
    painter.drawEllipse(center, 8, 8);
}

void CarbonIntensityGauge::drawCenterInfo(QPainter &painter, const QPointF &center) {
    // Position the text in the left whitespace area
    // Calculate position based on gauge center and radius
    QPointF textCenter(center.x() - 45, center.y() + 10);

    // Current value display
    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.setFont(QFont("Arial", 18, QFont::Bold)); // Increased font size

    QString valueText = QString::number(qRound(m_currentValue));
    QRectF valueRect(textCenter.x() - 25, textCenter.y() - 15, 50, 25);
    painter.drawText(valueRect, Qt::AlignCenter, valueText);

    // Units - positioned directly below the value
    painter.setFont(QFont("Arial", 9));
    painter.setPen(QPen(QColor(180, 180, 180)));
    QRectF unitsRect(textCenter.x() - 30, textCenter.y() + 12, 60, 15);
    painter.drawText(unitsRect, Qt::AlignCenter, "gCO₂/kWh");

    // Status text - positioned above the value
    QString status = getIntensityStatus();
    QColor statusColor = getStatusColor();
    painter.setPen(QPen(statusColor));
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QRectF statusRect(textCenter.x() - 35, textCenter.y() - 40, 70, 15);
    painter.drawText(statusRect, Qt::AlignCenter, status);
}

QString CarbonIntensityGauge::getIntensityStatus() const {
    if (m_currentValue < 250) return "LOW";
    else if (m_currentValue < 400) return "MEDIUM";
    else if (m_currentValue < 600) return "HIGH";
    else return "VERY HIGH";
}

QColor CarbonIntensityGauge::getStatusColor() const {
    if (m_currentValue < 250) return QColor(40, 167, 69);   // Green
    else if (m_currentValue < 400) return QColor(255, 193, 7);   // Yellow
    else if (m_currentValue < 600) return QColor(253, 126, 20);  // Orange
    else return QColor(220, 53, 69); // Red
}
