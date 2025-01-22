#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#include <consts.h>

#include <pwm.h>

bool isOutOfRange = false;
int32_t forces[1] = {0};
Gains gains[1];
EffectParams effectparams[1];

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
                   0, 0,                 // Button Count, Hat Switch Count
                   true, false, false,   // X, Y, Z Axis
                   false, false, false,  // Rx, Ry, Rz
                   false, false,         // rudder, throttle
                   false, false, false); // accelerator, brake, steering

unsigned long lastLog = 0;

volatile long value = 0;
int32_t g_force = 0;

int32_t currentPosition = 0;
volatile int8_t oldState = 0;
const int8_t KNOBDIR[] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0};

void tick(void)
{
  int sig1 = digitalReadFast(encoderPinA);
  int sig2 = digitalReadFast(encoderPinB);
  int8_t thisState = sig1 | (sig2 << 1);

  if (oldState != thisState)
  {
    currentPosition += KNOBDIR[thisState | (oldState << 2)];
    oldState = thisState;
  }
}

void timingMagic()
{
  cli();
  TCCR3A = 0; // set TCCR1A 0
  TCCR3B = 0; // set TCCR1B 0
  TCNT3 = 0;  // counter init
  OCR3A = 399;
  TCCR3B |= (1 << WGM32); // open CTC mode
  TCCR3B |= (1 << CS31);  // set CS11 1(8-fold Prescaler)
  TIMSK3 |= (1 << OCIE3A);
  sei();
}

void setup()
{
  Serial.begin(115200);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), tick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), tick, CHANGE);

  Joystick.setXAxisRange(ENCODER_MIN_VALUE, ENCODER_MAX_VALUE);

  gains[0].totalGain = 10;

  gains[0].constantGain = 100;
  gains[0].rampGain = 100;

  gains[0].squareGain = 100;
  gains[0].sineGain = 60;

  gains[0].triangleGain = 100;
  gains[0].sawtoothdownGain = 100;
  gains[0].sawtoothupGain = 100;

  gains[0].springGain = 0;
  gains[0].damperGain = 100;
  gains[0].inertiaGain = 0;
  gains[0].frictionGain = 100;

  gains[0].customGain = 0;

  Joystick.setGains(gains);

  Joystick.begin(true);

  timingMagic(); // is this to intercept the encoder values?

  InitPWM();
}

ISR(TIMER3_COMPA_vect)
{
  Joystick.getUSBPID();
}

bool shouldLog() {
  return millis() - lastLog > 1000;
}

void loop()
{
  value = currentPosition;

  if (value > ENCODER_MAX_VALUE)
  {
    isOutOfRange = true;
    value = ENCODER_MAX_VALUE;
  }
  else if (value < ENCODER_MIN_VALUE)
  {
    isOutOfRange = true;
    value = ENCODER_MIN_VALUE;
  }
  else
  {
    isOutOfRange = false;
  }

  Joystick.setXAxis(value);

  effectparams[0].springMaxPosition = ENCODER_MAX_VALUE;
  effectparams[0].springPosition = value;

  Joystick.setEffectParams(effectparams);
  Joystick.getForce(forces);

  SetPWM(forces[0]);

  // if (shouldLog()) {
  //     Serial.print("Force: ");
  //     Serial.println(forces[0]);

  //     // Serial.print("Encoder value: ");
  //     // Serial.println(currentPosition);

  //     lastLog = millis();
  // }
}
