#include "WeatherFrame.h"
#include "ui_WeatherFrame.h"

WeatherFrame::WeatherFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::WeatherFrame)
{
    ui->setupUi(this);

    setupWidget();
    applyStylesheet_dark();
}

WeatherFrame::~WeatherFrame()
{
    delete ui;
}

void WeatherFrame::setupWidget()
{
    // Wind Speed
    m_frameWind = ui->frameWind;
    m_graphicsViewWindSpeed = ui->graphicsViewWindSpeed;
    m_labelWindSpeedUnit = ui->labelWindSpeedUnit;
    m_labelWindSpeedValue = ui->labelWindSpeedValue;

    // Humidity
    m_frameHumidity = ui->frameHumidity;
    m_graphicsViewHumidity = ui->graphicsViewHumidity;
    m_labelHumidityUnit = ui->labelHumidityUnit;
    m_labelHumidityValue = ui->labelHumidityValue;

    // Atmospheric Pressure
    m_framePressure = ui->framePressure;
    m_graphicsViewPressure = ui->graphicsViewPressure;
    m_labelPressureUnit = ui->labelPressureUnit;
    m_labelPressureValue = ui->labelPressureValue;

    // Temperature
    m_frameTemperature = ui->frameTemperature;
    m_graphicsViewTemperature = ui->graphicsViewTemperature;
    m_labelTemperatureUnit = ui->labelTemperatureUnit;
    m_labelTemperatureValue = ui->labelTemperatureValue;
}

void WeatherFrame::applyStylesheet_dark()
{
    //
    // 1. Parent Frame (whole overlay)
    //
    QString frameStylesheet = R"(
        QFrame {
            background-color: rgba(30, 30, 30, 220); /* semi-transparent dark */
            border: 1px solid #555555;
            border-radius: 8px;
        }
    )";
    this->setStyleSheet(frameStylesheet);

    //
    // 2. Sub-Frames (wind / humidity / pressure / temperature)
    //
    QString subFrameStylesheet = R"(
        QFrame {
            background-color: #2b2b2b;      /* slightly lighter than parent */
            border: 1px solid #444444;
            border-radius: 6px;
            margin: 4px;                    /* breathing room inside parent */
        }
    )";

    m_frameWind->setStyleSheet(subFrameStylesheet);
    m_frameHumidity->setStyleSheet(subFrameStylesheet);
    m_framePressure->setStyleSheet(subFrameStylesheet);
    m_frameTemperature->setStyleSheet(subFrameStylesheet);

    //
    // 3. Labels (value + unit)
    //
    QString labelValueStylesheet = R"(
        QLabel {
            color: #f0f0f0;                 /* bright text */
            background: transparent;
            font-size: 16px;
            font-weight: bold;
            border: none;
        }
    )";

    QString labelUnitStylesheet = R"(
        QLabel {
            color: #aaaaaa;                 /* softer text for units */
            background: transparent;
            font-size: 12px;
            border: none;
        }
    )";

    // Apply to all labels
    m_labelWindSpeedValue->setStyleSheet(labelValueStylesheet);
    m_labelHumidityValue->setStyleSheet(labelValueStylesheet);
    m_labelPressureValue->setStyleSheet(labelValueStylesheet);
    m_labelTemperatureValue->setStyleSheet(labelValueStylesheet);

    m_labelWindSpeedUnit->setStyleSheet(labelUnitStylesheet);
    m_labelHumidityUnit->setStyleSheet(labelUnitStylesheet);
    m_labelPressureUnit->setStyleSheet(labelUnitStylesheet);
    m_labelTemperatureUnit->setStyleSheet(labelUnitStylesheet);

    //
    // 4. Graphics Views (wind / humidity / pressure / temperature)
    //
    QString graphicsViewStylesheet = R"(
        QGraphicsView {
            background-color: #1e1e1e;     /* darkest background */
            border: 1px solid #333333;
        }
    )";

    m_graphicsViewWindSpeed->setStyleSheet(graphicsViewStylesheet);
    m_graphicsViewHumidity->setStyleSheet(graphicsViewStylesheet);
    m_graphicsViewPressure->setStyleSheet(graphicsViewStylesheet);
    m_graphicsViewTemperature->setStyleSheet(graphicsViewStylesheet);
}

QPointer<QLabel> WeatherFrame::labelTemperatureValue() const
{
    return m_labelTemperatureValue;
}

QPointer<QGraphicsView> WeatherFrame::graphicsViewTemperature() const
{
    return m_graphicsViewTemperature;
}

QPointer<QLabel> WeatherFrame::labelPressureValue() const
{
    return m_labelPressureValue;
}

QPointer<QGraphicsView> WeatherFrame::graphicsViewPressure() const
{
    return m_graphicsViewPressure;
}

QPointer<QGraphicsView> WeatherFrame::graphicsViewHumidity() const
{
    return m_graphicsViewHumidity;
}

QPointer<QLabel> WeatherFrame::labelHumidityValue() const
{
    return m_labelHumidityValue;
}

QPointer<QLabel> WeatherFrame::labelWindSpeedValue() const
{
    return m_labelWindSpeedValue;
}

QPointer<QGraphicsView> WeatherFrame::graphicsViewWindSpeed() const
{
    return m_graphicsViewWindSpeed;
}

