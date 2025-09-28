#ifndef WEATHERFRAME_H
#define WEATHERFRAME_H

#include <QFrame>
#include <QGraphicsView>
#include <QLabel>
#include <QPointer>

namespace Ui {
class WeatherFrame;
}

class WeatherFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WeatherFrame(QWidget *parent = nullptr);
    ~WeatherFrame();

public:
    QPointer<QGraphicsView> graphicsViewWindSpeed() const;
    QPointer<QLabel> labelWindSpeedValue() const;
    QPointer<QLabel> labelHumidityValue() const;
    QPointer<QGraphicsView> graphicsViewHumidity() const;
    QPointer<QGraphicsView> graphicsViewPressure() const;
    QPointer<QLabel> labelPressureValue() const;
    QPointer<QGraphicsView> graphicsViewTemperature() const;
    QPointer<QLabel> labelTemperatureValue() const;

private:
    void setupWidget();
    void applyStylesheet_dark();

private:
    Ui::WeatherFrame *ui;

    // From .ui (Desinger)

    // Wind Speed
    QPointer<QFrame> m_frameWind;
    QPointer<QGraphicsView> m_graphicsViewWindSpeed;
    QPointer<QLabel> m_labelWindSpeedUnit;
    QPointer<QLabel> m_labelWindSpeedValue;

    // Humidity
    QPointer<QFrame> m_frameHumidity;
    QPointer<QGraphicsView> m_graphicsViewHumidity;
    QPointer<QLabel> m_labelHumidityUnit;
    QPointer<QLabel> m_labelHumidityValue;

    // Atmospheric Pressure
    QPointer<QFrame> m_framePressure;
    QPointer<QGraphicsView> m_graphicsViewPressure;
    QPointer<QLabel> m_labelPressureUnit;
    QPointer<QLabel> m_labelPressureValue;

    // Temperature
    QPointer<QFrame> m_frameTemperature;
    QPointer<QGraphicsView> m_graphicsViewTemperature;
    QPointer<QLabel> m_labelTemperatureUnit;
    QPointer<QLabel> m_labelTemperatureValue;

};

#endif // WEATHERFRAME_H
