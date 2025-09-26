#ifndef DIALOGADDCARGO_H
#define DIALOGADDCARGO_H

#include <QDialog>
#include "DialogBrowsePort.h"

namespace Ui {
class DialogAddCargo;
}

class DialogAddCargo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddCargo(QWidget *parent = nullptr);
    ~DialogAddCargo();

signals:
    void onCargoAdded(QString name, QString loadingPort, QString dischargingPort, QString quantity, QString units);

private slots:
    void on_browseLoadingPort_clicked();

    void on_browseDestPort_clicked();

private:
    Ui::DialogAddCargo *ui;

    DialogBrowsePort* m_loadingPort;
    DialogBrowsePort* m_dischargingPort;
};

#endif // DIALOGADDCARGO_H
