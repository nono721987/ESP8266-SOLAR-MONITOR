#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class SensorManager {
public:
    static void begin();
    static void update();

    static float getVorlauf();
    static float getRuecklauf();
    static float getCase();

    static float getByID(const String &id);

    static String getSensorListJson();

private:
    static float vorlauf;
    static float ruecklauf;
    static float caseTemp;

    static OneWire oneWire;
    static DallasTemperature sensors;
};