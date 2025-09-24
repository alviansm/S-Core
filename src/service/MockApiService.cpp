#include "MockApiService.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

MockApiService* MockApiService::m_instance = nullptr;

MockApiService::MockApiService(QObject *parent)
    : QObject(parent),
    m_manager(new QNetworkAccessManager(this)),
    m_timer(new QTimer(this)),
    m_logId(1)
{
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &MockApiService::onReplyFinished);

    connect(m_timer, &QTimer::timeout,
            this, &MockApiService::fetchVoyageLogs);
}

MockApiService* MockApiService::instance()
{
    if (!m_instance) {
        m_instance = new MockApiService();
    }
    return m_instance;
}

void MockApiService::startPolling(int logId, int intervalMs)
{
    m_logId = logId;
    if (!m_timer->isActive()) {
        m_timer->start(intervalMs);
    }
    fetchVoyageLogs();
}

void MockApiService::fetchVoyageLogs()
{
    QString url = QString("https://score-api.heyrend.cloud/api/v1/logs-data/voyage/%1").arg(m_logId);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_manager->get(request);
}

void MockApiService::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "API error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject root = jsonDoc.object();
        if (root.contains("data")) {
            VoyageLogs logs = parseVoyageLogs(root["data"].toObject());
            emit dataUpdated(logs);
        }
    }

    reply->deleteLater();
}

VoyageLogs MockApiService::parseVoyageLogs(const QJsonObject& obj)
{
    VoyageLogs logs;
    logs.log_id = obj["log_id"].toInt();
    logs.voyage_id = obj["voyage_id"].toString();
    logs.timestamp = obj["timestamp"].toString();
    logs.latitude = obj["latitude"].toDouble();
    logs.longitude = obj["longitude"].toDouble();
    logs.ship_speed = obj["ship_speed"].toDouble();
    logs.course = obj["course"].toInt();
    logs.wind_speed = obj["wind_speed"].toDouble();
    logs.sea_state = obj["sea_state"].toDouble();
    logs.hvac_power = obj["hvac_power"].toDouble();
    logs.galley_power = obj["galley_power"].toDouble();
    logs.lighting_power = obj["lighting_power"].toDouble();
    logs.total_hotel_load = obj["total_hotel_load"].toDouble();

    // Propulsion Logs
    QJsonArray propulsionArr = obj["propulsion_logs"].toArray();
    for (auto v : propulsionArr) {
        QJsonObject p = v.toObject();
        PropulsionLog log;
        log.propulsion_log_id = p["propulsion_log_id"].toInt();
        log.log_id = p["log_id"].toInt();
        log.engine_id = p["engine_id"].toString();
        log.status = p["status"].toString();
        log.rpm = p["rpm"].toInt();
        log.engine_load = p["engine_load"].toInt();
        log.power_output = p["power_output"].toDouble();
        log.fuel_consumption_rate = p["fuel_consumption_rate"].toDouble();
        log.exhaust_gas_temp = p["exhaust_gas_temp"].toInt();
        logs.propulsion_logs.append(log);
    }

    // Electrical Logs
    QJsonArray electricalArr = obj["electrical_logs"].toArray();
    for (auto v : electricalArr) {
        QJsonObject e = v.toObject();
        ElectricalLog log;
        log.electrical_log_id = e["electrical_log_id"].toInt();
        log.log_id = e["log_id"].toInt();
        log.generator_id = e["generator_id"].toString();
        log.status = e["status"].toString();
        log.gen_load = e["gen_load"].toInt();
        log.gen_power_output = e["gen_power_output"].toDouble();
        log.gen_fuel_consumption_rate = e["gen_fuel_consumption_rate"].toDouble();
        logs.electrical_logs.append(log);
    }

    // Fuel Tank Logs
    QJsonArray fuelArr = obj["fuel_tank_logs"].toArray();
    for (auto v : fuelArr) {
        QJsonObject f = v.toObject();
        FuelTankLog log;
        log.fuel_tank_log_id = f["fuel_tank_log_id"].toInt();
        log.log_id = f["log_id"].toInt();
        log.tank_id = f["tank_id"].toString();
        log.fuel_type = f["fuel_type"].toString();
        log.fuel_level = f["fuel_level"].toDouble();
        log.fuel_volume = f["fuel_volume"].toDouble();
        logs.fuel_tank_logs.append(log);
    }

    // Ballast Tank Logs
    QJsonArray ballastArr = obj["ballast_tank_logs"].toArray();
    for (auto v : ballastArr) {
        QJsonObject b = v.toObject();
        BallastTankLog log;
        log.ballast_tank_log_id = b["ballast_tank_log_id"].toInt();
        log.log_id = b["log_id"].toInt();
        log.tank_id = b["tank_id"].toString();
        log.tank_level = b["tank_level"].toDouble();
        log.pump_status = b["pump_status"].toString();
        log.pump_power_consumption = b["pump_power_consumption"].toDouble();
        logs.ballast_tank_logs.append(log);
    }

    return logs;
}
