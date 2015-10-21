#ifndef __COORD_HPP__
#define __COORD_HPP__

#include "point.hpp"
#include "control.hpp"

class Coord {
private:
	int   _width;
	int   _height;

	Point _motor_l;
	Point _motor_r;
	Point _cali;
	Point _pos;
	
	Stepper* _p_motor_l;
	Stepper* _p_motor_r;

	double calc_length(Point& p1, Point& p2);
public:
	Coord(int width, int height, Stepper* motor_l, Stepper* motor_r);

	void   motor_l(Point p);
	Point& motor_l();
	
	void   motor_r(Point p);
	Point& motor_r();
	
	void   cali(Point p);
	Point& cali();
	
	void   pos(Point p);
	Point& pos();
	
	void   move(Point target);
	void   go(Point target);
};

#endif
