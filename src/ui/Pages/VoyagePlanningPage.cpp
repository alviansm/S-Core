#include "VoyagePlanningPage.h"
#include "ui_VoyagePlanningPage.h"
#include "Components/VoyageItemWidget.h"
#include "Components/DialogAddPortRotation.h"
#include "Components/DialogAddCargo.h"
#include "Components/DialogAddFuelPlan.h"
#include "Components/DialogAddNewVoyage.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

VoyagePlanningPage::VoyagePlanningPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VoyagePlanningPage)
    , m_originPortId(-1)
    , m_destPortId(-1)
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

    // Connect map widget signals
    connect(m_mapboxWidget, &MapboxWidgetSimple::mapReady, this, &VoyagePlanningPage::onMapReady);
    connect(m_mapboxWidget, &MapboxWidgetSimple::routeDisplayed, this, &VoyagePlanningPage::onRouteDisplayed);
    connect(m_mapboxWidget, &MapboxWidgetSimple::routeError, this, &VoyagePlanningPage::onRouteError);

    // Connect voyage list selection change
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &VoyagePlanningPage::onVoyageSelectionChanged);

    // Example: Load first voyage route after map is ready
    QTimer::singleShot(2000, this, [this]() {
        // Auto-select first voyage if available
        if (ui->listWidget->count() > 0) {
            ui->listWidget->setCurrentRow(0);
        }
    });
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

    // Store voyage data with port IDs
    m_voyages = {
        { "Voyage Name 1", "Surabaya", "Singapore", "19 Sep 2025 08:00", "21 Sep 2025 07:30", 1683, 2865},
        { "Voyage Name 2", "Surabaya", "Jayapura", "22 Sep 2025 08:00", "25 Sep 2025 07:30", 1683, 1599},
        { "Voyage Name 3", "Jakarta", "Merauke", "22 Sep 2025 08:00", "25 Sep 2025 07:30", 1596, 1635 },
        { "Voyage Name 4", "Surabaya", "Batam", "22 Sep 2025 08:00", "25 Sep 2025 07:30", 1683, 1663 }
    };

    for (int i = 0; i < m_voyages.size(); ++i) {
        const Voyage &v = m_voyages[i];
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        VoyageItemWidget *w = new VoyageItemWidget(
            v.name, v.from, v.to, v.etd, v.eta
            );

        // Store voyage index in item data for easy retrieval
        item->setData(Qt::UserRole, i);

        item->setSizeHint(w->sizeHint());
        ui->listWidget->setItemWidget(item, w);
    }
}

