#ifndef MAPBOXWIDGET_H
#define MAPBOXWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QLabel>

class QWebEngineView;

class MapboxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapboxWidget(QWidget *parent = nullptr);
    ~MapboxWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QWebEngineView *m_view;

    QFrame *m_weatherBox;
    QLabel *m_weatherLabel;
};

#endif // MAPBOXWIDGET_H
