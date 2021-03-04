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

    void run() {
      if (_mode == Mode::jogging) {
        _stepper.runSpeed();
      } 
      else {
        _stepper.run();
      }
    }
};