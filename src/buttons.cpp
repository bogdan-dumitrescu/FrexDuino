#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#include <buttons.h>

uint8_t rowPins[buttonMatrixRows] = {12, 13};
uint8_t columnPins[buttonMatrixColumns] = {4, 5, 6, 7};

void setMatrixRow(uint8_t i, uint8_t val)
{
    digitalWriteFast(rowPins[i], val);
}

void initBttnMatrixPins()
{

    for (uint8_t i = 0; i < buttonMatrixRows; i++)
    {
        pinModeFast(rowPins[i], OUTPUT);
        setMatrixRow(i, HIGH);
    }

    for (uint8_t j = 0; j < buttonMatrixColumns; j++)
    {
        pinMode(columnPins[j], INPUT_PULLUP);
    }
}

void readButtons(uint8_t *buttons)
{
    for (uint8_t i = 0; i < buttonMatrixRows; i++)
    {
        setMatrixRow(i, LOW);

        for (uint8_t j = 0; j < buttonMatrixColumns; j++)
        {
            buttons[i * buttonMatrixColumns + j] = !digitalReadFast(columnPins[j]);
        }

        setMatrixRow(i, HIGH);
    }
}
