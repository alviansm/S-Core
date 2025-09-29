#ifndef KPIOVERVIEWFRAME_H
#define KPIOVERVIEWFRAME_H

#include <QFrame>
#include <QGraphicsView>
#include <QProgressBar>
#include <QPointer>
#include <QLabel>

namespace Ui {
class KPIOverviewFrame;
}

class KPIOverviewFrame : public QFrame
{
    Q_OBJECT

public:
    explicit KPIOverviewFrame(QWidget *parent = nullptr);
    ~KPIOverviewFrame();

// Getters
public:
    QPointer<QGraphicsView> graphicsViewEnergyEfficiency() const;
    QPointer<QGraphicsView> graphicsViewFOConsumption() const;
    QPointer<QGraphicsView> graphicsViewVoyageCII() const;

    QPointer<QLabel> labelEnergyEfficiency() const;
    QPointer<QLabel> labelFOConsumption() const;
    QPointer<QLabel> labelVoyageCII() const;

    QPointer<QProgressBar> progressBarEnergyEfficiency() const;
    QPointer<QProgressBar> progressBarFOConsumption() const;
    QPointer<QProgressBar> progressBarVoyageCII() const;

private:
    void setupWidget();

    void setupWidget_sliderLabel();
    void applyStylesheet_dark();

private:
    Ui::KPIOverviewFrame *ui;

    // From .ui
    QPointer<QFrame> m_frameEnergyEfficiency;
    QPointer<QLabel> m_labelEnergyEfficiency;
    QPointer<QGraphicsView> m_graphicsViewEnergyEfficiency;
    QPointer<QProgressBar> m_progressBarEnergyEfficiency;

    QPointer<QFrame> m_frameFOConsumption;
    QPointer<QLabel> m_labelFOConsumption;
    QPointer<QGraphicsView> m_graphicsViewFOConsumption;
    QPointer<QProgressBar> m_progressBarFOConsumption;

    QPointer<QFrame> m_frameVoyageCII;
    QPointer<QLabel> m_labelVoyageCII;
    QPointer<QGraphicsView> m_graphicsViewVoyageCII;
    QPointer<QProgressBar> m_progressBarVoyageCII;
};

#endif // KPIOVERVIEWFRAME_H
