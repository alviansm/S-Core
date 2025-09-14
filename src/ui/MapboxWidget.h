#ifndef MAPBOXWIDGET_H
#define MAPBOXWIDGET_H

#include <QWidget>

class QWebEngineView;

class MapboxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapboxWidget(QWidget *parent = nullptr);
    ~MapboxWidget();

private:
    QWebEngineView *m_view;
};

#endif // MAPBOXWIDGET_H
