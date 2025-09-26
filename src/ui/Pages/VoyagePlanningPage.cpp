#include "VoyagePlanningPage.h"
#include "ui_VoyagePlanningPage.h"
#include "Components/VoyageItemWidget.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

VoyagePlanningPage::VoyagePlanningPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VoyagePlanningPage)
    , m_dialogAddVoyage(new DialogAddNewVoyage(this))
    , m_dialogAddCargo(new DialogAddCargo(this))
    , m_dialogPortRotation(new DialogAddPortRotation(this))
    , m_dialogAddFuelPlan(new DialogAddFuelPlan(this))
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);

    QTimer::singleShot(0, this, [this]() {
        QList<int> sizes;
        sizes << this->width() * 0.3 << this->width() * 0.7;
        ui->splitter->setSizes(sizes);
    });

    // === Mapbox widget ===
    QVBoxLayout *mapLayout = new QVBoxLayout(ui->widgetMap);
    m_mapboxWidget = new MapboxWidgetSimple(ui->widgetMap);
    mapLayout->addWidget(m_mapboxWidget);

    // === Graphic View dengan PNG ===
    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setStyleSheet("background-color: #212121; color:white;");

    QPixmap pixmap(":/images/GA.png");
    if (!pixmap.isNull()) {
        m_pixmapItem = new QGraphicsPixmapItem(pixmap);
        m_scene->addItem(m_pixmapItem);
        m_scene->setSceneRect(pixmap.rect());

        ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

        QTimer::singleShot(0, this, [=]() {
            ui->graphicsView->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        });
    }

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Init voyage list (with default data)
    initializeVoyageList();

    // Connect map widget signals
    connect(m_mapboxWidget, &MapboxWidgetSimple::mapReady, this, &VoyagePlanningPage::onMapReady);
    connect(m_mapboxWidget, &MapboxWidgetSimple::routeDisplayed, this, &VoyagePlanningPage::onRouteDisplayed);
    connect(m_mapboxWidget, &MapboxWidgetSimple::routeError, this, &VoyagePlanningPage::onRouteError);

    // Connect voyage list selection
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &VoyagePlanningPage::onVoyageSelectionChanged);

    // Add Voyage button
    connect(ui->pushButton, &QPushButton::clicked, this, [this] () {
        m_dialogAddVoyage->setWindowTitle("Add New Voyage");
        m_dialogAddVoyage->exec();
    });

    // Voyage added
    connect(m_dialogAddVoyage, &DialogAddNewVoyage::voyageAdded, this, [this](const QString &voyageName){
        Voyage newVoyage = { voyageName, "Unknown", "Unknown", "N/A", "N/A", -1, -1 };
        m_voyages.append(newVoyage);

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        VoyageItemWidget *w = new VoyageItemWidget(
            newVoyage.name, newVoyage.from, newVoyage.to, newVoyage.etd, newVoyage.eta
            );
        item->setData(Qt::UserRole, m_voyages.size() - 1);
        item->setSizeHint(w->sizeHint());
        ui->listWidget->setItemWidget(item, w);

        QMessageBox::information(this, "Voyage Added", "New voyage added: " + voyageName);
    });

    // Connect onPortRotationAdded
    connect(m_dialogPortRotation, &DialogAddPortRotation::onPortRotationAdded,
            this, [this](const QString &fromPort, int fromPortId,
                   const QString &toPort, int toPortId,
                   const QString &eta, const QString &etd) {
                int currentRow = ui->listWidget->currentRow();
                if (currentRow < 0 || currentRow >= m_voyages.size()) return;

                Voyage &voyage = m_voyages[currentRow];
                voyage.portRotations.append({fromPort, toPort, eta, etd});

                // Update voyage summary info (from/to/etd/eta)
                if (voyage.portRotations.size() > 0) {
                    const auto &first = voyage.portRotations.first();
                    voyage.from = first.from;
                    voyage.fromPortId = fromPortId;
                    voyage.etd = first.etc;
                }
                if (voyage.portRotations.size() > 0) {
                    const auto &last = voyage.portRotations.last();
                    voyage.to = last.to;
                    voyage.toPortId = toPortId;
                    voyage.eta = last.eta;
                }

                // Refresh voyage item widget in list
                QListWidgetItem *item = ui->listWidget->item(currentRow);
                if (item) {
                    VoyageItemWidget *w = new VoyageItemWidget(
                        voyage.name, voyage.from, voyage.to, voyage.etd, voyage.eta
                        );
                    item->setSizeHint(w->sizeHint());
                    ui->listWidget->setItemWidget(item, w);
                }

                // Refresh port rotation table
                refreshPortRotationTable(voyage.portRotations);

                loadRouteFromSelection();
            });


    // Connect onCargoAdded
    connect(m_dialogAddCargo, &DialogAddCargo::onCargoAdded,
            this, [this](const QString &name, const QString &loadingPort,
                         const QString &dischargingPort, const QString &quantity,
                         const QString &units) {
        int currentRow = ui->listWidget->currentRow();
        if (currentRow < 0 || currentRow >= m_voyages.size()) return;

        Voyage &voyage = m_voyages[currentRow];
        voyage.cargos.append({name, loadingPort, dischargingPort, quantity, units});

        refreshCargoTable(voyage.cargos);
    });

    // Connect onFuelPlanAdded
    connect(m_dialogAddFuelPlan, &DialogAddFuelPlan::onFuelPlanAdded,
            this, [this](const QString &type, const QString &quantity,
                         const QString &speed, const QString &units) {
        int currentRow = ui->listWidget->currentRow();
        if (currentRow < 0 || currentRow >= m_voyages.size()) return;

        Voyage &voyage = m_voyages[currentRow];
        voyage.fuelPlans.append({type, quantity, speed, units});

        refreshFuelPlanTable(voyage.fuelPlans);
    });

    // Auto select first voyage
    QTimer::singleShot(1000, this, [this]() {
        if (ui->listWidget->count() > 0) {
            ui->listWidget->setCurrentRow(0);
        }
    });
}

