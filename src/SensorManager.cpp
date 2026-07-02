#include "SensorManager.h"
#include "ConfigManager.h"

#define ONE_WIRE_BUS D4

OneWire SensorManager::oneWire(ONE_WIRE_BUS);
DallasTemperature SensorManager::sensors(&oneWire);

float SensorManager::vorlauf = 0;
float SensorManager::ruecklauf = 0;
float SensorManager::caseTemp = 0;

void SensorManager::begin()
{
    sensors.begin();

    Serial.println("[SENSORS] DS18B20 gestartet");

    Serial.print("[SENSORS] Gefundene Sensoren: ");
    Serial.println(sensors.getDeviceCount());
}

void SensorManager::update()
{
    sensors.requestTemperatures();

    ConfigData &cfg = ConfigManager::get();

    vorlauf = getByID(cfg.sensors.vorlaufID);
    ruecklauf = getByID(cfg.sensors.ruecklaufID);
    caseTemp = getByID(cfg.sensors.caseID);
}

float SensorManager::getByID(const String &id)
{
    if (id.length() == 0) return -127;

    DeviceAddress addr;
    int count = sensors.getDeviceCount();

    for (int i = 0; i < count; i++)
    {
        if (sensors.getAddress(addr, i))
        {
            String currentID = "";

            for (uint8_t j = 0; j < 8; j++)
            {
                if (addr[j] < 16) currentID += "0";
                currentID += String(addr[j], HEX);
            }

            currentID.toUpperCase();

            String target = id;
            target.toUpperCase();

            if (currentID == target)
            {
                return sensors.getTempC(addr);
            }
        }
    }

    return -127; // Sensor nicht gefunden
}

String SensorManager::getSensorListJson()
{
    DeviceAddress addr;
    int count = sensors.getDeviceCount();

    if (count == 0)
        return "[{\"index\":-1,\"id\":\"NO_SENSORS\"}]";

    String json = "[";

    for (int i = 0; i < count; i++)
    {
        if (sensors.getAddress(addr, i))
        {
            String id = "";

            for (uint8_t j = 0; j < 8; j++)
            {
                if (addr[j] < 16) id += "0";
                id += String(addr[j], HEX);
            }

            id.toUpperCase();

            json += "{";
            json += "\"index\":" + String(i) + ",";
            json += "\"id\":\"" + id + "\"}";
        }

        if (i < count - 1) json += ",";
    }

    json += "]";
    return json;
}

float SensorManager::getVorlauf()  { return vorlauf; }
float SensorManager::getRuecklauf(){ return ruecklauf; }
float SensorManager::getCase()     { return caseTemp; }