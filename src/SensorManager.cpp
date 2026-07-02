#include "SensorManager.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float SensorManager::vorlauf = 0;
float SensorManager::ruecklauf = 0;
float SensorManager::caseTemp = 0;


static unsigned long lastUpdate = 0;
static const unsigned long interval = 2000;


void SensorManager::begin()
{
    sensors.begin();

    Serial.println("[SENSORS] DS18B20 gestartet");

    int count = sensors.getDeviceCount();
    Serial.print("[SENSORS] Gefundene Sensoren: ");
    Serial.println(count);
}

void SensorManager::update()
{
    if (millis() - lastUpdate < interval)
        return;

    lastUpdate = millis();

    sensors.requestTemperatures();

    vorlauf = sensors.getTempCByIndex(0);
    ruecklauf = sensors.getTempCByIndex(1);
    caseTemp = sensors.getTempCByIndex(2);

    Serial.print("[SENSORS] V: ");
    Serial.print(vorlauf);
    Serial.print(" R: ");
    Serial.print(ruecklauf);
    Serial.print(" C: ");
    Serial.println(caseTemp);
}

float SensorManager::getVorlauf()
{
    return vorlauf;
}

float SensorManager::getRuecklauf()
{
    return ruecklauf;
}

float SensorManager::getCase()
{
    return caseTemp;
}

float SensorManager::getDeltaT()
{
    return vorlauf - ruecklauf;
}