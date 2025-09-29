#include "SettingPage.h"
#include "ui_SettingPage.h"
#include <QMessageBox>

SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingPage)
{
    ui->setupUi(this);
    setupUI();
    applyDarkTheme();
}

SettingPage::~SettingPage()
{
    delete ui;
}

void SettingPage::setupUI()
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title
    QLabel *titleLabel = new QLabel("Settings", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #E0E0E0; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    // Scroll Area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);

    // Add group boxes
    contentLayout->addWidget(createGeneralGroup());
    contentLayout->addWidget(createIoTGroup());
    contentLayout->addWidget(createAIGroup());
    contentLayout->addWidget(createDisplayGroup());

    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    resetButton = new QPushButton("Reset to Default", this);
    resetButton->setMinimumHeight(40);
    resetButton->setMinimumWidth(150);
    connect(resetButton, &QPushButton::clicked, this, &SettingPage::onResetSettings);

    saveButton = new QPushButton("Save Settings", this);
    saveButton->setMinimumHeight(40);
    saveButton->setMinimumWidth(150);
    connect(saveButton, &QPushButton::clicked, this, &SettingPage::onSaveSettings);

    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);
}

QGroupBox* SettingPage::createGeneralGroup()
{
    QGroupBox *group = new QGroupBox("General Settings", this);
    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(12);

    // Ship Name
    QLabel *shipNameLabel = new QLabel("Ship Name:", group);
    shipNameEdit = new QLineEdit(group);
    shipNameEdit->setPlaceholderText("Enter ship name");
    shipNameEdit->setText("MV Example");
    shipNameEdit->setMinimumHeight(35);
    layout->addWidget(shipNameLabel);
    layout->addWidget(shipNameEdit);

    // IMO Number
    QLabel *imoLabel = new QLabel("IMO Number:", group);
    imoNumberEdit = new QLineEdit(group);
    imoNumberEdit->setPlaceholderText("Enter IMO number");
    imoNumberEdit->setText("IMO 1234567");
    imoNumberEdit->setMinimumHeight(35);
    layout->addWidget(imoLabel);
    layout->addWidget(imoNumberEdit);

    // Ship Type
    QLabel *typeLabel = new QLabel("Ship Type:", group);
    shipTypeCombo = new QComboBox(group);
    shipTypeCombo->addItems({"Container Ship", "Bulk Carrier", "Tanker",
                             "LNG Carrier", "General Cargo", "Ro-Ro"});
    shipTypeCombo->setMinimumHeight(35);
    layout->addWidget(typeLabel);
    layout->addWidget(shipTypeCombo);

    return group;
}

QGroupBox* SettingPage::createIoTGroup()
{
    QGroupBox *group = new QGroupBox("IoT Connection Settings", this);
    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(12);

    // Server Address
    QLabel *serverLabel = new QLabel("IoT Server Address:", group);
    iotServerEdit = new QLineEdit(group);
    iotServerEdit->setPlaceholderText("e.g., iot.eeship.com");
    iotServerEdit->setText("iot.eeship.com");
    iotServerEdit->setMinimumHeight(35);
    layout->addWidget(serverLabel);
    layout->addWidget(iotServerEdit);

    // Port
    QLabel *portLabel = new QLabel("Server Port:", group);
    iotPortSpin = new QSpinBox(group);
    iotPortSpin->setRange(1, 65535);
    iotPortSpin->setValue(8883);
    iotPortSpin->setMinimumHeight(35);
    layout->addWidget(portLabel);
    layout->addWidget(iotPortSpin);

    // Update Interval
    QLabel *intervalLabel = new QLabel("Data Update Interval (seconds):", group);
    updateIntervalSpin = new QSpinBox(group);
    updateIntervalSpin->setRange(1, 300);
    updateIntervalSpin->setValue(30);
    updateIntervalSpin->setSuffix(" sec");
    updateIntervalSpin->setMinimumHeight(35);
    layout->addWidget(intervalLabel);
    layout->addWidget(updateIntervalSpin);

    return group;
}

