#include "DialogAddFuelPlan.h"
#include "ui_DialogAddFuelPlan.h"
#include <QMessageBox>

DialogAddFuelPlan::DialogAddFuelPlan(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddFuelPlan)
{
    ui->setupUi(this);

    // Connect on button box accepted
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        QString type = ui->lineEditType->text();
        QString quantity = ui->lineEditQuantity->text();
        QString speed = ui->lineEditSpeed->text();
        QString units = ui->comboBoxUnits->currentText();

        if (type.isEmpty() || quantity.isEmpty() || speed.isEmpty()) {
            // Validasi sederhana
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        emit onFuelPlanAdded(type, quantity, speed, units);

        qDebug() << "Fuel Plan Added:"
                 << "Type:" << type
                 << "Quantity:" << quantity
                 << "Speed:" << speed
                 << "Units:" << units;

        accept();
    });

}

DialogAddFuelPlan::~DialogAddFuelPlan()
{
    delete ui;
}
