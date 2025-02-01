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
                   BTN_MTRX_ROW_COUNT *BTN_MTRX_CLM_COUNT, 0, // Button Count, Hat Switch Count
                   true, false, false,                        // X, Y, Z Axis
                   false, false, false,                       // Rx, Ry, Rz
                   false, false,                              // rudder, throttle
                   false, false, false);                      // accelerator, brake, steering

Gains gains[1];
EffectParams effectParams[1];
int32_t forces[1] = {0};

uint8_t buttons[BTN_MTRX_ROW_COUNT * BTN_MTRX_CLM_COUNT];

unsigned long centeringBttnLastReleased = millis();

void setWheelConfig()
{
  auto config = getConfig();

  gains[0].totalGain = config.gainPercentage;
  Joystick.setGains(gains);

  float wheelRotations = (float)config.rotationDegrees / 360;
  float encoderTotalPulses = wheelRotations * ENCODER_PPR; // the Frex wheel turns once for every encoder revolution

  int16_t encoderMinValue = (int16_t)(-encoderTotalPulses / 2); // minimum value that the encoder will read (when wheel is turned all the way left)
  int16_t encoderMaxValue = (int16_t)(encoderTotalPulses / 2);  // maximum value that the encoder will read (when wheel is turned all the way right)

  Joystick.setXAxisRange(encoderMinValue, encoderMaxValue);

  effectParams[0].springMaxPosition = encoderMaxValue;
  Joystick.setEffectParams(effectParams);
}

void setup()
{
  Serial.begin(115200);

  initConfig();
  initXAxisEncoder();
  initPwmPins();
  initBttnMatrixPins();

  setWheelConfig();

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

  effectParams[0].springPosition = currentPosition;
  Joystick.setEffectParams(effectParams);
  Joystick.getForce(forces);

  setMotorPwm(forces[0]);

  log(currentPosition, forces[0], buttons);

  if (processSerialCfgCmds())
  {
    setWheelConfig();
  }
}
