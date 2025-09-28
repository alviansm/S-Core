#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QPushButton>
#include <QToolTip>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QDialog>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QLegend>
#include <QPdfView>
#include <QPdfDocument>
#include <QDateTime>
#include <QRandomGenerator>

QT_USE_NAMESPACE

namespace Ui {
class HistoryPage;
}

// Custom widget for heat map cells
class HeatMapCell : public QFrame
{
    Q_OBJECT

public:
    explicit HeatMapCell(const QString &time, const QString &date,
                        double emission, double power, double saving, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void showDetails();

private:
    QString m_time;
    QString m_date;
    double m_emission;
    double m_power;
    double m_saving;
    QColor getColorForEmission(double emission);
};

// Details dialog for heat map cells
class EmissionDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmissionDetailsDialog(const QString &time, const QString &date,
                                 double emission, double power, double saving,
                                 QWidget *parent = nullptr);
};

class HistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPage(QWidget *parent = nullptr);
    ~HistoryPage();

private slots:
    void onNavigationSelectionChanged();
    void onDCSComponentChanged();
    void onReportSelectionChanged();
    void updateDCSChart();

private:
    void setupNavigationAndContent();
    void setupDCSPage();
    void setupHistoricalPage();
    void setupReportPage();
    void createHeatMapGrid();
    QWidget* createDCSPage();
    QWidget* createHistoricalPage();
    QWidget* createReportPage();
    void generateDummyChartData();

    Ui::HistoryPage *ui;

    // DCS related
    QComboBox *m_dcsComponentCombo;
    QChartView *m_chartView;
    QChart *m_chart;
    QTimer *m_updateTimer;

    // Report related
    QComboBox *m_reportCombo;
    QPdfView *m_pdfView;
    QPdfDocument *m_pdfDocument;

    // Heat map related
    QGridLayout *m_heatMapLayout;
    QScrollArea *m_heatMapScrollArea;
};

#endif // HISTORYPAGE_H