VoyagePlanningPage::~VoyagePlanningPage()
{
    delete ui;
}

void VoyagePlanningPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (ui->graphicsView->scene() && !ui->graphicsView->scene()->items().isEmpty()) {
        ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

// === Initialize voyages with default data ===
void VoyagePlanningPage::initializeVoyageList()
{
    m_voyages = {
        {
            "Voyage Name 1", "Surabaya", "Singapore",
            "19 Sep 2025 08:00", "21 Sep 2025 07:30", 1683, 2865,
            {
                {"Surabaya", "Singapore", "19 Sep 2025 08:00", "21 Sep 2025 07:30"},
            },
            {
                {"Crude Oil", "Surabaya", "Singapore", "5000", "MT"},
                {"Palm Oil", "Singapore", "Makassar", "3000", "MT"}
            },
            {
                {"MDO", "200", "14", "MT"},
                {"HSFO", "150", "12", "MT"}
            }
        },
        {
            "Voyage Name 2", "Jakarta", "Merauke",
            "22 Sep 2025 08:00", "25 Sep 2025 07:30", 1596, 1635,
            {
                {"Jakarta", "Merauke", "22 Sep 2025 08:00", "25 Sep 2025 07:30"}
            },
            {
                {"Coal", "Jakarta", "Merauke", "4000", "MT"}
            },
            {
                {"MDO", "180", "13", "MT"}
            }
        }
    };

    ui->listWidget->clear();

    for (int i = 0; i < m_voyages.size(); ++i) {
        const Voyage &v = m_voyages[i];
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        VoyageItemWidget *w = new VoyageItemWidget(v.name, v.from, v.to, v.etd, v.eta);
        item->setData(Qt::UserRole, i);
        item->setSizeHint(w->sizeHint());
        ui->listWidget->setItemWidget(item, w);
    }
}

// === Refresh tables ===
void VoyagePlanningPage::refreshPortRotationTable(const QVector<PortRotation>& rotations)
{
    ui->tableWidgetPortRotation->setColumnCount(4);
    ui->tableWidgetPortRotation->setHorizontalHeaderLabels({"From", "Destination", "ETA", "ETD"});
    ui->tableWidgetPortRotation->setRowCount(0);
    // Stretch all columns
    ui->tableWidgetPortRotation->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Set selection to row
    ui->tableWidgetPortRotation->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetPortRotation->setSelectionMode(QAbstractItemView::SingleSelection);
    // Disable editing
    ui->tableWidgetPortRotation->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetPortRotation->setStyleSheet("background-color: #212121; color:white;");

    for (const auto &r : rotations) {
        int rowIndex = ui->tableWidgetPortRotation->rowCount();
        ui->tableWidgetPortRotation->insertRow(rowIndex);
        ui->tableWidgetPortRotation->setItem(rowIndex, 0, new QTableWidgetItem(r.from));
        ui->tableWidgetPortRotation->setItem(rowIndex, 1, new QTableWidgetItem(r.to));
        ui->tableWidgetPortRotation->setItem(rowIndex, 2, new QTableWidgetItem(r.eta));
        ui->tableWidgetPortRotation->setItem(rowIndex, 3, new QTableWidgetItem(r.etc));
    }
}

void VoyagePlanningPage::refreshCargoTable(const QVector<Cargo>& cargos)
{
    ui->tableWidgetCargo->setColumnCount(5);
    ui->tableWidgetCargo->setHorizontalHeaderLabels({"Name", "Loading Port", "Discharging Port", "Quantity", "Units"});
    ui->tableWidgetCargo->setRowCount(0);
    // Stretch all columns
    ui->tableWidgetCargo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Set selection to row
    ui->tableWidgetCargo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetCargo->setSelectionMode(QAbstractItemView::SingleSelection);
    // Disable editing
    ui->tableWidgetCargo->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidgetCargo->setStyleSheet("background-color: #212121; color:white;");

    for (const auto &c : cargos) {
        int rowIndex = ui->tableWidgetCargo->rowCount();
        ui->tableWidgetCargo->insertRow(rowIndex);
        ui->tableWidgetCargo->setItem(rowIndex, 0, new QTableWidgetItem(c.name));
        ui->tableWidgetCargo->setItem(rowIndex, 1, new QTableWidgetItem(c.loadingPort));
        ui->tableWidgetCargo->setItem(rowIndex, 2, new QTableWidgetItem(c.dischargingPort));
        ui->tableWidgetCargo->setItem(rowIndex, 3, new QTableWidgetItem(c.quantity));
        ui->tableWidgetCargo->setItem(rowIndex, 4, new QTableWidgetItem(c.units));
    }
}

void VoyagePlanningPage::refreshFuelPlanTable(const QVector<FuelPlan>& fuels)
{
    ui->tableWidgetFuelPlan->setColumnCount(4);
    ui->tableWidgetFuelPlan->setHorizontalHeaderLabels({"Type", "Quantity", "Speed", "Units"});
    ui->tableWidgetFuelPlan->setRowCount(0);
    // Stretch all columns
    ui->tableWidgetFuelPlan->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Set selection to row
    ui->tableWidgetFuelPlan->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetFuelPlan->setSelectionMode(QAbstractItemView::SingleSelection);
    // Disable editing
    ui->tableWidgetFuelPlan->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidgetFuelPlan->setStyleSheet("background-color: #212121; color:white;");

    for (const auto &f : fuels) {
        int rowIndex = ui->tableWidgetFuelPlan->rowCount();
        ui->tableWidgetFuelPlan->insertRow(rowIndex);
        ui->tableWidgetFuelPlan->setItem(rowIndex, 0, new QTableWidgetItem(f.type));
        ui->tableWidgetFuelPlan->setItem(rowIndex, 1, new QTableWidgetItem(f.quantity));
        ui->tableWidgetFuelPlan->setItem(rowIndex, 2, new QTableWidgetItem(f.speed));
        ui->tableWidgetFuelPlan->setItem(rowIndex, 3, new QTableWidgetItem(f.units));
    }
}

// === Handle selection ===
void VoyagePlanningPage::onVoyageSelectionChanged(int currentRow)
{
    if (currentRow < 0 || currentRow >= m_voyages.size()) {
        clearRoute();
        return;
    }

    const Voyage &selectedVoyage = m_voyages[currentRow];

    m_originPortId = selectedVoyage.fromPortId;
    m_destPortId = selectedVoyage.toPortId;

    refreshPortRotationTable(selectedVoyage.portRotations);
    refreshCargoTable(selectedVoyage.cargos);
    refreshFuelPlanTable(selectedVoyage.fuelPlans);

    loadRouteFromPorts(m_originPortId, m_destPortId);
}

// === Map events ===
void VoyagePlanningPage::onMapReady() { qDebug() << "Map ready"; }
void VoyagePlanningPage::onRouteDisplayed() { qDebug() << "Route displayed"; }
void VoyagePlanningPage::onRouteError(const QString &error) { QMessageBox::warning(this,"Route Error",error); }

// === Route methods ===
void VoyagePlanningPage::loadRouteFromPorts(int originPortId, int destPortId)
{
    if (originPortId <= 0 || destPortId <= 0) return;
    m_mapboxWidget->setSeaRouteByPorts(originPortId, destPortId);
}

void VoyagePlanningPage::loadRouteFromSelection()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow >= 0 && currentRow < m_voyages.size()) {
        const Voyage &voyage = m_voyages[currentRow];
        loadRouteFromPorts(voyage.fromPortId, voyage.toPortId);
    }
}

