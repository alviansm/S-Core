#include "DialogAddCargo.h"
#include "ui_DialogAddCargo.h"
#include <QMessageBox>

DialogAddCargo::DialogAddCargo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddCargo)
    , m_loadingPort(new DialogBrowsePort(this))
    , m_dischargingPort(new DialogBrowsePort(this))
{
    ui->setupUi(this);

    // Handle pemilihan FROM port
    connect(m_loadingPort, &DialogBrowsePort::selectedPortId,
            this, [this](int portId, const QString &portName){
                ui->lineEditLoadingPort->setText(portName);
                ui->lineEditLoadingPort->setProperty("portId", portId);
                qDebug() << "From Port selected:" << portId << portName;
            });

    // Handle pemilihan DESTINATION port
    connect(m_dischargingPort, &DialogBrowsePort::selectedPortId,
            this, [this](int portId, const QString &portName){
                ui->lineEditDischargingPort->setText(portName);
                ui->lineEditDischargingPort->setProperty("portId", portId);
                qDebug() << "Dest Port selected:" << portId << portName;
            });

    // Connect on button box accepted
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        QString name = ui->lineEditName->text();
        QString loadingPort = ui->lineEditLoadingPort->text();
        QString dischargingPort = ui->lineEditDischargingPort->text();
        QString quantity = ui->lineEditQuantity->text();
        QString units = ui->comboBoxUnits->currentText();

        if (name.isEmpty() || loadingPort.isEmpty() || dischargingPort.isEmpty() || quantity.isEmpty()) {
            // Validasi sederhana
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        emit onCargoAdded(name, loadingPort, dischargingPort, quantity, units);

        qDebug() << "Cargo Added:"
                 << "Name:" << name
                 << "Loading Port:" << loadingPort
                 << "Discharging Port:" << dischargingPort
                 << "Quantity:" << quantity
                 << "Units:" << units;

        accept();
    });
}

DialogAddCargo::~DialogAddCargo()
{
    delete ui;
}


void DialogAddCargo::on_browseLoadingPort_clicked()
{
    m_loadingPort->setWindowTitle("Browse From Port");
    m_loadingPort->exec();
}


void DialogAddCargo::on_browseDestPort_clicked()
{
    m_dischargingPort->setWindowTitle("Browse Destination Port");
    m_dischargingPort->exec();
}

