#ifndef TECHNICALPAGE_H
#define TECHNICALPAGE_H

#include <QWidget>

namespace Ui {
class TechnicalPage;
}

class TechnicalPage : public QWidget
{
    Q_OBJECT

public:
    explicit TechnicalPage(QWidget *parent = nullptr);
    ~TechnicalPage();

private:
    Ui::TechnicalPage *ui;
};

#endif // TECHNICALPAGE_H
