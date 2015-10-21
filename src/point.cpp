#include "point.hpp"

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

Point Point::operator + (Point& rhs) {
	return Point(x + rhs.x, y + rhs.y);
} 
