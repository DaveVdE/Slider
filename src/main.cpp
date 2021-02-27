#include <Arduino.h>
#include <PS4Controller.h>
#include <AccelStepper.h>

AccelStepper stepperX(AccelStepper::DRIVER, 26, 16);
AccelStepper stepperY(AccelStepper::DRIVER, 25, 27);
AccelStepper stepperZ(AccelStepper::DRIVER, 17, 14);

void setup() {
  // Configuring X axis, 800 steps/rev, max speed 4 rev/s, accel at 800 for now.
  stepperX.setMaxSpeed(3200);
  stepperX.setAcceleration(800);
  stepperY.setMaxSpeed(800);
  stepperY.setAcceleration(800);
  stepperZ.setMaxSpeed(800);
  stepperZ.setAcceleration(800);

  // Configure to receive PS4 commands
  PS4.begin("DC:A6:32:EA:3B:2D");
}

class Axis {

  private:
    AccelStepper& _stepper;
    const int8_t _deadZone = 4;
    bool _isRunning = false;

    void isRunning(bool value) {
      _isRunning = value;
    }

    bool isRunning() {
      return _isRunning;
    }

  public:
    Axis(AccelStepper& stepper, int8_t deadZone) : _stepper(stepper), _deadZone(deadZone) {}

    void update(int8_t value) {
      float move;

      if (value < -_deadZone) {
        move = (value + _deadZone) * _stepper.maxSpeed() / 64;        
        _stepper.move(move);
        isRunning(true);
      } else if (value > _deadZone) {
        move = (value - _deadZone) * _stepper.maxSpeed() / 64;
        _stepper.move(move);
        isRunning(true);
      } else {
        if (isRunning()) {
        _stepper.stop();
        isRunning(false);
        }        
      }
    }
};

Axis x_axis(stepperX, 4);
Axis y_axis(stepperY, 8);
Axis z_axis(stepperZ, 16);

void loop() {
  if (PS4.isConnected()) {
    x_axis.update(PS4.LStickX());
    y_axis.update(PS4.RStickX());
    z_axis.update(PS4.RStickY());
  }

  stepperX.run();
  stepperY.run();
  stepperZ.run();
}