#include "HistoryPage.h"
#include "ui_HistoryPage.h"

HistoryPage::HistoryPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryPage)
{
    ui->setupUi(this);


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

    // 2. Terapkan stylesheet ke listWidget
    ui->listWidget->setStyleSheet(styleSheet);

    // 3. Atur ukuran ikon agar terlihat lebih besar dan pas
    ui->listWidget->setIconSize(QSize(48, 48));

    // 4. Tambahkan item ke dalam list
    // Ganti path ikon sesuai dengan path di file resource (.qrc) Anda
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/home.png"), "DCS"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/folders.png"), "Historical"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/icons/ribbon/tags.png"), "Report"));

    // (Opsional) Pilih item pertama secara default
    if (ui->listWidget->count() > 0) {
        ui->listWidget->setCurrentRow(0);
    }
}

HistoryPage::~HistoryPage()
{
    delete ui;
}
