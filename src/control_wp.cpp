#include <iostream>

#include <cstdlib>
#include <wiringPi.h>

#include "control.hpp"

using namespace std;

void init_control(Coord& coord) {}

Stepper::Stepper(motor_t type) {
  static bool called = false;
  
  if (not called) {
	wiringPiSetup();
	called = true;
  }
  
  switch (type) {
  case MOTOR_LEFT:
	  _pins[0] = 11;
	  _pins[1] = 10;
	  _pins[2] = 13;
	  _pins[3] = 12;
	  break;
  case MOTOR_RIGHT:
	  _pins[0] = 5;
	  _pins[1] = 4;
	  _pins[2] = 9;
	  _pins[3] = 8;
	  break;
  default:;
  }

  _speed = 0;
  _steps = 0;
  _id    = (int)type;
  _steps_target = 0;
  _last_step = 0;
  _type = type;
  _real_steps = 0;
  _active = 0;
	
  for (int pin : _pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }

  digitalWrite(_pins[0], 1);
}

void Stepper::move(double speed, double steps) {
  _speed = speed;
  _steps_target = _steps + steps;
  if (_steps_target < _steps) _speed = -_speed;
}

bool Stepper::tic() {
  if (_steps < _steps_target + 0.5 and _steps > _steps_target - 0.5)
    return false;

  _steps += MAX_SPEED * _speed * STEPS_PER_SECOND / 1000000.0;
  
  cout << _id << ": " << _steps_target - _steps << " steps left" << endl;

  if (_speed < 0) {
    if (_steps <= _last_step - 1) {
      step();
      --_last_step;
    }
  } else {
    if (_steps >= _last_step + 1) {
      step();
      ++_last_step;
    }
  }

  return true;
}

void Stepper::step() {
  char direction = _speed > 0 ? 1 : -1;

  byte next_pin = (_active + direction + 4) % 4;
  byte next_next_pin = (next_pin + direction + 4) % 4;
  
  cout << _id << ": " << (int)_active << endl;
  
  digitalWrite(_pins[next_pin], 1);
  digitalWrite(_pins[next_next_pin], 1);
  digitalWrite(_pins[_active], 0);

  _active = next_pin;
  _real_steps += direction;
}

Servo::Servo(int range, byte pin) {}

void Servo::set(int target) const {}

void Servo::up() const {}

void Servo::down() const {}
