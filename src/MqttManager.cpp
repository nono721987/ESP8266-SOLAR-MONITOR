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
    float d = SensorManager::getVorlauf() - SensorManager::getRuecklauf();

    String stateJson = "{";
    stateJson += "\"v\":" + String(v) + ",";
    stateJson += "\"r\":" + String(r) + ",";
    stateJson += "\"c\":" + String(c) + ",";
    stateJson += "\"d\":" + String(d) + ",";
    stateJson += "\"state\":\"" + SolarLogic::getStateString() + "\"";
    stateJson += "}";

    client.publish("poolsolar/state", stateJson.c_str());

    Serial.println("[MQTT] state gesendet");
}

bool MqttManager::isConnected()
{
    return mqttConnected;
}