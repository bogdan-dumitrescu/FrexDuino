#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#include <constants.h>

const uint8_t BTN_MTRX_ROW_PINS[BTN_MTRX_ROW_COUNT] = {12, 13};     // Arduino pins for connecting rows
const uint8_t BTN_MTRX_CLM_PINS[BTN_MTRX_CLM_COUNT] = {4, 5, 6, 7}; // Arduino pins for connecting rows

void setMatrixRow(uint8_t i, uint8_t val)
{
    digitalWriteFast(BTN_MTRX_ROW_PINS[i], val);
}

void initBttnMatrixPins()
{

    for (uint8_t i = 0; i < BTN_MTRX_ROW_COUNT; i++)
    {
        pinModeFast(BTN_MTRX_ROW_PINS[i], OUTPUT);
        setMatrixRow(i, HIGH);
    }

    for (uint8_t j = 0; j < BTN_MTRX_CLM_COUNT; j++)
    {
        pinMode(BTN_MTRX_CLM_PINS[j], INPUT_PULLUP);
    }
}

void readButtons(uint8_t *buttons)
{
    for (uint8_t i = 0; i < BTN_MTRX_ROW_COUNT; i++)
    {
        setMatrixRow(i, LOW);

        for (uint8_t j = 0; j < BTN_MTRX_CLM_COUNT; j++)
        {
            buttons[i * BTN_MTRX_CLM_COUNT + j] = !digitalReadFast(BTN_MTRX_CLM_PINS[j]);
        }

        setMatrixRow(i, HIGH);
    }
}
