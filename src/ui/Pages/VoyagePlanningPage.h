#ifndef VOYAGEPLANNINGPAGE_H
#define VOYAGEPLANNINGPAGE_H

#include <QWidget>

namespace Ui {
class VoyagePlanningPage;
}

class VoyagePlanningPage : public QWidget
{
    Q_OBJECT

public:
    explicit VoyagePlanningPage(QWidget *parent = nullptr);
    ~VoyagePlanningPage();

private:
    Ui::VoyagePlanningPage *ui;
};

#endif // VOYAGEPLANNINGPAGE_H
