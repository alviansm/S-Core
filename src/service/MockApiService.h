#ifndef MOCKAPISERVICE_H
#define MOCKAPISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonObject>
#include <QList>
#include <QString>

// ------------------- Data Models -------------------
struct PropulsionLog {
    int propulsion_log_id;
    int log_id;
    QString engine_id;
    QString status;
    int rpm;
    int engine_load;
    double power_output;
    double fuel_consumption_rate;
    int exhaust_gas_temp;
};

struct ElectricalLog {
    int electrical_log_id;
    int log_id;
    QString generator_id;
    QString status;
    int gen_load;
    double gen_power_output;
    double gen_fuel_consumption_rate;
};

struct FuelTankLog {
    int fuel_tank_log_id;
    int log_id;
    QString tank_id;
    QString fuel_type;
    double fuel_level;
    double fuel_volume;
};

struct BallastTankLog {
    int ballast_tank_log_id;
    int log_id;
    QString tank_id;
    double tank_level;
    QString pump_status;
    double pump_power_consumption;
};

struct VoyageLogs {
    int log_id;
    QString voyage_id;
    QString timestamp;
    double latitude;
    double longitude;
    double ship_speed;
    int course;
    double wind_speed;
    double sea_state;
    double hvac_power;
    double galley_power;
    double lighting_power;
    double total_hotel_load;

    QList<PropulsionLog> propulsion_logs;
    QList<ElectricalLog> electrical_logs;
    QList<FuelTankLog> fuel_tank_logs;
    QList<BallastTankLog> ballast_tank_logs;
};

// ------------------- Service -------------------
class MockApiService : public QObject
{
    Q_OBJECT
public:
    static MockApiService* instance();
    void startPolling(int logId, int intervalMs = 5000);

signals:
    void dataUpdated(const VoyageLogs& data);

private slots:
    void fetchVoyageLogs();
    void onReplyFinished(QNetworkReply* reply);

private:
    explicit MockApiService(QObject *parent = nullptr);
    static MockApiService* m_instance;

    QNetworkAccessManager* m_manager;
    QTimer* m_timer;
    int m_logId;

    VoyageLogs parseVoyageLogs(const QJsonObject& obj);
};

#endif // MOCKAPISERVICE_H
