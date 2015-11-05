#include <iostream>

#include <cmath>

#include "coord.hpp"

using namespace std;

Coord::Coord(int width, int height, Stepper* motor_l, Stepper* motor_r) : 
  _motor_l(0, 0),
  _motor_r(0, 0),
  _cali(   0, 0),
  _pos(    0, 0) {
		
  _width = width;
  _height = height;
  _p_motor_l = motor_l;
  _p_motor_r = motor_r;
}

double Coord::calc_length(Point& p1, Point& p2) {
  int dx, dy;
  dx = p1.x - p2.x;
  dy = p1.y - p2.y;
  return sqrt(dx * dx + dy * dy);
}

void   Coord::motor_l(Point p) {
  _motor_l = p;
}

Point& Coord::motor_l() {
  return _motor_l;
}


void   Coord::motor_r(Point p) {
  _motor_r = p;

}

Point& Coord::motor_r() {
  return _motor_r;

}


void   Coord::cali(Point p) {
  // cout << "p1: (" << p.x << "; " << p.y << ")" << endl;
  _cali = p;
  // cout << "p2: (" << p.x << "; " << p.y << ")" << endl;
  _pos  = p;
}

Point& Coord::cali() {
  return _cali;

}


void   Coord::pos(Point p) {
  _pos = p;

}

Point& Coord::pos() {
  return _pos;

}

void Coord::move(Point target) {
  go(_pos + target);
}

void Coord::home() {
  go(_cali);
}

void Coord::go(Point target) {
  double dl, dr;
  double ll, lr;
  double tl, tr;
  char dir_l = 0;
  char dir_r = 0;
	
  ll = calc_length(_pos, _motor_l);
  lr = calc_length(_pos, _motor_r);
	
  tl = calc_length(target, _motor_l);
  tr = calc_length(target, _motor_r);
	
  dl = tl - ll;
  dr = tr - lr;

  if (tl != ll)
    dir_l = tl > ll ? 1 : -1;
	
  if (tr != lr)
    dir_r = tr > lr ? 1 : -1;
	
  /*	
  cout << "ll: " << ll << endl << "lr: " << lr << endl
       << "tl: " << tl << endl << "tr: " << tr << endl
       << "dl: " << dl << endl << "dr: " << dr << endl;
  //*/	 

  if (dl < 0) dl = -dl;
  if (dr < 0) dr = -dr;

  if (dl > dr) {
    _p_motor_l->move(1,       dir_l * dl);
    _p_motor_r->move(dr / dl, dir_r * dr);
  } else {
    _p_motor_l->move(dl / dr, dir_l * dl);
    _p_motor_r->move(1,       dir_r * dr);
  }
  /*
    cout << "l_speed: " << _p_motor_l->speed << endl
    << "r_speed: " << _p_motor_r->speed << endl;
  */
  _pos = target;	 
}
