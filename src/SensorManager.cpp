#include "SensorManager.h"

#define ONE_WIRE_BUS D4

OneWire SensorManager::oneWire(ONE_WIRE_BUS);
DallasTemperature SensorManager::sensors(&SensorManager::oneWire);

void SensorManager::begin()
{
    sensors.begin();
    Serial.println("[SENSORS] gestartet");
}

void SensorManager::update()
{
    sensors.requestTemperatures();
}

float SensorManager::getTempByID(const String &id)
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
    String json = "[";

    int count = sensors.getDeviceCount();

    if (count == 0)
        return "[{\"id\":\"NO_SENSORS\",\"index\":-1}]";

    for (int i = 0; i < count; i++)
    {
        DeviceAddress addr;
        sensors.getAddress(addr, i);

        String id = "";

        for (int j = 0; j < 8; j++)
        {
            if (addr[j] < 16) id += "0";
            id += String(addr[j], HEX);
        }

        json += "{\"index\":" + String(i) + ",\"id\":\"" + id + "\"}";

        if (i < count - 1) json += ",";
    }

    json += "]";
    return json;
}


float SensorManager::getVorlauf()
{
    return getTempByID(ConfigManager::get().sensors.vorlaufID);
}

float SensorManager::getRuecklauf()
{
    return getTempByID(ConfigManager::get().sensors.ruecklaufID);
}

float SensorManager::getCase()
{
    return getTempByID(ConfigManager::get().sensors.caseID);
}

float SensorManager::getDeltaT()
{
    return getVorlauf() - getRuecklauf();
}