void VoyagePlanningPage::setOriginPort(int portId)
{
    m_originPortId = portId;
    if (m_originPortId > 0 && m_destPortId > 0)
        loadRouteFromPorts(m_originPortId, m_destPortId);
}

void VoyagePlanningPage::setDestinationPort(int portId)
{
    m_destPortId = portId;
    if (m_originPortId > 0 && m_destPortId > 0)
        loadRouteFromPorts(m_originPortId, m_destPortId);
}

void VoyagePlanningPage::clearRoute()
{
    m_mapboxWidget->setSeaRouteByPorts(-1, -1);
    m_originPortId = -1;
    m_destPortId = -1;
}

// === Accessors ===
const Voyage* VoyagePlanningPage::getCurrentlySelectedVoyage() const
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow >= 0 && currentRow < m_voyages.size()) {
        return &m_voyages[currentRow];
    }
    return nullptr;
}

void VoyagePlanningPage::selectVoyage(int index)
{
    if (index >= 0 && index < m_voyages.size()) {
        ui->listWidget->setCurrentRow(index);
    }
}

// === Buttons ===
void VoyagePlanningPage::on_pushButtonPortRotation_clicked()
{
    m_dialogPortRotation->setWindowTitle("Add Port Rotation");
    m_dialogPortRotation->exec();
}
void VoyagePlanningPage::on_pushButtonCargo_clicked()
{
    m_dialogAddCargo->setWindowTitle("Add Cargo");
    m_dialogAddCargo->exec();
}
void VoyagePlanningPage::on_pushButtonFuel_clicked()
{
    m_dialogAddFuelPlan->setWindowTitle("Add Fuel Plan");
    m_dialogAddFuelPlan->exec();
}
void VoyagePlanningPage::on_pushButton_clicked()
{
    // sudah di-handle di constructor connect()
}

