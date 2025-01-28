#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#define encoderPinA 2
#define encoderPinB 3

volatile int32_t currentPosition = 0;
volatile int8_t oldState = 0;

const int8_t knobDirection[] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0};

void resetCurrentPosition()
{
    currentPosition = 0;
}

int32_t getCurrentPosition()
{
    return currentPosition;
}

void tick(void)
{
    int sig1 = digitalReadFast(encoderPinA);
    int sig2 = digitalReadFast(encoderPinB);
    int8_t thisState = sig1 | (sig2 << 1);

    if (oldState != thisState)
    {
        currentPosition += knobDirection[thisState | (oldState << 2)];
        oldState = thisState;
    }
}

void initXAxisEncoder()
{
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(encoderPinA), tick, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderPinB), tick, CHANGE);

    cli();
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0; // counter init
    OCR3A = 399;
    TCCR3B |= (1 << WGM32); // open CTC mode
    TCCR3B |= (1 << CS31);
    TIMSK3 |= (1 << OCIE3A);
    sei();
}