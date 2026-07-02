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

String SolarLogic::getStateString()
{
    return state;
}