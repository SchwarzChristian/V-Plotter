#ifndef __LIBVPLOTTER_HPP__
#define __LIBVPLOTTER_HPP__

#include "control.hpp"

extern "C" {
  void vp_init(int left_motor_x,        int left_motor_y,
	       int right_motor_x,       int right_motor_y,
	       int calibration_point_x, int calibration_point_y,
	       int width,               int height);
  void vp_calibrate();
  void vp_wait(float seconds);
  void vp_pen_up();
  void vp_pen_down();
  void vp_set_pen(int position);
  void vp_move_left_motor(int distance);
  void vp_move_right_motor(int distance);
  void vp_goto(int x, int y);
  void vp_move(int x, int y);
  void vp_home();
  void vp_close();
}

#endif
