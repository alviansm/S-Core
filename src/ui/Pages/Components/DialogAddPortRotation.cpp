#include "DialogAddPortRotation.h"
#include "ui_DialogAddPortRotation.h"
#include <QMessageBox>

DialogAddPortRotation::DialogAddPortRotation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddPortRotation)
    , m_fromPort(new DialogBrowsePort(this))   // ✅ inisialisasi
    , m_destPort(new DialogBrowsePort(this))   // ✅ inisialisasi
{
    ui->setupUi(this);

    setDateToToday(ui->dateTimeEditETA);
    setDateToToday(ui->dateTimeEditETD);

    // Handle pemilihan FROM port
    connect(m_fromPort, &DialogBrowsePort::selectedPortId,
            this, [this](int portId, const QString &portName){
                ui->lineEdit->setText(portName);
                ui->lineEdit->setProperty("portId", portId);
                qDebug() << "From Port selected:" << portId << portName;
            });

    // Handle pemilihan DESTINATION port
    connect(m_destPort, &DialogBrowsePort::selectedPortId,
            this, [this](int portId, const QString &portName){
                ui->lineEdit_2->setText(portName);
                ui->lineEdit_2->setProperty("portId", portId);
                qDebug() << "Dest Port selected:" << portId << portName;
            });

    // Connect on button box accepted
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        QString fromPort = ui->lineEdit->text();
        int fromPortId = ui->lineEdit->property("portId").toInt();
        QString destPort = ui->lineEdit_2->text();
        int destPortId = ui->lineEdit_2->property("portId").toInt();
        QString eta = ui->dateTimeEditETA->dateTime().toString("dd MMM yyyy HH:mm");
        QString etd = ui->dateTimeEditETD->dateTime().toString("dd MMM yyyy HH:mm");

        if (fromPort.isEmpty() || fromPortId <= 0 ||
            destPort.isEmpty() || destPortId <= 0) {
            // Validasi sederhana
            QMessageBox::warning(this, "Input Error", "Please select both From and Destination ports.");
            return;
        }

        emit onPortRotationAdded(fromPort, fromPortId, destPort, destPortId, eta, etd);

        // qDebug() << "Port Rotation Added:"
        //          << "From:" << fromPort << "(" << fromPortId << ")"
        //          << "To:" << destPort << "(" << destPortId << ")"
        //          << "ETA:" << eta
        //          << "ETD:" << etd;

        accept();
    });
}

DialogAddPortRotation::~DialogAddPortRotation()
{
    delete ui;
}

void DialogAddPortRotation::setDateToToday(QDateTimeEdit *editor)
{
    if (!editor) return;

    QDate today = QDate::currentDate();
    QTime time = editor->dateTime().time();

    editor->setDateTime(QDateTime::currentDateTime());
}

void DialogAddPortRotation::on_browseFromPort_clicked()
{
    m_fromPort->setWindowTitle("Browse From Port");
    m_fromPort->exec();
}

void DialogAddPortRotation::on_browseDestPort_clicked()
{
    m_destPort->setWindowTitle("Browse Destination Port");
    m_destPort->exec();
}

void DialogAddPortRotation::on_buttonBox_accepted()
{
}

