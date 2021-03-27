#include <Arduino.h>
#include <PS4Controller.h>
#include "Axis.h"

AccelStepper stepperX(AccelStepper::DRIVER, 26, 16);
AccelStepper stepperY(AccelStepper::DRIVER, 25, 27);
AccelStepper stepperZ(AccelStepper::DRIVER, 17, 14);

struct Speed {
  float x;
  float y;
  float z;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

Speed speeds[] = {
  {.x = 3200, .y = 800, .z = 800, .r = 255, .g = 0, .b = 0},
  {.x = 1600, .y = 200, .z = 200, .r = 0, .g = 255, .b = 0},
  {.x = 200, .y = 50, .z = 50, .r = 0, .g = 0, .b = 255}
};

void setup() {
  stepperX.setMaxSpeed(3200);
  stepperX.setAcceleration(3200);
  stepperY.setMaxSpeed(400);
  stepperY.setAcceleration(400);
  stepperZ.setMaxSpeed(400);
  stepperZ.setAcceleration(400);

  // Configure to receive PS4 commands
  PS4.begin((char*) "DC:A6:32:EA:3B:2D"); 
}

Axis x_axis(stepperX, 4);
Axis y_axis(stepperY, 8);
Axis z_axis(stepperZ, 16);
bool touchPadDown;
int speedIndex = 0;

void loop() {
  if (PS4.isConnected()) {
    if (PS4.Touchpad()) {
      if (!touchPadDown) {
        touchPadDown = true;
        speedIndex = (speedIndex + 1) % 3;
        const Speed& speed = speeds[speedIndex];
        stepperX.setMaxSpeed(speed.x);
        stepperX.setAcceleration(speed.x);
        stepperY.setMaxSpeed(speed.y);
        stepperY.setAcceleration(speed.y);
        stepperZ.setMaxSpeed(speed.z);
        stepperZ.setAcceleration(speed.z);
        PS4.setLed(speed.r, speed.g, speed.b);
        PS4.sendToController();
      }
    }
    else {
      touchPadDown = false;
    }

    x_axis.update(PS4.LStickX());
    y_axis.update(PS4.RStickX());
    z_axis.update(PS4.RStickY());
  }

  x_axis.run();
  y_axis.run();
  z_axis.run();
}