#include "PumpController.h"

#define PUMP_PIN D1

bool PumpController::running = false;

void PumpController::begin()
{
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW);

    Serial.println("[PUMP] Controller gestartet");
}

void PumpController::update(bool shouldRun)
{
    if (shouldRun && !running)
    {
        digitalWrite(PUMP_PIN, HIGH);
        running = true;
        Serial.println("[PUMP] ON");
    }
    else if (!shouldRun && running)
    {
        digitalWrite(PUMP_PIN, LOW);
        running = false;
        Serial.println("[PUMP] OFF");
    }
}

bool PumpController::isRunning()
{
    return running;
}