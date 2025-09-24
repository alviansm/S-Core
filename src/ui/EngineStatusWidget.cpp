#include "EngineStatusWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

EngineStatusWidget::EngineStatusWidget(const QString& engineName, QWidget* parent)
    : QWidget(parent), m_engineName(engineName)
{
    setFixedHeight(65);

    // Dark theme background
    setStyleSheet(
        "EngineStatusWidget {"
        "    background-color: #2b2b2b;"
        "    border: 1px solid #404040;"
        "    border-radius: 4px;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 6, 8, 6);
    mainLayout->setSpacing(6);

    // Top section - Progress bar (thinner)
    m_efficiencyBar = new QProgressBar();
    m_efficiencyBar->setFixedHeight(12);
    m_efficiencyBar->setRange(0, 100);
    m_efficiencyBar->setTextVisible(false);
    m_efficiencyBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid #555555;"
        "    border-radius: 2px;"
        "    background-color: #3c3c3c;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #28a745;"
        "    border-radius: 1px;"
        "}"
    );

    // Bottom section - Labels in horizontal layout
    QHBoxLayout* labelsLayout = new QHBoxLayout();
    labelsLayout->setContentsMargins(0, 0, 0, 0);
    labelsLayout->setSpacing(8);

    // Engine name
    QLabel* nameLabel = new QLabel(engineName);
    nameLabel->setStyleSheet("color: white; font-weight: bold; font-size: 11px;");
    nameLabel->setFixedWidth(35);

    m_efficiencyLabel = new QLabel("42.8%");
    m_efficiencyLabel->setStyleSheet("color: #28a745; font-size: 10px; font-weight: bold;");
    m_efficiencyLabel->setFixedWidth(40);

    m_hoursLabel = new QLabel("18.5h");
    m_hoursLabel->setStyleSheet("color: #cccccc; font-size: 10px;");
    m_hoursLabel->setFixedWidth(35);

    m_sfocLabel = new QLabel("186 g/kWh");
    m_sfocLabel->setStyleSheet("color: #cccccc; font-size: 10px;");
    m_sfocLabel->setFixedWidth(60);

    labelsLayout->addWidget(nameLabel);
    labelsLayout->addWidget(m_efficiencyLabel);
    labelsLayout->addWidget(m_hoursLabel);
    labelsLayout->addWidget(m_sfocLabel);
    labelsLayout->addStretch();

    mainLayout->addWidget(m_efficiencyBar);
    mainLayout->addLayout(labelsLayout);

    // Set initial values
    setEfficiency(42.8);
    setRunningHours(18.5);
    setSFOC(186);
}

void EngineStatusWidget::setEfficiency(double efficiency)
{
    m_efficiencyBar->setValue(static_cast<int>(efficiency));
    m_efficiencyLabel->setText(QString::number(efficiency, 'f', 1) + "%");

    // Keep the efficiency label color consistent
    m_efficiencyLabel->setStyleSheet("color: #28a745; font-size: 10px; font-weight: bold;");
}

void EngineStatusWidget::setRunningHours(double hours)
{
    m_hoursLabel->setText(QString::number(hours, 'f', 1) + "h");
}

void EngineStatusWidget::setSFOC(double sfoc)
{
    m_sfocLabel->setText(QString::number(sfoc, 'f', 0) + " g/kWh");
}