QGroupBox* SettingPage::createAIGroup()
{
    QGroupBox *group = new QGroupBox("AI Decision Support Settings", this);
    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(12);

    // AI Optimization
    aiOptimizationCheck = new QCheckBox("Enable AI-powered Fuel Optimization", group);
    aiOptimizationCheck->setChecked(true);
    aiOptimizationCheck->setMinimumHeight(30);
    layout->addWidget(aiOptimizationCheck);

    // Route Prediction
    routePredictionCheck = new QCheckBox("Enable Real-time Route Prediction", group);
    routePredictionCheck->setChecked(true);
    routePredictionCheck->setMinimumHeight(30);
    layout->addWidget(routePredictionCheck);

    // Carbon Analysis
    carbonAnalysisCheck = new QCheckBox("Enable Carbon Emission Analysis (IMO Compliance)", group);
    carbonAnalysisCheck->setChecked(true);
    carbonAnalysisCheck->setMinimumHeight(30);
    layout->addWidget(carbonAnalysisCheck);

    return group;
}

QGroupBox* SettingPage::createDisplayGroup()
{
    QGroupBox *group = new QGroupBox("Display Settings", this);
    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(12);

    // Language
    QLabel *langLabel = new QLabel("Language:", group);
    languageCombo = new QComboBox(group);
    languageCombo->addItems({"English", "Bahasa Indonesia"});
    languageCombo->setMinimumHeight(35);
    layout->addWidget(langLabel);
    layout->addWidget(languageCombo);

    // Unit System
    QLabel *unitLabel = new QLabel("Unit System:", group);
    unitSystemCombo = new QComboBox(group);
    unitSystemCombo->addItems({"Metric (km, kg, °C)", "Imperial (miles, lbs, °F)", "Nautical (nm, knots)"});
    unitSystemCombo->setCurrentIndex(2);
    unitSystemCombo->setMinimumHeight(35);
    layout->addWidget(unitLabel);
    layout->addWidget(unitSystemCombo);

    return group;
}

void SettingPage::applyDarkTheme()
{
    // Main widget background
    this->setStyleSheet(
        "QWidget {"
        "   background-color: #1E1E1E;"
        "   color: #E0E0E0;"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "   font-size: 13px;"
        "}"
    );

    // ScrollArea
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "   background-color: #1E1E1E;"
        "   border: none;"
        "}"
        "QScrollBar:vertical {"
        "   background-color: #2D2D2D;"
        "   width: 12px;"
        "   border-radius: 6px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background-color: #4A4A4A;"
        "   border-radius: 6px;"
        "   min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background-color: #5A5A5A;"
        "}"
    );

    // GroupBox
    QString groupBoxStyle =
        "QGroupBox {"
        "   background-color: #252525;"
        "   border: 1px solid #3A3A3A;"
        "   border-radius: 8px;"
        "   margin-top: 12px;"
        "   padding-top: 15px;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   color: #00A8E8;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   subcontrol-position: top left;"
        "   left: 15px;"
        "   padding: 0 5px;"
        "   color: #00A8E8;"
        "}";

    // LineEdit
    QString lineEditStyle =
        "QLineEdit {"
        "   background-color: #2D2D2D;"
        "   border: 1px solid #3A3A3A;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   color: #E0E0E0;"
        "   selection-background-color: #00A8E8;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #00A8E8;"
        "}"
        "QLineEdit:hover {"
        "   border: 1px solid #4A4A4A;"
        "}";

    // ComboBox
    QString comboBoxStyle =
        "QComboBox {"
        "   background-color: #2D2D2D;"
        "   border: 1px solid #3A3A3A;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   color: #E0E0E0;"
        "}"
        "QComboBox:hover {"
        "   border: 1px solid #4A4A4A;"
        "}"
        "QComboBox:focus {"
        "   border: 1px solid #00A8E8;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/general/ic-down_arrow.png);"
        "   width: 16px;"
        "   height: 16px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #2D2D2D;"
        "   border: 1px solid #3A3A3A;"
        "   selection-background-color: #00A8E8;"
        "   color: #E0E0E0;"
        "   outline: none;"
        "}";

    // SpinBox
    QString spinBoxStyle =
        "QSpinBox {"
        "   background-color: #2D2D2D;"
        "   border: 1px solid #3A3A3A;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   color: #E0E0E0;"
        "   font-size: 14px;"
        "}"
        "QSpinBox:hover {"
        "   border: 1px solid #4A4A4A;"
        "}"
        "QSpinBox:focus {"
        "   border: 1px solid #00A8E8;"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "   background-color: #3A3A3A;"
        "   border: none;"
        "   width: 20px;"
        "}"
        "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
        "   background-color: #4A4A4A;"
        "}";

    // CheckBox
    QString checkBoxStyle =
        "QCheckBox {"
        "   color: #E0E0E0;"
        "   spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "   width: 20px;"
        "   height: 20px;"
        "   border: 2px solid #3A3A3A;"
        "   border-radius: 4px;"
        "   background-color: #2D2D2D;"
        "}"
        "QCheckBox::indicator:hover {"
        "   border: 2px solid #4A4A4A;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color: #00A8E8;"
        "   border: 2px solid #00A8E8;"
        "}"
        "QCheckBox::indicator:checked:hover {"
        "   background-color: #0098D8;"
        "}";

    // Buttons
    QString buttonStyle =
        "QPushButton {"
        "   background-color: #00A8E8;"
        "   color: #FFFFFF;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0098D8;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0088C8;"
        "}";

    QString resetButtonStyle =
        "QPushButton {"
        "   background-color: #3A3A3A;"
        "   color: #E0E0E0;"
        "   border: 1px solid #4A4A4A;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4A4A4A;"
        "   border: 1px solid #5A5A5A;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2A2A2A;"
        "}";

    // Label
    QString labelStyle =
        "QLabel {"
        "   color: #B0B0B0;"
        "   font-size: 13px;"
        "}";

    // Apply styles to group boxes
    QList<QGroupBox*> groupBoxes = this->findChildren<QGroupBox*>();
    for (QGroupBox *gb : groupBoxes) {
        gb->setStyleSheet(groupBoxStyle);
    }

    // Apply styles to widgets
    QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
    for (QLineEdit *le : lineEdits) {
        le->setStyleSheet(lineEditStyle);
    }

    QList<QComboBox*> comboBoxes = this->findChildren<QComboBox*>();
    for (QComboBox *cb : comboBoxes) {
        cb->setStyleSheet(comboBoxStyle);
    }

    QList<QSpinBox*> spinBoxes = this->findChildren<QSpinBox*>();
    for (QSpinBox *sb : spinBoxes) {
        sb->setStyleSheet(spinBoxStyle);
    }

    QList<QCheckBox*> checkBoxes = this->findChildren<QCheckBox*>();
    for (QCheckBox *chk : checkBoxes) {
        chk->setStyleSheet(checkBoxStyle);
    }

    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel *lbl : labels) {
        if (lbl->text() != "Settings") {  // Don't style the title
            lbl->setStyleSheet(labelStyle);
        }
    }

    saveButton->setStyleSheet(buttonStyle);
    resetButton->setStyleSheet(resetButtonStyle);
}