void VoyagePlanningPage::onVoyageSelectionChanged(int currentRow)
{
    qDebug() << "Voyage selection changed to row:" << currentRow;

    if (currentRow < 0 || currentRow >= m_voyages.size()) {
        qDebug() << "Invalid voyage selection, clearing route";
        clearRoute();
        return;
    }

    // Get selected voyage data
    const Voyage &selectedVoyage = m_voyages[currentRow];

    qDebug() << "Selected voyage:" << selectedVoyage.name
             << "From Port ID:" << selectedVoyage.fromPortId
             << "To Port ID:" << selectedVoyage.toPortId;

    // Update current port selections
    m_originPortId = selectedVoyage.fromPortId;
    m_destPortId = selectedVoyage.toPortId;

    // Load route for selected voyage
    loadRouteFromPorts(selectedVoyage.fromPortId, selectedVoyage.toPortId);

    // Update UI to show current selection (you can uncomment these if you have status labels)
    // ui->labelCurrentVoyage->setText(selectedVoyage.name);
    // ui->labelCurrentRoute->setText(QString("%1 → %2").arg(selectedVoyage.from, selectedVoyage.to));
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
    QList<QStringList> portData = {
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

void VoyagePlanningPage::onMapReady()
{
    qDebug() << "Map is ready for route display";
    // You can perform any initialization here after map is ready
}

void VoyagePlanningPage::onRouteDisplayed()
{
    qDebug() << "Route has been successfully displayed on the map";
    // Update UI state, show route information, etc.

    // Example: Update status label if you have one
    // ui->labelStatus->setText("Route loaded successfully");

    // Example: Enable route-related buttons
    // ui->buttonExportRoute->setEnabled(true);
    // ui->buttonOptimizeRoute->setEnabled(true);
}

void VoyagePlanningPage::onRouteError(const QString &error)
{
    qWarning() << "Route display error:" << error;

    // Show error message to user
    QMessageBox::warning(this, "Route Error",
                         QString("Failed to load route:\n%1").arg(error));

    // Example: Update status label
    // ui->labelStatus->setText("Failed to load route");

    // Example: Disable route-related buttons
    // ui->buttonExportRoute->setEnabled(false);
    // ui->buttonOptimizeRoute->setEnabled(false);
}

void VoyagePlanningPage::loadRouteExample()
{
    // Example port IDs - replace with actual values from your port selection
    const int originPortId = 1683;    // Surabaya
    const int destPortId = 2865;      // Singapore

    qDebug() << "Loading example route from port" << originPortId << "to port" << destPortId;

    // Load route using the new API-based method
    m_mapboxWidget->setSeaRouteByPorts(originPortId, destPortId);
}

void VoyagePlanningPage::loadRouteFromPorts(int originPortId, int destPortId)
{
    // Method to load route with custom port IDs
    // This can be called from other parts of your application

    if (originPortId <= 0 || destPortId <= 0) {
        qWarning() << "Invalid port IDs provided:" << originPortId << destPortId;
        return;
    }

    qDebug() << "Loading route from port" << originPortId << "to port" << destPortId;
    m_mapboxWidget->setSeaRouteByPorts(originPortId, destPortId);
}

void VoyagePlanningPage::loadRouteFromSelection()
{
    // Load route from current voyage selection
    int currentRow = ui->listWidget->currentRow();

    if (currentRow >= 0 && currentRow < m_voyages.size()) {
        const Voyage &voyage = m_voyages[currentRow];
        loadRouteFromPorts(voyage.fromPortId, voyage.toPortId);
    } else {
        QMessageBox::information(this, "Selection Required",
                                 "Please select a voyage from the list.");
    }
}

// Additional helper methods for port management
void VoyagePlanningPage::setOriginPort(int portId)
{
    m_originPortId = portId;
    // Update UI to show selected origin port
    // ui->labelOriginPort->setText(QString("Origin Port ID: %1").arg(portId));

    // Auto-load route if both ports are selected
    if (m_originPortId > 0 && m_destPortId > 0) {
        loadRouteFromPorts(m_originPortId, m_destPortId);
    }
}

void VoyagePlanningPage::setDestinationPort(int portId)
{
    m_destPortId = portId;
    // Update UI to show selected destination port
    // ui->labelDestPort->setText(QString("Destination Port ID: %1").arg(portId));

    // Auto-load route if both ports are selected
    if (m_originPortId > 0 && m_destPortId > 0) {
        loadRouteFromPorts(m_originPortId, m_destPortId);
    }
}

void VoyagePlanningPage::clearRoute()
{
    // Clear the current route from map
    m_mapboxWidget->setSeaRouteByPorts(-1, -1); // Invalid IDs will clear the route

    // Reset port selections
    m_originPortId = -1;
    m_destPortId = -1;

    // Update UI
    // ui->labelOriginPort->setText("No origin port selected");
    // ui->labelDestPort->setText("No destination port selected");
    // ui->labelStatus->setText("Route cleared");
}

// Get currently selected voyage
const Voyage* VoyagePlanningPage::getCurrentlySelectedVoyage() const
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow >= 0 && currentRow < m_voyages.size()) {
        return &m_voyages[currentRow];
    }
    return nullptr;
}

// Select voyage by index
void VoyagePlanningPage::selectVoyage(int index)
{
    if (index >= 0 && index < m_voyages.size()) {
        ui->listWidget->setCurrentRow(index);
        // onVoyageSelectionChanged will be called automatically
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
