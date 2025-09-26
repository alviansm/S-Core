#ifndef DIALOGADDNEWVOYAGE_H
#define DIALOGADDNEWVOYAGE_H

#include <QDialog>

namespace Ui {
class DialogAddNewVoyage;
}

class DialogAddNewVoyage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddNewVoyage(QWidget *parent = nullptr);
    ~DialogAddNewVoyage();

signals:
    void voyageAdded(QString name);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogAddNewVoyage *ui;
};

#endif // DIALOGADDNEWVOYAGE_H
