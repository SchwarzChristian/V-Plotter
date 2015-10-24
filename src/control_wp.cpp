#include <iostream>

#include <cstdlib>
#include <wiringPi.h>

#include "Motor.hpp"

using namespace std;

Motor::Motor(byte pin1, byte pin2, byte pin3, byte pin4) {
  static int id = 0;
  this->id = id++;
  pins.push_back(pin1);
  pins.push_back(pin2);
  pins.push_back(pin3);
  pins.push_back(pin4);
  active = 0;
  drop_steps = 1;
  direction = 0;
  speed = 1;
  steps = 0;
	
  for (int pin : pins) {
    pinMode(pin, OUTPUT);
  }
}

bool Motor::step() {
  if (direction == 0) return false;
	
  if (--drop_steps <= 0.5) {
    do_step();
    drop_steps = (double)STEPS_PER_SECOND 
      / ((double)MAX_SPEED * speed);
    cout << id << ": " << drop_steps << endl;
    cout.flush();
    return true;
  }
  return false;
}

void Motor::do_step() {
  if (direction == 0) return;

  //cout << "ok" << endl;
  byte next_pin = (active + direction + 4) % 4;
  byte next_next_pin = (next_pin + direction + 4) % 4;
	
  digitalWrite(pins.at(next_pin), 1);
  digitalWrite(pins.at(next_next_pin), 1);

  digitalWrite(pins.at(active), 0);

  active = (active + direction + 4) % 4;
  steps += direction;
}
