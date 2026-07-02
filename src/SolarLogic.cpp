#include "SolarLogic.h"
#include "SensorManager.h"

String SolarLogic::state = "IDLE";

void SolarLogic::update()
{
    float v = SensorManager::getVorlauf();
    float r = SensorManager::getRuecklauf();

    float delta = v - r;

    // ❌ Fehlercheck
    if (v == -127 || r == -127)
    {
        state = "ERROR";
        return;
    }

    // 🔥 Heizlogik
    if (delta > 5.0)
    {
        state = "HEATING";
    }
    else if (delta < 2.0)
    {
        state = "IDLE";
    }
    else
    {
        // Zwischenbereich = stabil halten
        state = "IDLE";
    }
}

bool SolarLogic::shouldPumpRun()
{
    float v = SensorManager::getVorlauf();
    float r = SensorManager::getRuecklauf();

    if (v == -127 || r == -127)
        return false;

    float delta = v - r;

    // 🔥 einfache aber stabile Regel
    if (delta > 5.0)
        return true;

    if (delta < 2.0)
        return false;

    return false;
}

String SolarLogic::getStateString()
{
    return state;
}