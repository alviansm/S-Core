#include "CircleProgressBar.h"
#include <QPainter>
#include <QConicalGradient>

CircleProgressBar::CircleProgressBar(QWidget *parent)
    : QWidget(parent),
    m_value(0),
    m_max(100),
    m_text("Load"),
    m_startColor(QColor(0, 191, 255)),   // biru muda
    m_endColor(QColor(186, 85, 211))     // ungu
{
    setMinimumSize(150, 150);
}

void CircleProgressBar::setValue(int value)
{
    if (value != m_value) {
        m_value = qBound(0, value, m_max);
        emit valueChanged(m_value);
        update();
    }
}

void CircleProgressBar::setMaximum(int max)
{
    m_max = max;
    update();
}

void CircleProgressBar::setText(const QString &text)
{
    m_text = text;
    update();
}

void CircleProgressBar::setStartColor(const QColor &color)
{
    if (m_startColor != color) {
        m_startColor = color;
        update();
    }
}

void CircleProgressBar::setEndColor(const QColor &color)
{
    if (m_endColor != color) {
        m_endColor = color;
        update();
    }
}

void CircleProgressBar::setColors(const QColor &startColor, const QColor &endColor)
{
    bool changed = false;
    if (m_startColor != startColor) {
        m_startColor = startColor;
        changed = true;
    }
    if (m_endColor != endColor) {
        m_endColor = endColor;
        changed = true;
    }
    if (changed) {
        update();
    }
}

void CircleProgressBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Background circle
    QPen pen(Qt::gray, 15, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(pen);
    painter.drawArc(-80, -80, 160, 160, 0, 360 * 16);

    // Progress circle dengan gradient
    QConicalGradient gradient(0, 0, -90);
    gradient.setColorAt(0.0, m_startColor);
    gradient.setColorAt(1.0, m_endColor);

    QPen penProgress(QBrush(gradient), 15, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(penProgress);

    double angleSpan = (static_cast<double>(m_value) / m_max) * 360.0;
    painter.drawArc(-80, -80, 160, 160, 90 * 16, -angleSpan * 16);

    // Text angka
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(28);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(-80, -80, 160, 160), Qt::AlignCenter, QString::number(m_value));

    // Text label
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(QRect(-80, 20, 160, 30), Qt::AlignCenter, m_text);
}
