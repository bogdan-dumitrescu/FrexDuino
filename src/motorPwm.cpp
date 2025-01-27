#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#define pwmLeft 9
#define pwmRight 10
#define pwmEnable 11

uint16_t minUpdateRate = 60;
uint16_t maxUpdateRate = 1000; // set PWM to this frequency to get maximum possible motor torque

void pwm16Begin()
{
    // Stop Timer/Counter1
    TCCR1A = 0;          // Timer/Counter1 Control Register A
    TCCR1B = 0;          // Timer/Counter1 Control Register B
    TIMSK1 = 0;          // Timer/Counter1 Interrupt Mask Register
    TIFR1 = 0;           // Timer/Counter1 Interrupt Flag Register
    ICR1 = maxUpdateRate; // set upper counter flag
    OCR1A = minUpdateRate;           // Default to 0% PWM, D9
    OCR1B = minUpdateRate;           // Default to 0% PWM, D10

    // fast pwm mode
    TCCR1A |= (1 << WGM11);

    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    TCCR1B |= (1 << CS10); // Set clock prescaler to 1 for maximum PWM frequency
}

void pwm16EnableA()
{
    // Enable Fast PWM on Pin 9: Set OC1A at BOTTOM and clear OC1A on OCR1A compare
    TCCR1A |= (1 << COM1A1);
    pinModeFast(pwmLeft, OUTPUT);
}

void pwm16EnableB()
{
    // Enable Fast PWM on Pin 10: Set OC1B at BOTTOM and clear OC1B on OCR1B compare
    TCCR1A |= (1 << COM1B1);
    pinModeFast(pwmRight, OUTPUT);
}

inline void pwm16A(uint16_t pwmValue)
{
    OCR1A = pwmValue;
}

inline void pwm16B(uint16_t pwmValue)
{
    OCR1B = pwmValue;
}

void initPwmPins()
{
    pinModeFast(pwmEnable, OUTPUT);

    pwm16Begin();   // Timer1 and Timer3 configuration: frequency and mode depend on pwmstate byte
    pwm16A(0);      // Set initial PWM value for Pin 9
    pwm16EnableA(); // Turn PWM on for Pin 9
    pwm16B(0);      // Set initial PWM value for Pin 10
    pwm16EnableB(); // Turn PWM on for Pin 10
}

void setMotorPwm(int32_t torque)
{
    if (torque > 0)
    {
        torque = map(torque, 0, 10000, minUpdateRate, maxUpdateRate);
        pwm16A(minUpdateRate);
        pwm16B(torque);
        digitalWriteFast(pwmEnable, HIGH);
    }
    else if (torque < 0)
    {
        torque = map(-torque, 0, 10000, minUpdateRate, maxUpdateRate);
        pwm16A(torque);
        pwm16B(minUpdateRate);
        digitalWriteFast(pwmEnable, HIGH);
    }
    else
    {
        pwm16A(minUpdateRate);
        pwm16B(minUpdateRate);
        digitalWriteFast(pwmEnable, LOW); // disable bts output when no pwm signal to make it rotate freely
    }
}
