#include "AlertAndRecomendationFrame.h"
#include "ui_AlertAndRecomendationFrame.h"

AlertAndRecomendationFrame::AlertAndRecomendationFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::AlertAndRecomendationFrame)
{
    ui->setupUi(this);

    this->setMinimumSize(220, 111);
    this->setMaximumSize(220, 111);

    setupWidget();
    applyStylesheet_dark();
}

AlertAndRecomendationFrame::~AlertAndRecomendationFrame()
{
    delete ui;
}

void AlertAndRecomendationFrame::setupWidget()
{
    m_listWidget = ui->listWidget;
    m_titleLabel = ui->titleLabel;
}

void AlertAndRecomendationFrame::applyStylesheet_dark()
{
    // Dark style for QListWidget
    QString stylesheetListWidget = R"(
        QListWidget {
            background-color: #2b2b2b;
            color: #f0f0f0;
            border: 1px solid #444444;
            selection-background-color: #3d3d3d;
            selection-color: #ffffff;
        }
        QListWidget::item {
            padding: 5px;
        }
        QListWidget::item:selected {
            background-color: #505050;
            color: #ffffff;
        }
    )";
    m_listWidget->setStyleSheet(stylesheetListWidget);

    QString stylesheetLabel = R"(
        QLabel {
            color: #f0f0f0;
            background: transparent;
            font-size: 16px;
            font-weight: bold;
            border: none; /* ensure no border */
        }
    )";
    m_titleLabel->setStyleSheet(stylesheetLabel);

    // Dark style for the frame itself (overlay)
    QString stylesheetFrame = R"(
        QFrame {
            background-color: rgba(30, 30, 30, 220);
            border: 1px solid #555555;
            border-radius: 6px;
        }
    )";
    this->setStyleSheet(stylesheetFrame);
}


QPointer<QListWidget> AlertAndRecomendationFrame::listWidget() const
{
    return m_listWidget;
}

