#include <Arduino.h>

#include <Joystick.h>
#include <DigitalWriteFast.h>

#include <constants.h>

#include <config.h>
#include <xAxisEnoder.h>
#include <motorPwm.h>
#include <buttons.h>
#include <logger.h>

Joystick_ Joystick(0x03, JOYSTICK_TYPE_JOYSTICK,
                   BTN_COUNT, 0,         // Button Count, Hat Switch Count
                   true, false, false,   // X, Y, Z Axis
                   false, false, false,  // Rx, Ry, Rz
                   false, false,         // rudder, throttle
                   false, false, false); // accelerator, brake, steering

Gains gains[1];
EffectParams effectParams[1];
int32_t forces[1] = {0};

uint8_t buttons[BTN_COUNT];

unsigned long centeringBttnLastReleased = millis();

void setWheelConfig()
{
  gains[0].totalGain = getConfig().gain;
  Joystick.setGains(gains);
}

void setup()
{
  Serial.begin(115200);

  initConfig();
  initXAxisEncoder();
  initPwmPins();
  initBttnMatrixPins();

  Joystick.setXAxisRange(ENCODER_MIN_VALUE, ENCODER_MAX_VALUE);

  setWheelConfig();

  // these need to be set, otherwise weird / unexpected behavior happens during spring effects
  Joystick.setEffectParams(effectParams);

  Joystick.begin(true);
}

ISR(TIMER3_COMPA_vect)
{
  Joystick.getUSBPID();
}

void checkForCenteringLongPress()
{
  if (buttons[CENTERING_BTN])
  {
    if ((millis() - centeringBttnLastReleased) > CTR_LNG_PRS_TRESH_MS)
    {
      resetCurrentPosition();
    }
  }
  else
  {
    centeringBttnLastReleased = millis();
  }
}

void loop()
{
  int32_t currentPosition = getCurrentPosition();

  readButtons(buttons);
  checkForCenteringLongPress();

  Joystick.setAllButtons(buttons);
  Joystick.setXAxis(currentPosition);

  Joystick.getForce(forces);

  setMotorPwm(forces[0]);

  log(currentPosition, forces[0], buttons);

  if (processSerialCfgCmds())
  {
    setWheelConfig();
  }
}
