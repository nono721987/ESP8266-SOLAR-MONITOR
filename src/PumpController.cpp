#include "PumpController.h"
#include "ConfigManager.h"

#define PUMP_PIN D1

bool PumpController::running = false;

unsigned long PumpController::lastStart = 0;
unsigned long PumpController::lastStop = 0;

void PumpController::begin()
{
    pinMode(PUMP_PIN, OUTPUT);

    digitalWrite(PUMP_PIN, LOW);

    running = false;

    Serial.println("[PUMP] Controller gestartet");
}

bool PumpController::isRunning()
{
    return running;
}

void PumpController::update(bool requestRun)
{
    ConfigData &cfg = ConfigManager::get();

    unsigned long now = millis();

    if (!cfg.pumpEnabled)
    {
        digitalWrite(PUMP_PIN, LOW);
        running = false;
        return;
    }

    if (requestRun)
    {
        if (!running)
        {
            if ((now - lastStop) >= cfg.minOffTime * 1000UL)
            {
                running = true;
                lastStart = now;

                digitalWrite(PUMP_PIN, HIGH);

                Serial.println("[PUMP] EIN");
            }
        }
    }
    else
    {
        if (running)
        {
            if ((now - lastStart) >= cfg.minRunTime * 1000UL)
            {
                running = false;
                lastStop = now;

                digitalWrite(PUMP_PIN, LOW);

                Serial.println("[PUMP] AUS");
            }
        }
    }
}