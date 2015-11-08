#include <unistd.h>
#include <sys/time.h>

#include "vplotter.h"
#include "coord.hpp"

namespace {
  Stepper *left_motor;
  Stepper *right_motor;
  Servo   *servo;
  Coord   *coord;
}

void execute() {
    bool running = true;
    while (running) {
      running = right_motor->tic();
      running = left_motor->tic() or running;

      vp_wait(1.0 / STEPS_PER_SECOND);
    }
}

void vp_init(int lx, int ly,
	     int rx, int ry,
	     int cx, int cy,
	     int w,  int h) {

  left_motor  = new Stepper(MOTOR_LEFT);
  right_motor = new Stepper(MOTOR_RIGHT);
  coord       = new Coord(w, h, left_motor, right_motor);

  servo = new Servo();

  coord->motor_l(Point(lx, ly));
  coord->motor_r(Point(rx, ry));
  coord->cali(   Point(cx, cy));

  init_control(*coord);
}

void vp_calibrate() {
  coord->pos(coord->cali());
}

void vp_wait(float seconds) {
  struct timeval start, now;
  time_t to_sleep = seconds * 1000*1000;

  gettimeofday(&start, NULL);

  do {
    usleep(to_sleep);
    gettimeofday(&now, NULL);
    to_sleep -= (now.tv_sec - start.tv_sec) * 1000*1000 +
      (now.tv_usec - start.tv_usec);
  } while (to_sleep > 0);
}

void vp_pen_up() {
  servo->up();
}

void vp_pen_down() {
  servo->down();
}

void vp_set_pen(int pos) {
  servo->set(pos);
}

void vp_move_left_motor(int distance) {
  left_motor->move(1, distance);
  execute();
}

void vp_move_right_motor(int distance) {
  right_motor->move(1, distance);
  execute();
}

void vp_goto(int x, int y) {
  coord->go(Point(x, y));
  execute();
}

void vp_move(int x, int y) {
  coord->move(Point(x, y));
  execute();
}

void vp_home() {
  coord->home();
  execute();
}

void vp_close() {
  vp_home();
  if (left_motor)  delete left_motor;
  if (right_motor) delete right_motor;
  if (servo)       delete servo;
  if (coord)       delete coord;
}
