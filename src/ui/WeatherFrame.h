#ifndef WEATHERFRAME_H
#define WEATHERFRAME_H

#include <QFrame>

namespace Ui {
class WeatherFrame;
}

class WeatherFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WeatherFrame(QWidget *parent = nullptr);
    ~WeatherFrame();

private:
    Ui::WeatherFrame *ui;
};

#endif // WEATHERFRAME_H
