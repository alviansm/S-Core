#ifndef DIALOGBROWSEPORT_H
#define DIALOGBROWSEPORT_H

#include <QDialog>

namespace Ui {
class DialogBrowsePort;
}

class DialogBrowsePort : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBrowsePort(QWidget *parent = nullptr);
    ~DialogBrowsePort();

signals:
    void selectedPortId(int portId, QString portName); // Emit when a port is selected)

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogBrowsePort *ui;
};

#endif // DIALOGBROWSEPORT_H
