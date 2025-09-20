#include "DashboardPage.h"
#include "ui_DashboardPage.h"
#include "MapboxWidget.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>
#include "CircleProgressBar.h"

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardPage)
{
    ui->setupUi(this);

    // Layout untuk map
    QVBoxLayout *layout = new QVBoxLayout(ui->widget);
    QVBoxLayout *kpi1Layout = new QVBoxLayout(ui->widget_2);

    CircleProgressBar *circleBar1 = new CircleProgressBar(this);
    circleBar1->setText("Load");
    kpi1Layout->addWidget(circleBar1);


    // Tambahkan Mapbox
    MapboxWidget *mapboxWidget = new MapboxWidget(ui->widget);
    layout->addWidget(mapboxWidget);

    // Timer untuk animasi
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [=]() {
        auto animate = [](CircleProgressBar *bar) {
            int newValue = rand() % 100;
            QPropertyAnimation *anim = new QPropertyAnimation(bar, "value");
            anim->setDuration(1000);
            anim->setStartValue(bar->value());
            anim->setEndValue(newValue);
            anim->setEasingCurve(QEasingCurve::InOutCubic);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        };
        animate(circleBar1);
    });
    timer->start(3000);
}

DashboardPage::~DashboardPage()
{
    delete ui;
}
