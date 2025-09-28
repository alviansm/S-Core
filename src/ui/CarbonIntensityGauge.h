#ifndef CARBONINTENSITYGAUGE_H
#define CARBONINTENSITYGAUGE_H

#include <QtWidgets>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <cmath>

#include <QtWidgets>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <cmath>

class CarbonIntensityGauge : public QWidget
{
    Q_OBJECT

public:
    enum IntensityLevel {
        LOW = 0,
        MEDIUM = 1,
        HIGH = 2,
        VERY_HIGH = 3
    };

    explicit CarbonIntensityGauge(QWidget *parent = nullptr);
    void setValue(double value);
    double value() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAnimation();

private:
    void drawIntensityZones(QPainter &painter, const QPointF &center, double radius);
    void drawScale(QPainter &painter, const QPointF &center, double radius);
    void drawNeedle(QPainter &painter, const QPointF &center, double radius);
    void drawCenterInfo(QPainter &painter, const QPointF &center);
    QString getIntensityStatus() const;
    QColor getStatusColor() const;

private:
    double m_currentValue;
    double m_targetValue;
    double m_animatedValue;
    QTimer *m_animationTimer;
};

#endif // CARBONINTENSITYGAUGE_H
