#ifndef __MOTOR_HPP__
#define __MOTOR_HPP__

#include <vector>
#include <string>

#include "coord.hpp"

typedef unsigned char byte;

using namespace std;

class Coord;

typedef enum {
  MOTOR_LEFT,
  MOTOR_RIGHT,
  MOTOR_PEN
} motor_t;

//const float MAX_SPEED = 4 * 1024 / 60.0;    // steps / sec
const float MAX_SPEED = 500;    // steps / sec
const unsigned int STEPS_PER_SECOND = 1000;  // steps / sec
const float STEPS_PER_MM = 10000 / 315;   // steps / mm
const byte SERVO_PIN = 1;

void init_control(Coord& coord);

class Stepper {
private:
  long        _last_step;
  long        _real_steps;
  double      _steps;
  double      _steps_target;
  double      _speed;
  int         _id;
  byte        _pins[4];
  byte        _active;
  motor_t     _type;

  void step();
        
public:        
  Stepper(motor_t type);
  
  void move(double speed, double steps);
  bool tic();
  string to_s();
};

class Servo {
private:
  byte _pin;
  int  _range;
public:
  Servo(int range = 100, byte pin = SERVO_PIN);

  void set(int target) const;
  void up() const;
  void down() const;
};

#endif
