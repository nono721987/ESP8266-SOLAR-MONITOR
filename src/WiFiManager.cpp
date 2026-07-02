#include "WiFiManager.h"
#include <ESP8266WiFi.h>

bool WiFiManager::apMode = false;

#include "ConfigManager.h"

void WiFiManager::begin()
{
    WiFi.mode(WIFI_STA);
    connectWiFi();
}

void WiFiManager::loop()
{
    if (!apMode && WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[WIFI] Verbindung verloren -> reconnect...");
        connectWiFi();
    }
}

void WiFiManager::connectWiFi()
{
    Serial.println("[WIFI] Verbinde mit WLAN...");

    ConfigData &cfg = ConfigManager::get();


    if (cfg.wifi_ssid.length() == 0)
    {
        Serial.println("[WIFI] First Start → AP Mode");

        startAP();
        return;
    }

    WiFi.begin(cfg.wifi_ssid.c_str(), cfg.wifi_pass.c_str());


    int timeout = 20;
    while (WiFi.status() != WL_CONNECTED && timeout-- > 0)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println();
        Serial.println("[WIFI] Verbunden!");
        Serial.print("[WIFI] IP: ");
        Serial.println(WiFi.localIP());
        apMode = false;
    }
    else
    {
        Serial.println();
        Serial.println("[WIFI] Verbindung fehlgeschlagen!");
        startAP();
    }
}

void WiFiManager::startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("SOLAR-SETUP");

    Serial.println();
    Serial.println("[WIFI] =======================");
    Serial.println("[WIFI] AP MODE AKTIV");
    Serial.print("[WIFI] IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("[WIFI] Öffne im Browser:");
    Serial.println("http://192.168.4.1/settings");
    Serial.println("[WIFI] =======================");
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIP()
{
    if (apMode)
        return WiFi.softAPIP().toString();

    return WiFi.localIP().toString();
}