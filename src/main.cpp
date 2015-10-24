// libs: GL GLU glut
#include <iostream> 
#include <string>

#include <unistd.h>
#include <sys/time.h>

#include "control.hpp"
#include "coord.hpp"
#include "point.hpp"

using namespace std;

/*
 * l <length>  [speed] // direct control, left motor
 * r <length>  [speed] // direct control, right motor
 * m <dx> <dy> [speed] // move relative to current position
 * g  <x>  <y> [speed] // move to absolute coordinate
 * c                   // calibrate
 * u                   // move pen up
 * d                   // move pin down
 * w <time>            // wait for a given time (in ms)
 * s <level>           // set servo to this pwm level
 */

void wait(int usec) {
  struct timeval start, now;
  time_t diff;

  gettimeofday(&start, NULL);

  do {
    usleep(usec);
    gettimeofday(&now, NULL);
    diff = (now.tv_sec - start.tv_sec) * 1000000 +
      (now.tv_usec - start.tv_usec);
    // cout << "now:   " << now.tv_sec << "." << now.tv_usec << endl;
    // cout << "start: " << start.tv_sec << "." << start.tv_usec << endl;
    // cout << "diff:  " << diff << endl;
    // cout << "usec:  " << usec << endl;
  } while (diff < usec);
}

int main(int argc, char* argv[]) {
  // const byte pwm_up   = 7;
  // const byte pwm_down = 3;

  string buf = "";
  vector<string> cmd;
  int steps = 0;
  char c;

  Stepper left_motor (MOTOR_LEFT);
  Stepper right_motor(MOTOR_RIGHT);
  Coord coord(0, 0, &left_motor, &right_motor);
  Servo servo;

  coord.motor_l(Point(100, 1260));
  coord.motor_r(Point(570, 1260));
  coord.cali(   Point(305, 1160));

  init_control(coord);
	
  while (true) {
    Stepper *selected = NULL;
    cmd.clear();
    buf = "";

    cout << left_motor.to_s() << endl << right_motor.to_s() << endl;
    
    cout << "> ";
    cout.flush();
    while ((c = cin.get()) != '\n') {
      if (c == ' ') {
	cmd.push_back(buf);
	buf = "";
      } else {
	buf += c;
      }
    }
		
    cmd.push_back(buf);
		
    switch (cmd.at(0)[0]) {
    case 'c':
      coord.pos(coord.cali());
      continue;

    case 'w':
      usleep(stoi(cmd.at(1)) * 1000);
      continue;

    case 'u':
      servo.up();
      continue;

    case 's':
      servo.set(stoi(cmd.at(1)));
      continue;

    case 'd':
      servo.down();
      continue;

    case 'q':
      goto end;
      break;

    case 'l':
      selected = &left_motor;
      break;

    case 'r':
      selected = &right_motor;
      break;

    case 'g':
	coord.go(Point(stoi(cmd.at(1)),
		       stoi(cmd.at(2))));
      break;

    case 'm':
      coord.move(Point(stoi(cmd.at(1)),
		       stoi(cmd.at(2))));
      break;
    }
    
    if (selected) {
      steps = stoi(cmd.at(1)) * STEPS_PER_MM;
      if (steps < 0) selected->move(-1, steps);
      else selected->move(1, steps);
    }
    
    bool running = true;
    while (running) {
      running = right_motor.tic();
      running = left_motor.tic() or running;

      wait(1000000 / STEPS_PER_SECOND);
    }
  }
 end:
	
  return 0;
}
