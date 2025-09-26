#ifndef KPIOVERVIEWFRAME_H
#define KPIOVERVIEWFRAME_H

#include <QFrame>

namespace Ui {
class KPIOverviewFrame;
}

class KPIOverviewFrame : public QFrame
{
    Q_OBJECT

public:
    explicit KPIOverviewFrame(QWidget *parent = nullptr);
    ~KPIOverviewFrame();

private:
    Ui::KPIOverviewFrame *ui;
};

#endif // KPIOVERVIEWFRAME_H
