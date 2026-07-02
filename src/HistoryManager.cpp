#include "HistoryManager.h"

HistoryEntry HistoryManager::buffer[HISTORY_SIZE];
int HistoryManager::writeIndex = 0;
int HistoryManager::count = 0;

void HistoryManager::begin()
{
    writeIndex = 0;
    count = 0;
    Serial.println("[HISTORY] gestartet");
}

void HistoryManager::update(float v, float r, float c)
{
    buffer[writeIndex] = {v, r, c};

    writeIndex++;
    if (writeIndex >= HISTORY_SIZE)
        writeIndex = 0;

    if (count < HISTORY_SIZE)
        count++;
}

HistoryEntry HistoryManager::get(int index)
{
    return buffer[index];
}

int HistoryManager::size()
{
    return count;
}