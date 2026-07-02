#include "App.h"
#include "version.h"
#include "WiFiManager.h"
#include "WebServerManager.h"
#include "SensorManager.h"
#include "MqttManager.h"

#include "HistoryManager.h"
#include "SolarLogic.h"
#include "ConfigManager.h"

unsigned long App::lastHistoryUpdate = 0;
unsigned long App::lastStateUpdate = 0;




void App::begin()
{
    Serial.begin(115200);
    delay(200);

    log("BOOTING...");

    ConfigManager::begin();   // ✅ HIER gehört es rein

    WiFiManager::begin();
    WebServerManager::begin();
    SensorManager::begin();
    MqttManager::begin();
    HistoryManager::begin();

    log("System ready");
}



void App::loop()
{
    WiFiManager::loop();
    WebServerManager::loop();
    SensorManager::update();
    MqttManager::loop();
   

    if (millis() - lastHistoryUpdate > 30000)
{
    lastHistoryUpdate = millis();

    HistoryManager::update(
        SensorManager::getVorlauf(),
        SensorManager::getRuecklauf(),
        SensorManager::getCase()
    );

    Serial.println("[HISTORY] gespeichert");
}

if (millis() - lastStateUpdate > 2000)
{
    lastStateUpdate = millis();
    SolarLogic::update();
}

}

void App::log(const String &msg)
{
    Serial.println("[" + String(millis()) + "] " + msg);
}