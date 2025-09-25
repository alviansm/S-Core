#ifndef ALERTANDRECOMENDATIONFRAME_H
#define ALERTANDRECOMENDATIONFRAME_H

#include <QFrame>

namespace Ui {
class AlertAndRecomendationFrame;
}

class AlertAndRecomendationFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AlertAndRecomendationFrame(QWidget *parent = nullptr);
    ~AlertAndRecomendationFrame();

private:
    Ui::AlertAndRecomendationFrame *ui;
};

#endif // ALERTANDRECOMENDATIONFRAME_H
