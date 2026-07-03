#include "WebServerManager.h"

#include <ESP8266WebServer.h>
#include <LittleFS.h>

#include "SensorManager.h"
#include "MqttManager.h"
#include "SolarLogic.h"
#include "HistoryManager.h"
#include "ConfigManager.h"
#include "PumpController.h"

void handleApiData();
void handleApiHistory();
void handleSaveSettings();

ESP8266WebServer server(80);

// ----------------------------
// INIT
// ----------------------------
void WebServerManager::begin()
{
    LittleFS.begin();

    // ---------------- DASHBOARD ----------------
    server.on("/", []() {
        File f = LittleFS.open("/index.html", "r");
        server.streamFile(f, "text/html");
        f.close();
    });

    // ---------------- SETTINGS ----------------
    server.on("/settings", []() {

        ConfigData &cfg = ConfigManager::get();

        File f = LittleFS.open("/settings.html", "r");
        String page = f.readString();
        f.close();

        page.replace("{{ssid}}", cfg.wifi_ssid);
        page.replace("{{pass}}", cfg.wifi_pass);
        page.replace("{{mqtt}}", cfg.mqtt_server);
        page.replace("{{port}}", String(cfg.mqtt_port));

        page.replace("{{deltaOn}}", String(cfg.deltaOn));
        page.replace("{{deltaOff}}", String(cfg.deltaOff));
        page.replace("{{minRunTime}}", String(cfg.minRunTime));
        page.replace("{{minOffTime}}", String(cfg.minOffTime));

        page.replace("{{pumpEnabled}}",
                     cfg.pumpEnabled ? "checked" : "");

        server.send(200, "text/html", page);
    });

    // ---------------- API ----------------
    server.on("/api/data", HTTP_GET, handleApiData);
    server.on("/api/history", HTTP_GET, handleApiHistory);

    server.on("/api/sensors", HTTP_GET, []() {
        server.send(200, "application/json",
                    SensorManager::getSensorListJson());
    });

    // ---------------- SAVE ----------------
    server.on("/save", HTTP_POST, handleSaveSettings);

    server.begin();

    Serial.println("[WEB] Clean WebServer gestartet");
}

// ----------------------------
// LOOP
// ----------------------------
void WebServerManager::loop()
{
    server.handleClient();
}

void handleApiData()
{
    float v = SensorManager::getVorlauf();
    float r = SensorManager::getRuecklauf();
    float c = SensorManager::getCase();
    float d = v - r;

    String json = "{";
    json += "\"v\":" + String(v) + ",";
    json += "\"r\":" + String(r) + ",";
    json += "\"c\":" + String(c) + ",";
    json += "\"delta\":" + String(d) + ",";
    json += "\"mqtt\":" + String(MqttManager::isConnected() ? 1 : 0) + ",";
    json += "\"pump\":" + String(PumpController::isRunning() ? 1 : 0) + ",";
    json += "\"state\":\"" + SolarLogic::getStateString() + "\"";
    json += "}";

    server.send(200, "application/json", json);
}

void handleApiHistory()
{
    String json = "{";
    json += "\"values\":[";

    for (int i = 0; i < HistoryManager::size(); i++)
    {
        HistoryEntry e = HistoryManager::get(i);

        json += "{";
        json += "\"v\":" + String(e.v) + ",";
        json += "\"r\":" + String(e.r) + ",";
        json += "\"c\":" + String(e.c);
        json += "}";

        if (i < HistoryManager::size() - 1) json += ",";
    }

    json += "]}";

    server.send(200, "application/json", json);
}

void handleSaveSettings()
{
    ConfigData &cfg = ConfigManager::get();

    cfg.wifi_ssid = server.arg("ssid");
    cfg.wifi_pass = server.arg("pass");

    cfg.mqtt_server = server.arg("mqtt");
    cfg.mqtt_port = server.arg("port").toInt();

    cfg.deltaOn = server.arg("deltaOn").toFloat();
    cfg.deltaOff = server.arg("deltaOff").toFloat();

    cfg.minRunTime = server.arg("minRunTime").toInt();
    cfg.minOffTime = server.arg("minOffTime").toInt();

    cfg.pumpEnabled = server.hasArg("pumpEnabled");

    ConfigManager::save();

    server.send(200, "text/plain", "OK");

    delay(500);
    ESP.restart();
}