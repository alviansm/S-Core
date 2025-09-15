#ifndef DECISIONSUPPORTPAGE_H
#define DECISIONSUPPORTPAGE_H

#include <QWidget>

namespace Ui {
class DecisionSupportPage;
}

class DecisionSupportPage : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionSupportPage(QWidget *parent = nullptr);
    ~DecisionSupportPage();

private:
    Ui::DecisionSupportPage *ui;
};

#endif // DECISIONSUPPORTPAGE_H
