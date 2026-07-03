#include "ConfigManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

ConfigData ConfigManager::config;

void ConfigManager::begin()
{
    LittleFS.begin();
    load();
}

void ConfigManager::load()
{
    File file = LittleFS.open("/config.json", "r");

    if (!file)
    {
        Serial.println("[CONFIG] keine Datei, Default nutzen");
        return;
    }

    StaticJsonDocument<256> doc;
    deserializeJson(doc, file);

    config.wifi_ssid = doc["wifi_ssid"] | "";
    config.wifi_pass = doc["wifi_pass"] | "";

    config.mqtt_server = doc["mqtt_server"] | "192.168.1.56";
    config.mqtt_port = doc["mqtt_port"] | 1883;
    
    config.pumpEnabled = doc["pumpEnabled"] | true;

    config.deltaOn = doc["deltaOn"] | 5.0;
    config.deltaOff = doc["deltaOff"] | 3.0;

    config.minRunTime = doc["minRunTime"] | 300;
    config.minOffTime = doc["minOffTime"] | 120;

    file.close();

    Serial.println("[CONFIG] geladen");
}

void ConfigManager::save()
{
    File file = LittleFS.open("/config.json", "w");

    StaticJsonDocument<256> doc;

    doc["wifi_ssid"] = config.wifi_ssid;
    doc["wifi_pass"] = config.wifi_pass;
    doc["mqtt_server"] = config.mqtt_server;
    doc["mqtt_port"] = config.mqtt_port;
    doc["pumpEnabled"] = config.pumpEnabled;

    doc["deltaOn"] = config.deltaOn;
    doc["deltaOff"] = config.deltaOff;

    doc["minRunTime"] = config.minRunTime;
    doc["minOffTime"] = config.minOffTime;

    serializeJson(doc, file);

    file.close();

    Serial.println("[CONFIG] gespeichert");
}

ConfigData& ConfigManager::get()
{
    return config;
}