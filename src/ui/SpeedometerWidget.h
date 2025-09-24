#ifndef SPEEDOMETERWIDGET_H
#define SPEEDOMETERWIDGET_H

#include <QObject>
#include <QWidget>

class SpeedometerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedometerWidget(QWidget* parent = nullptr);
    void setValue(double value);
    void setRange(double min, double max);
    void setTitle(const QString& title);
    void setUnit(const QString& unit);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double m_value = 0.0;
    double m_minValue = 0.0;
    double m_maxValue = 100.0;
    QString m_title = "Thermal Efficiency";
    QString m_unit = "%";

    QColor getValueColor(double normalizedValue) const;
};

#endif // SPEEDOMETERWIDGET_H
