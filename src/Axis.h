#include <AccelStepper.h>

enum class Mode {
  stopped = 0,
  jogging = 1,
  running = 2
};

class Axis {

  private:
    AccelStepper& _stepper;
    const int8_t _deadZone = 4;
    Mode _mode = Mode::stopped;
    float _speed;

  public:
    Axis(AccelStepper& stepper, int8_t deadZone) : _stepper(stepper), _deadZone(deadZone) {}

    void update(int8_t value) {
      float move;

      // Jogging. Sets a speed determined as a fraction of the maximum speed.
      if (value < -_deadZone) {
        move = (value + _deadZone) * _stepper.maxSpeed() / 127;        
        _stepper.setSpeed(move);
        _mode = Mode::jogging;
      } else if (value > _deadZone) {
        move = (value - _deadZone) * _stepper.maxSpeed() / 127;
        _stepper.setSpeed(move);
        _mode = Mode::jogging;
      } else if (_mode == Mode::jogging) {
        _stepper.stop();
        _mode = Mode::stopped;            
      }
    }

    void setSpeed(float speed) {
      _speed = speed;
      _stepper.setAcceleration(speed);
      _stepper.setMaxSpeed(speed);
    }

    long position() {
      return _stepper.currentPosition();
    }    

    void moveTo(long position, uint8_t throttle) {
      float speed = _speed * throttle / 255;

      _stepper.setMaxSpeed(speed);
      _stepper.setAcceleration(speed);
      _stepper.moveTo(position);
      _stepper.run();
    }

    void run(int8_t throttle) {
        _stepper.setMaxSpeed(_speed);

      float speed;

      if (throttle < -_deadZone) {
        speed = (throttle + _deadZone) * _speed / 127;        
        _stepper.setSpeed(speed);
        _stepper.runSpeed();
      } else if (throttle > _deadZone) {
        speed = (throttle - _deadZone) * _speed / 127;
        _stepper.setSpeed(speed);
        _stepper.runSpeed();
      } else {
        _stepper.stop();
      }
    }
};