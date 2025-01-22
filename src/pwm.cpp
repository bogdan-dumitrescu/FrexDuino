#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#include <consts.h>

uint16_t TOP = 800;
uint16_t MM_MIN_MOTOR_TORQUE = 100;
uint16_t MM_MAX_MOTOR_TORQUE = TOP;

void PWM16Begin()
{
    // Stop Timer/Counter1
    TCCR1A = 0; // Timer/Counter1 Control Register A
    TCCR1B = 0; // Timer/Counter1 Control Register B
    TIMSK1 = 0; // Timer/Counter1 Interrupt Mask Register
    TIFR1 = 0;  // Timer/Counter1 Interrupt Flag Register
    ICR1 = TOP; // set upper counter flag
    OCR1A = 0;  // Default to 0% PWM, D9
    OCR1B = 0;  // Default to 0% PWM, D10

    // for all other pwm modes
    TCCR1B |= (1 << CS10); // Set clock prescaler to 1 for maximum PWM frequency

    // fast pwm mode
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
}

void PWM16EnableA()
{
    // Enable Fast PWM on Pin 9: Set OC1A at BOTTOM and clear OC1A on OCR1A compare
    TCCR1A |= (1 << COM1A1);
    pinModeFast(pwmLeft, OUTPUT);
}

void PWM16EnableB()
{
    // Enable Fast PWM on Pin 10: Set OC1B at BOTTOM and clear OC1B on OCR1B compare
    TCCR1A |= (1 << COM1B1);
    pinModeFast(pwmRight, OUTPUT);
}

inline void PWM16A(uint16_t PWMValue)
{
    OCR1A = constrain(PWMValue, 0, TOP);
}

inline void PWM16B(uint16_t PWMValue)
{
    OCR1B = constrain(PWMValue, 0, TOP);
}

void InitPWM()
{
    pinModeFast(pwmEnable, OUTPUT);

    PWM16Begin();   // Timer1 and Timer3 configuration: frequency and mode depend on pwmstate byte
    PWM16A(0);      // Set initial PWM value for Pin 9
    PWM16EnableA(); // Turn PWM on for Pin 9
    PWM16B(0);      // Set initial PWM value for Pin 10
    PWM16EnableB(); // Turn PWM on for Pin 10
}

void SetPWM(int32_t torque)
{ // torque between -MM_MAX_MOTOR and +MM_MAX_MOTOR
    if (torque > 0)
    {
        torque = map(torque, 0, MM_MAX_MOTOR_TORQUE, MM_MIN_MOTOR_TORQUE, MM_MAX_MOTOR_TORQUE);
        PWM16A(0);
        PWM16B(torque);
        digitalWriteFast(pwmEnable, HIGH);
    }
    else if (torque < 0)
    {
        torque = map(-torque, 0, MM_MAX_MOTOR_TORQUE, MM_MIN_MOTOR_TORQUE, MM_MAX_MOTOR_TORQUE);
        PWM16A(torque);
        PWM16B(0);
        digitalWriteFast(pwmEnable, HIGH);
    }
    else
    {
        PWM16A(0);
        PWM16B(0);
        digitalWriteFast(pwmEnable, LOW); // disable bts output when no pwm signal to make it rotate freely
    }
}
