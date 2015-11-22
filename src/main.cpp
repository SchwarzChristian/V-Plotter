// libs: GL GLU glut
#include <iostream> 
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/time.h>

#include "vplotter.h"
#include "control.hpp"

using namespace std;

/*
 * l <length>  // direct control, left motor
 * r <length>  // direct control, right motor
 * m <dx> <dy> // move relative to current position
 * g <x>  <y>  // move to absolute coordinate
 * c           // calibrate printer position
 * u           // move pen up
 * d           // move pin down
 * w <time>    // wait for a given time (in ms)
 * s <level>   // set servo to this pwm level
 * h           // move to calibration point
 */
 
int main(int argc, char* argv[]) {
  // const byte pwm_up   = 7;
  // const byte pwm_down = 3;

  string buf = "";
  vector<string> cmd;
  char c;

  vp_init(-27, 440, 543, 440,
	      230, 350,   
	       30,  70, 100,
	        0,   0);
	
  while (true) {
    cmd.clear();
    buf = "";

    // cout << left_motor.to_s() << endl << right_motor.to_s() << endl;
    
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
    case 'c': vp_calibrate(); break;
    case 'w': vp_wait(stoi(cmd.at(1)) / 1000.0); break;
    case 'u': vp_pen_up(); break;
    case 's': vp_set_pen(stoi(cmd.at(1))); break;
    case 'd': vp_pen_down(); break;
    case 'q': goto end; break;
    case 'l': vp_move_left_motor(stoi(cmd.at(1))); break;
    case 'r': vp_move_right_motor(stoi(cmd.at(1))); break;
    case 'g': vp_goto(stoi(cmd.at(1)), stoi(cmd.at(2))); break;
    case 'm': vp_move(stoi(cmd.at(1)), stoi(cmd.at(2))); break;
    case 'h': vp_home(); break;
    }

  }
 end:
	
  return 0;
}
