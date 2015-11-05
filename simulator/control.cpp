#include <iostream>
#include <sstream>

#include "control.hpp"

using namespace std;

const string MOTOR_TYPES[] = {
  "MOTOR_LEFT",
  "MOTOR_RIGHT",
  "MOTOR_PEN"
};

void init_control() {

}

Stepper::Stepper(motor_t type) {
  _speed = 0;
  _steps = 0;
  _id = (int)type;
  _steps_target = 0;
  _last_step = 0;
  _type = type;
  _real_steps = 0;
}

void Stepper::move(double speed, double steps) {
  _speed = speed;
  _steps_target = _steps + steps;

  if (_steps_target < _steps) _speed = -_speed;
}

bool Stepper::tic() {
  /*
  cout << "motor:  " << MOTOR_TYPES[_type] << endl
       << "steps:  " << _steps << endl
       << "target: " << _steps_target << endl
       << "speed:  " << _speed << endl << endl;
  //*/
  if (_steps < _steps_target + 0.5 and _steps > _steps_target - 0.5)
    return false;

  _steps += _speed;

  if (_speed < 0) {
    while (_steps <= _last_step - 1) {
      step();
      --_last_step;
    }
  } else {
    while (_steps >= _last_step + 1) {
      step();
      ++_last_step;
    }
  }

  return true;
}

void Stepper::step() {
  _real_steps += _speed > 0 ? 1 : -1;
}

string Stepper::to_s() {
  stringstream ss;
  ss << "{Stepper(" << MOTOR_TYPES[_type] << "), pos: " << _real_steps << "}";
  return ss.str();
}

Servo::Servo(int range, byte pin) {

}

void Servo::set(int target) const {

}

void Servo::up() const {

}

void Servo::down() const {

}
