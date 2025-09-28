#include "DialogBrowsePort.h"
#include "ui_DialogBrowsePort.h"
#include "../service/PortSearch.h"

DialogBrowsePort::DialogBrowsePort(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogBrowsePort)
{
    ui->setupUi(this);

    // Style for list widget
    QString styleSheet = R"(
        QListWidget {
            background-color: #333333;
            border: none;
            outline: 0;
        }
        QListWidget::item {
            background-color: #4F4F4F;
            color: white;
            padding: 10px;
            margin: 6px 10px;
            border-radius: 8px;
            font-size: 12pt;
        }
        QListWidget::item:hover {
            background-color: #616161;
        }
        QListWidget::item:selected {
            background-color: #4F4F4F;
            color: white;
            border: 2px solid #34A853;
        }
    )";

    ui->listWidget->setStyleSheet(styleSheet);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        if (item) {
            int portId = item->data(Qt::UserRole).toInt();
            QString portName = item->text(); // ambil text yang ditampilkan

            emit selectedPortId(portId, portName);

            qDebug() << "Selected port ID:" << portId << "Name:" << portName;

            accept(); // Close dialog after selection
        }
    });
}

DialogBrowsePort::~DialogBrowsePort()
{
    delete ui;
}

void DialogBrowsePort::on_pushButton_clicked()
{
    qDebug() << "Browse Port button clicked";

    QString searchText = ui->search->text().trimmed();
    if (searchText.isEmpty()) {
        return;
    }

    PortSearch *portSearch = new PortSearch(this);

    connect(portSearch, &PortSearch::searchFinished, this, [this, portSearch](const QVector<PortData> &ports) {
        ui->listWidget->clear();
        for (const PortData &port : ports) {
            QListWidgetItem *item = new QListWidgetItem(QString("%1 (%2)").arg(port.port, port.country));
            item->setData(Qt::UserRole, port.id);
            ui->listWidget->addItem(item);
        }
        portSearch->deleteLater(); // supaya tidak leak
    });

    connect(portSearch, &PortSearch::searchError, this, [this, portSearch](const QString &err) {
        qWarning() << "Port search error:" << err;
        portSearch->deleteLater();
    });

    // 🚀 Trigger request
    portSearch->searchPort(searchText);
}

