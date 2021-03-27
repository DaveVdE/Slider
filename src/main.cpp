#include <Arduino.h>
#include <PS4Controller.h>
#include "Axis.h"

AccelStepper stepperX(AccelStepper::DRIVER, 26, 16);
AccelStepper stepperPan(AccelStepper::DRIVER, 25, 27);
AccelStepper stepperTilt(AccelStepper::DRIVER, 17, 14);

Axis axisX(stepperX, 4);
Axis axisPan(stepperPan, 8);
Axis axisTilt(stepperTilt, 16);

// Structure to store speed settings
struct Speed {
  float x;
  float pan;
  float tilt;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// Three speeds to switch between.
Speed speeds[] = {
  {.x = 200, .pan = 50, .tilt = 50, .r = 0, .g = 0, .b = 255},
  {.x = 400, .pan = 200, .tilt = 200, .r = 0, .g = 255, .b = 0},
  {.x = 800, .pan = 800, .tilt = 800, .r = 255, .g = 0, .b = 0}
};

// Structure to store set points
struct SetPoint {
  long x;
  long pan;
  long tilt;
};

// Two set points to use (L12, R1/2)
SetPoint left, right;

void setup() {
  const Speed& speed = speeds[0];
  axisX.setSpeed(speed.x);
  axisPan.setSpeed(speed.pan);
  axisTilt.setSpeed(speed.tilt);

  // Configure to receive PS4 commands
  PS4.begin((char*) "DC:A6:32:EA:3B:2D"); 
}

int speedIndex = 0;

void loop() {
  if (PS4.isConnected()) {
    if (PS4.Touchpad()) {
      speedIndex = (speedIndex + 1) % 3;
      const Speed& speed = speeds[speedIndex];
      axisX.setSpeed(speed.x);
      axisPan.setSpeed(speed.pan);
      axisTilt.setSpeed(speed.tilt);
      PS4.setLed(speed.r, speed.g, speed.b);
      PS4.sendToController();
      delay(500);
    } 
    else if (PS4.L1()) {
      // Pressing L1 means storing the current position in set point "left".
      left.x = axisX.position();
      left.pan = axisPan.position();
      left.tilt = axisTilt.position();
    }
    else if (PS4.R1()) {
      // Pressing R1 means storing the current position in set point "right".
      right.x = axisX.position();
      right.pan = axisPan.position();
      right.tilt = axisTilt.position();
    }
    else if (PS4.L2()) {
      // Pressing L2 means moving towards set point "left" at a fraction of the currently selected maximum speed and acceleration
      axisX.moveTo(left.x, PS4.L2Value());
      axisPan.moveTo(left.pan, PS4.L2Value());
      axisTilt.moveTo(left.tilt, PS4.L2Value());
    }
    else if (PS4.R2()) {
      // Pressing R2 means moving towards set point "right" at a fraction of the currently selected maximum speed and acceleration
      axisX.moveTo(right.x, PS4.R2Value());
      axisPan.moveTo(right.pan, PS4.R2Value());
      axisTilt.moveTo(right.tilt, PS4.R2Value());
    }
    else {
      // We must be jogging.
      axisX.run(PS4.LStickX());
      axisPan.run(PS4.RStickX());
      axisTilt.run(PS4.RStickY());
    }
  }

  // Don't run if not connected.
}