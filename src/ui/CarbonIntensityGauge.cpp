#include "CarbonIntensityGauge.h"


CarbonIntensityGauge::CarbonIntensityGauge(QWidget *parent)
    : QWidget(parent)
    , m_currentValue(250)  // Current CI value (gCO2/kWh)
    , m_targetValue(250)
    , m_animatedValue(250)
    , m_animationTimer(new QTimer(this))
{
    setFixedSize(200, 200);

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

    QRectF rect = this->rect().adjusted(10, 10, -10, -10);
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

    // Draw center information
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
    // Define zones: Low (0-250), Medium (250-400), High (400-600), Very High (600-800)
    const QColor zoneColors[] = {
        QColor(40, 167, 69),   // Low - Green
        QColor(255, 193, 7),   // Medium - Yellow
        QColor(253, 126, 20),  // High - Orange
        QColor(220, 53, 69)    // Very High - Red
    };

    const double startAngle = 225; // Start from bottom-left
    const double totalSpan = 270;  // 270 degrees total
    const double zoneSpan = totalSpan / 4; // Each zone gets equal arc

    for (int i = 0; i < 4; ++i) {
        double angle = startAngle + (i * zoneSpan);

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

    const int majorTicks = 5;
    const int minorTicks = 4; // between major ticks
    const double startAngle = 225;
    const double totalSpan = 270;

    // Major ticks and labels
    for (int i = 0; i <= majorTicks; ++i) {
        double angle = startAngle + (i * totalSpan / majorTicks);
        double radian = qDegreesToRadians(angle);

        QPointF tickStart(center.x() + (radius - 25) * cos(radian),
                          center.y() + (radius - 25) * sin(radian));
        QPointF tickEnd(center.x() + (radius - 15) * cos(radian),
                        center.y() + (radius - 15) * sin(radian));

        painter.setPen(QPen(QColor(200, 200, 200), 2));
        painter.drawLine(tickStart, tickEnd);

        // Labels
        int value = (i * 800) / majorTicks;
        QPointF labelPos(center.x() + (radius - 35) * cos(radian),
                         center.y() + (radius - 35) * sin(radian));

        painter.setPen(QPen(QColor(180, 180, 180)));
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        painter.drawText(QRectF(labelPos.x() - 15, labelPos.y() - 8, 30, 16),
                         Qt::AlignCenter, QString::number(value));
    }

    // Minor ticks
    painter.setPen(QPen(QColor(120, 120, 120), 1));
    for (int i = 0; i < majorTicks; ++i) {
        for (int j = 1; j <= minorTicks; ++j) {
            double angle = startAngle + (i * totalSpan / majorTicks) +
                           (j * totalSpan / (majorTicks * (minorTicks + 1)));
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
    double normalizedValue = qBound(0.0, m_currentValue / 800.0, 1.0);
    double needleAngle = 225 + (normalizedValue * 270);
    double radian = qDegreesToRadians(needleAngle);

    // Draw needle shadow
    painter.save();
    painter.translate(center.x() + 1, center.y() + 1);
    painter.rotate(needleAngle - 90);

    QPainterPath shadowPath;
    shadowPath.moveTo(0, -radius + 20);
    shadowPath.lineTo(-3, -10);
    shadowPath.lineTo(-8, 0);
    shadowPath.lineTo(0, 8);
    shadowPath.lineTo(8, 0);
    shadowPath.lineTo(3, -10);
    shadowPath.closeSubpath();

    painter.fillPath(shadowPath, QBrush(QColor(0, 0, 0, 100)));
    painter.restore();

    // Draw main needle
    painter.save();
    painter.translate(center);
    painter.rotate(needleAngle - 90);

    QPainterPath needlePath;
    needlePath.moveTo(0, -radius + 20);
    needlePath.lineTo(-3, -10);
    needlePath.lineTo(-8, 0);
    needlePath.lineTo(0, 8);
    needlePath.lineTo(8, 0);
    needlePath.lineTo(3, -10);
    needlePath.closeSubpath();

    QLinearGradient needleGradient(0, -radius + 20, 0, 8);
    needleGradient.setColorAt(0, QColor(255, 255, 255));
    needleGradient.setColorAt(1, QColor(200, 200, 200));

    painter.fillPath(needlePath, QBrush(needleGradient));
    painter.setPen(QPen(QColor(100, 100, 100), 1));
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
    // Current value display
    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.setFont(QFont("Arial", 14, QFont::Bold));

    QString valueText = QString::number(qRound(m_currentValue));
    QRectF valueRect(center.x() - 30, center.y() + 25, 60, 20);
    painter.drawText(valueRect, Qt::AlignCenter, valueText);

    // Units
    painter.setFont(QFont("Arial", 8));
    painter.setPen(QPen(QColor(180, 180, 180)));
    QRectF unitsRect(center.x() - 40, center.y() + 45, 80, 15);
    painter.drawText(unitsRect, Qt::AlignCenter, "gCO₂/kWh");

    // Status text
    QString status = getIntensityStatus();
    QColor statusColor = getStatusColor();
    painter.setPen(QPen(statusColor));
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QRectF statusRect(center.x() - 40, center.y() - 50, 80, 15);
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
