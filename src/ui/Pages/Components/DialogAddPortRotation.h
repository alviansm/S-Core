#ifndef DIALOGADDPORTROTATION_H
#define DIALOGADDPORTROTATION_H

#include <QDialog>

namespace Ui {
class DialogAddPortRotation;
}

class DialogAddPortRotation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddPortRotation(QWidget *parent = nullptr);
    ~DialogAddPortRotation();

private:
    Ui::DialogAddPortRotation *ui;
};

#endif // DIALOGADDPORTROTATION_H
