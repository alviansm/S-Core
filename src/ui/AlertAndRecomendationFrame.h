#ifndef ALERTANDRECOMENDATIONFRAME_H
#define ALERTANDRECOMENDATIONFRAME_H

#include <QFrame>
#include <QPointer>
#include <QLabel>
#include <QListWidget>

namespace Ui {
class AlertAndRecomendationFrame;
}

class AlertAndRecomendationFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AlertAndRecomendationFrame(QWidget *parent = nullptr);
    ~AlertAndRecomendationFrame();

public:
    QPointer<QListWidget> listWidget() const;

private:
    void setupWidget();
    void applyStylesheet_dark();

private:
    Ui::AlertAndRecomendationFrame *ui;

    // From .ui
    QPointer<QListWidget> m_listWidget;
    QPointer<QLabel> m_titleLabel;
};

#endif // ALERTANDRECOMENDATIONFRAME_H
