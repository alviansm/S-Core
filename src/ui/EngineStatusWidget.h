#ifndef ENGINESTATUSWIDGET_H
#define ENGINESTATUSWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>

class EngineStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EngineStatusWidget(const QString& engineName, QWidget* parent = nullptr);
    void setEfficiency(double efficiency);
    void setRunningHours(double hours);
    void setSFOC(double sfoc);

private:
    QProgressBar* m_efficiencyBar;
    QLabel* m_efficiencyLabel;
    QLabel* m_hoursLabel;
    QLabel* m_sfocLabel;
    QString m_engineName;
};

#endif // ENGINESTATUSWIDGET_H
