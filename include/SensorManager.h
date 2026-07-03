#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ConfigManager.h"

class SensorManager
{
public:
    static void begin();
    static void update();

    static float getVorlauf();
    static float getRuecklauf();
    static float getCase();
    static float getDeltaT();

    static float getTempByID(const String &id);
    static String getSensorListJson();

private:
    static OneWire oneWire;
    static DallasTemperature sensors;
};