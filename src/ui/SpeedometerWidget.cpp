#include "SpeedometerWidget.h"

#include <QPainter>
#include <QProgressBar>

SpeedometerWidget::SpeedometerWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(200, 120);
    setValue(42.8); // Default thermal efficiency
}

void SpeedometerWidget::setValue(double value)
{
    m_value = qBound(m_minValue, value, m_maxValue);
    update();
}

void SpeedometerWidget::setRange(double min, double max)
{
    m_minValue = min;
    m_maxValue = max;
    m_value = qBound(m_minValue, m_value, m_maxValue);
    update();
}

void SpeedometerWidget::setTitle(const QString& title)
{
    m_title = title;
    update();
}

void SpeedometerWidget::setUnit(const QString& unit)
{
    m_unit = unit;
    update();
}

QColor SpeedometerWidget::getValueColor(double normalizedValue) const
{
    if (normalizedValue < 0.3) return QColor(220, 53, 69);  // Red
    if (normalizedValue < 0.7) return QColor(255, 193, 7);  // Yellow
    return QColor(40, 167, 69);  // Green
}

void SpeedometerWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height() * 2);
    QRect rect(0, 0, side, side);
    rect.moveCenter(QPoint(width() / 2, height()));

    // Background arc
    painter.setPen(QPen(QColor(60, 60, 60), 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(rect.adjusted(4, 4, -4, -4), 0, 180 * 16);

    // Value arc - now starts from left (180 degrees) and goes right
    double normalizedValue = (m_value - m_minValue) / (m_maxValue - m_minValue);
    QColor valueColor = getValueColor(normalizedValue);
    painter.setPen(QPen(valueColor, 8, Qt::SolidLine, Qt::RoundCap));
    // Start angle is 180 degrees (left side), span angle is based on value
    int startAngle = 180 * 16; // Start from left
    int spanAngle = static_cast<int>(180 * 16 * normalizedValue);
    painter.drawArc(rect.adjusted(4, 4, -4, -4), startAngle, -spanAngle); // Negative span to go clockwise

    // Center text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(18);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_value, 'f', 1) + m_unit;
    QRect textRect(0, height() - 40, width(), 30);
    painter.drawText(textRect, Qt::AlignCenter, valueText);

    // Title
    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(QColor(200, 200, 200));
    QRect titleRect(0, height() - 15, width(), 15);
    painter.drawText(titleRect, Qt::AlignCenter, m_title);
}
