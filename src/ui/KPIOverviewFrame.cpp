#include "KPIOverviewFrame.h"
#include "ui_KPIOverviewFrame.h"

#include <QStyleOptionSlider>
#include <QStyle>

KPIOverviewFrame::KPIOverviewFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::KPIOverviewFrame)
{
    ui->setupUi(this);
    setupWidget();
}

KPIOverviewFrame::~KPIOverviewFrame()
{
    delete ui;
}

void KPIOverviewFrame::setupWidget()
{
    m_frameEnergyEfficiency = ui->frameEnergyEfficiency;
    m_labelEnergyEfficiency = ui->labelEnergyEfficiency;
    m_graphicsViewEnergyEfficiency = ui->graphicsViewEnergyEfficiency;
    m_progressBarEnergyEfficiency = ui->progressBarEnergyEfficiency;

    m_frameFOConsumption = ui->frameFOConsumption;
    m_labelFOConsumption = ui->labelFOConsumption;
    m_graphicsViewFOConsumption = ui->graphicsViewFOConsumption;
    m_progressBarFOConsumption = ui->progressBarFOConsumption;

    m_frameVoyageCII = ui->frameVoyageCII;
    m_labelVoyageCII = ui->labelVoyageCII;
    m_graphicsViewVoyageCII = ui->graphicsViewVoyageCII;
    m_progressBarVoyageCII = ui->progressBarVoyageCII;

    setupWidget_sliderLabel();
    applyStylesheet_dark();
}

void KPIOverviewFrame::setupWidget_sliderLabel()
{
    // Setup Energy Efficiency progress bar
    if (m_progressBarEnergyEfficiency) {
        m_progressBarEnergyEfficiency->setMinimum(0);
        m_progressBarEnergyEfficiency->setMaximum(100);
        m_progressBarEnergyEfficiency->setValue(75); // Example value
        m_progressBarEnergyEfficiency->setTextVisible(true);
        m_progressBarEnergyEfficiency->setFormat("%v%");
    }

    // Setup FO Consumption progress bar
    if (m_progressBarFOConsumption) {
        m_progressBarFOConsumption->setMinimum(0);
        m_progressBarFOConsumption->setMaximum(100);
        m_progressBarFOConsumption->setValue(60); // Example value
        m_progressBarFOConsumption->setTextVisible(true);
        m_progressBarFOConsumption->setFormat("%v%");
    }

    // Setup Voyage CII progress bar
    if (m_progressBarVoyageCII) {
        m_progressBarVoyageCII->setMinimum(0);
        m_progressBarVoyageCII->setMaximum(100);
        m_progressBarVoyageCII->setValue(85); // Example value
        m_progressBarVoyageCII->setTextVisible(true);
        m_progressBarVoyageCII->setFormat("%v%");
    }
}

void KPIOverviewFrame::applyStylesheet_dark()
{
    // Dark style for QLabels
    QString stylesheetLabel = R"(
        QLabel {
            color: #f0f0f0;
            background: transparent;
            font-size: 9px;
            border: none;
        }
    )";
    if (m_labelEnergyEfficiency) m_labelEnergyEfficiency->setStyleSheet(stylesheetLabel);
    if (m_labelFOConsumption) m_labelFOConsumption->setStyleSheet(stylesheetLabel);
    if (m_labelVoyageCII) m_labelVoyageCII->setStyleSheet(stylesheetLabel);

    // Dark style for KPI container frames
    QString stylesheetKPIFrame = R"(
        QFrame {
            background-color: #3a3a3a;
            border: 1px solid #555555;
            border-radius: 8px;
        }
    )";
    if (m_frameEnergyEfficiency) m_frameEnergyEfficiency->setStyleSheet(stylesheetKPIFrame);
    if (m_frameFOConsumption) m_frameFOConsumption->setStyleSheet(stylesheetKPIFrame);
    if (m_frameVoyageCII) m_frameVoyageCII->setStyleSheet(stylesheetKPIFrame);

    // Dark style for QGraphicsView
    QString stylesheetGraphicsView = R"(
        QGraphicsView {
            background-color: #2b2b2b;
            border: 1px solid #444444;
            border-radius: 4px;
        }
    )";
    if (m_graphicsViewEnergyEfficiency) m_graphicsViewEnergyEfficiency->setStyleSheet(stylesheetGraphicsView);
    if (m_graphicsViewFOConsumption) m_graphicsViewFOConsumption->setStyleSheet(stylesheetGraphicsView);
    if (m_graphicsViewVoyageCII) m_graphicsViewVoyageCII->setStyleSheet(stylesheetGraphicsView);

    // Dark style for QProgressBar
    QString stylesheetProgressBar = R"(
        QProgressBar {
            border: 1px solid #555555;
            border-radius: 8px;
            background-color: #2b2b2b;
            text-align: center;
            color: #f0f0f0;
            font-weight: bold;
            font-size: 12px;
            padding: 2px;
        }
        QProgressBar::chunk {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #0078d4, stop:1 #106ebe);
            border-radius: 6px;
            margin: 1px;
        }
        QProgressBar::chunk:disabled {
            background-color: #555555;
        }
    )";
    if (m_progressBarEnergyEfficiency) m_progressBarEnergyEfficiency->setStyleSheet(stylesheetProgressBar);
    if (m_progressBarFOConsumption) m_progressBarFOConsumption->setStyleSheet(stylesheetProgressBar);
    if (m_progressBarVoyageCII) m_progressBarVoyageCII->setStyleSheet(stylesheetProgressBar);

    // Dark style for the main frame (KPIOverviewFrame itself)
    QString stylesheetMainFrame = R"(
        QFrame {
            background-color: rgba(30, 30, 30, 220);
            border: 1px solid #555555;
            border-radius: 6px;
        }
    )";
    this->setStyleSheet(stylesheetMainFrame);
}


QPointer<QGraphicsView> KPIOverviewFrame::graphicsViewVoyageCII() const
{
    return m_graphicsViewVoyageCII;
}

QPointer<QGraphicsView> KPIOverviewFrame::graphicsViewFOConsumption() const
{
    return m_graphicsViewFOConsumption;
}

QPointer<QGraphicsView> KPIOverviewFrame::graphicsViewEnergyEfficiency() const
{
    return m_graphicsViewEnergyEfficiency;
}