void SettingPage::onSaveSettings()
{
    // Dummy implementation - in real app, save to QSettings or config file
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Settings Saved");
    msgBox.setText("Settings have been saved successfully!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #2D2D2D;"
        "}"
        "QLabel {"
        "   color: #E0E0E0;"
        "   background-color: transparent;"
        "}"
        "QPushButton {"
        "   background-color: #00A8E8;"
        "   color: #FFFFFF;"
        "   border: none;"
        "   border-radius: 4px;"
        "   padding: 8px 20px;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0098D8;"
        "}"
    );
    msgBox.exec();
}

void SettingPage::onResetSettings()
{
    // Dummy implementation - reset to defaults
    shipNameEdit->setText("MV Example");
    imoNumberEdit->setText("IMO 1234567");
    shipTypeCombo->setCurrentIndex(0);
    iotServerEdit->setText("iot.eeship.com");
    iotPortSpin->setValue(8883);
    updateIntervalSpin->setValue(30);
    aiOptimizationCheck->setChecked(true);
    routePredictionCheck->setChecked(true);
    carbonAnalysisCheck->setChecked(true);
    languageCombo->setCurrentIndex(0);
    unitSystemCombo->setCurrentIndex(2);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Reset Complete");
    msgBox.setText("Settings have been reset to default values.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #2D2D2D;"
        "}"
        "QLabel {"
        "   color: #E0E0E0;"
        "   background-color: transparent;"
        "}"
        "QPushButton {"
        "   background-color: #00A8E8;"
        "   color: #FFFFFF;"
        "   border: none;"
        "   border-radius: 4px;"
        "   padding: 8px 20px;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0098D8;"
        "}"
    );
    msgBox.exec();
}
