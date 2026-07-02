#include "MqttManager.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SensorManager.h"
#include "ConfigManager.h"
#include "SolarLogic.h"

WiFiClient espClient;
PubSubClient client(espClient);


unsigned long lastSend = 0;

bool mqttConnected = false;

void MqttManager::begin()
{
    ConfigData &cfg = ConfigManager::get();

client.setServer(cfg.mqtt_server.c_str(), cfg.mqtt_port);
    connect();
}

void MqttManager::loop()
{
if (!client.connected())
{
    mqttConnected = false;
    connect();
}

    client.loop();

    if (millis() - lastSend > 30000)
    {
        lastSend = millis();
        publish();
    }
}

void MqttManager::connect()
{
    ConfigData &cfg = ConfigManager::get();


if (client.connect("ESP8266-SOLAR-MONITOR"))
{
    Serial.println("[MQTT] Verbunden");
    mqttConnected = true;
}
else
{
    Serial.print("[MQTT] Fehler, rc=");
    Serial.println(client.state());
    mqttConnected = false;
    delay(2000);
}
}

void MqttManager::publish()
{
    float v = SensorManager::getVorlauf();
    float r = SensorManager::getRuecklauf();
    float c = SensorManager::getCase();
    float d = v - r;

    // ❌ Fehlercheck (wichtig für echte Systeme)
    if (v == -127 || r == -127 || c == -127)
    {
        String errorJson = "{\"state\":\"ERROR\"}";
        client.publish("poolsolar/status", errorJson.c_str());
        Serial.println("[MQTT] ERROR state gesendet");
        return;
    }

    // 🔵 TELEMETRIE (Messwerte)
    String telemetry = "{";
    telemetry += "\"v\":" + String(v) + ",";
    telemetry += "\"r\":" + String(r) + ",";
    telemetry += "\"c\":" + String(c) + ",";
    telemetry += "\"delta\":" + String(d);
    telemetry += "}";

    client.publish("poolsolar/telemetry", telemetry.c_str());

    // 🔵 SYSTEM STATUS
    String status = "{";
    status += "\"state\":\"" + SolarLogic::getStateString() + "\"";
    status += "}";

    client.publish("poolsolar/status", status.c_str());

    Serial.println("[MQTT] telemetry + status gesendet");
}

bool MqttManager::isConnected()
{
    return mqttConnected;
}