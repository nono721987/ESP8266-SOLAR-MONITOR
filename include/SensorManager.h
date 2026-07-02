#pragma once

#include <Arduino.h>

class SensorManager {
public:
    static void begin();
    static void update();

    static float getVorlauf();
    static float getRuecklauf();
    static float getCase();
    static float getDeltaT();

private:
    static float vorlauf;
    static float ruecklauf;
    static float caseTemp;
};