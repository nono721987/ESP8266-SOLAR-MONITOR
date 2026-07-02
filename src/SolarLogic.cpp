#include "SolarLogic.h"
#include "SensorManager.h"

SolarState SolarLogic::state = OFF;

void SolarLogic::update()
{
    float vorlauf = SensorManager::getVorlauf();
    float ruecklauf = SensorManager::getRuecklauf();
    float delta = SensorManager::getVorlauf() - SensorManager::getRuecklauf();

    // ❌ Fehlererkennung
    if (vorlauf < -50 || ruecklauf < -50)
    {
        state = ERROR;
        return;
    }

    // ☀️ Logik
    if (delta > 6)
        state = ACTIVE;
    else if (delta > 2)
        state = IDLE;
    else
        state = OFF;
}

SolarState SolarLogic::getState()
{
    return state;
}

String SolarLogic::getStateString()
{
    switch (state)
    {
        case OFF: return "OFF";
        case IDLE: return "IDLE";
        case ACTIVE: return "ACTIVE";
        case ERROR: return "ERROR";
    }
    return "UNKNOWN";
}