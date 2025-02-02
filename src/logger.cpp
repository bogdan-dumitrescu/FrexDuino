#include <Arduino.h>

#include <constants.h>

unsigned long lastLog = 0;

char data[100];

bool shouldLog()
{
    if (!DEBUG)
    {
        return false;
    }
    return millis() - lastLog > 1000;
}

void logPosition(int32_t currentPosition)
{
    sprintf(data, "Pos: %6ld / ", currentPosition);
    Serial.print(data);
}

void logForce(int32_t force)
{
    sprintf(data, "F: %5ld / ", force);
    Serial.print(data);
}

void logButtons(uint8_t *buttons)
{

    Serial.print("Btts: ");

    for (uint8_t i = 0; i < BTN_MTRX_ROW_COUNT * BTN_MTRX_CLM_COUNT; i++)
    {
        Serial.print(buttons[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void log(int32_t currentPosition, int32_t force, uint8_t *buttons)
{
    if (shouldLog())
    {
        logPosition(currentPosition);
        logForce(force);
        logButtons(buttons);
        lastLog = millis();
    }
}