void VoyagePlanningPage::on_portRotationDelete_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_voyages.size()) return;

    int selectedRow = ui->tableWidgetPortRotation->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete Port Rotation", "Please select a row to delete.");
        return;
    }

    Voyage &voyage = m_voyages[currentRow];
    if (selectedRow >= 0 && selectedRow < voyage.portRotations.size()) {
        voyage.portRotations.removeAt(selectedRow);
        refreshPortRotationTable(voyage.portRotations);
    }
}

void VoyagePlanningPage::on_cargoDelete_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_voyages.size()) return;

    int selectedRow = ui->tableWidgetCargo->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete Cargo", "Please select a row to delete.");
        return;
    }

    Voyage &voyage = m_voyages[currentRow];
    if (selectedRow >= 0 && selectedRow < voyage.cargos.size()) {
        voyage.cargos.removeAt(selectedRow);
        refreshCargoTable(voyage.cargos);
    }
}

void VoyagePlanningPage::on_fuelPlanDelete_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_voyages.size()) return;

    int selectedRow = ui->tableWidgetFuelPlan->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete Fuel Plan", "Please select a row to delete.");
        return;
    }

    Voyage &voyage = m_voyages[currentRow];
    if (selectedRow >= 0 && selectedRow < voyage.fuelPlans.size()) {
        voyage.fuelPlans.removeAt(selectedRow);
        refreshFuelPlanTable(voyage.fuelPlans);
    }
}

