#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QScrollArea>

namespace Ui {
class SettingPage;
}

class SettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    ~SettingPage();

private slots:
    void onSaveSettings();
    void onResetSettings();

private:
    Ui::SettingPage *ui;

    // UI Components
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    QVBoxLayout *mainLayout;

    // General Settings
    QLineEdit *shipNameEdit;
    QLineEdit *imoNumberEdit;
    QComboBox *shipTypeCombo;

    // IoT Settings
    QLineEdit *iotServerEdit;
    QSpinBox *iotPortSpin;
    QSpinBox *updateIntervalSpin;

    // AI Settings
    QCheckBox *aiOptimizationCheck;
    QCheckBox *routePredictionCheck;
    QCheckBox *carbonAnalysisCheck;

    // Display Settings
    QComboBox *languageCombo;
    QComboBox *unitSystemCombo;

    // Buttons
    QPushButton *saveButton;
    QPushButton *resetButton;

    void setupUI();
    void applyDarkTheme();
    QGroupBox* createGeneralGroup();
    QGroupBox* createIoTGroup();
    QGroupBox* createAIGroup();
    QGroupBox* createDisplayGroup();
};

#endif // SETTINGPAGE_H
