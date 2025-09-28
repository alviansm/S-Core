#ifndef DIALOGADDPORTROTATION_H
#define DIALOGADDPORTROTATION_H

#include <QDialog>
#include "DialogBrowsePort.h"
#include <QDateTimeEdit>

namespace Ui {
class DialogAddPortRotation;
}

class DialogAddPortRotation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddPortRotation(QWidget *parent = nullptr);
    ~DialogAddPortRotation();

    void setDateToToday(QDateTimeEdit *editor);

private slots:
    void on_browseFromPort_clicked();

    void on_browseDestPort_clicked();

    void on_buttonBox_accepted();

signals:
    void onPortRotationAdded(QString fromPort, int fromPortId, QString destPort, int destPortId, QString eta, QString etc);

private:
    Ui::DialogAddPortRotation *ui;

    DialogBrowsePort* m_fromPort;
    DialogBrowsePort* m_destPort;

};

#endif // DIALOGADDPORTROTATION_H
