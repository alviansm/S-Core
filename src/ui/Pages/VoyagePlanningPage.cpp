#include "VoyagePlanningPage.h"
#include "ui_VoyagePlanningPage.h"
#include "Components/VoyageItemWidget.h"
#include "Components/DialogAddPortRotation.h"
#include "Components/DialogAddCargo.h"
#include "Components/DialogAddFuelPlan.h"
#include "Components/DialogAddNewVoyage.h"
#include <QVBoxLayout>

VoyagePlanningPage::VoyagePlanningPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VoyagePlanningPage)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 3);  // widget kiri = bobot 3
    ui->splitter->setStretchFactor(1, 7);  // widget kanan = bobot 7

    QVBoxLayout *mapLayout = new QVBoxLayout(ui->widgetMap);

    m_mapboxWidget = new MapboxWidgetSimple(ui->widgetMap);
    mapLayout->addWidget(m_mapboxWidget);

    initializeVoyageList();

    initializePortRotationList();
    initializeCargoList();
    initializFuelPlanList();
}

VoyagePlanningPage::~VoyagePlanningPage()
{
    delete ui;
}

void VoyagePlanningPage::initializeVoyageList()
{
    ui->listWidget->setStyleSheet(R"(
    QListWidget {
        background: #1e1e1e;
        border: none;
        outline: 0; /* hilangkan outline */
    }
    QListWidget::item {
        margin: 5px;
    }
    QListWidget::item:selected {
        border: 1px solid #3cb371;
        border-radius: 6px;
        outline: none; /* hilangkan outline item */
    }
    )");

    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    QVector<Voyage> voyages = {
        { "Voyage Name 1", "Surabaya", "Singapore", "19 Sep 2025 08:00", "21 Sep 2025 07:30" },
        { "Voyage Name 2", "Singapore", "Makassar", "22 Sep 2025 08:00", "25 Sep 2025 07:30" },
        { "Voyage Name 2", "Singapore", "Makassar", "22 Sep 2025 08:00", "25 Sep 2025 07:30" },
        { "Voyage Name 2", "Singapore", "Makassar", "22 Sep 2025 08:00", "25 Sep 2025 07:30" }
    };

    for (const Voyage &v : voyages) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        VoyageItemWidget *w = new VoyageItemWidget(
            v.name, v.from, v.to, v.etd, v.eta
            );
        item->setSizeHint(w->sizeHint());
        ui->listWidget->setItemWidget(item, w);
    }

}

void VoyagePlanningPage::initializePortRotationList()
{
    ui->tableWidgetPortRotation->setColumnCount(4);
    ui->tableWidgetPortRotation->setHorizontalHeaderLabels({"From", "Destination", "ETA", "ETC"});
    ui->tableWidgetPortRotation->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetPortRotation->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetPortRotation->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidgetPortRotation->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ==== Dummy Data ====
    QList<QStringList>  portData = {
        {"Surabaya", "Singapore", "19 Sep 2025 08:00", "21 Sep 2025 07:30"},
        {"Singapore", "Makassar", "22 Sep 2025 10:00", "25 Sep 2025 12:00"}
    };

    for (const auto &row : portData) {
        int rowIndex = ui->tableWidgetPortRotation->rowCount();
        ui->tableWidgetPortRotation->insertRow(rowIndex);
        for (int col = 0; col < row.size(); ++col) {
            ui->tableWidgetPortRotation->setItem(rowIndex, col, new QTableWidgetItem(row[col]));
        }
    }
}

void VoyagePlanningPage::initializeCargoList()
{
    ui->tableWidgetCargo->setColumnCount(5);
    ui->tableWidgetCargo->setHorizontalHeaderLabels({"Name", "Loading Port", "Discharging Port", "Quantity", "Units"});
    ui->tableWidgetCargo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetCargo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetCargo->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidgetCargo->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ==== Dummy Data ====
    QList<QStringList> cargoData = {
        {"Crude Oil", "Surabaya", "Singapore", "5000", "MT"},
        {"Palm Oil", "Singapore", "Makassar", "3000", "MT"}
    };

    for (const auto &row : cargoData) {
        int rowIndex = ui->tableWidgetCargo->rowCount();
        ui->tableWidgetCargo->insertRow(rowIndex);
        for (int col = 0; col < row.size(); ++col) {
            ui->tableWidgetCargo->setItem(rowIndex, col, new QTableWidgetItem(row[col]));
        }
    }
}

void VoyagePlanningPage::initializFuelPlanList()
{
    ui->tableWidgetFuelPlan->setColumnCount(4);
    ui->tableWidgetFuelPlan->setHorizontalHeaderLabels({"Type", "Quantity", "Speed", "Units"});
    ui->tableWidgetFuelPlan->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetFuelPlan->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetFuelPlan->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidgetFuelPlan->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ==== Dummy Data ====
    QList<QStringList> fuelData = {
        {"MDO", "200", "14", "MT"},
        {"HSFO", "150", "12", "MT"}
    };

    for (const auto &row : fuelData) {
        int rowIndex = ui->tableWidgetFuelPlan->rowCount();
        ui->tableWidgetFuelPlan->insertRow(rowIndex);
        for (int col = 0; col < row.size(); ++col) {
            ui->tableWidgetFuelPlan->setItem(rowIndex, col, new QTableWidgetItem(row[col]));
        }
    }
}

void VoyagePlanningPage::on_pushButtonPortRotation_clicked()
{
    // Open DdialogAddPortRotation
    DialogAddPortRotation dialog(this);

    // Set Title
    dialog.setWindowTitle("Add Port Rotation");

    dialog.exec();
}


void VoyagePlanningPage::on_pushButtonCargo_clicked()
{
    // Open DdialogAddCargo
    DialogAddCargo dialog(this);

    // Set Title
    dialog.setWindowTitle("Add Cargo Plan");

    dialog.exec();
}


void VoyagePlanningPage::on_pushButtonFuel_clicked()
{
    // Open DdialogAddFuelPlan
    DialogAddFuelPlan dialog(this);

    // Set Title
    dialog.setWindowTitle("Add Fuel Plan");

    dialog.exec();
}


void VoyagePlanningPage::on_pushButton_clicked()
{
    DialogAddNewVoyage dialog(this);
    dialog.setWindowTitle("Add New Voyage");
    dialog.exec();
}

