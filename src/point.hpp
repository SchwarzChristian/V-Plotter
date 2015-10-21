#ifndef __POINT_HPP__
#define __POINT_HPP__

class Point {
private:
public:
	double x;
	double y;
	
	Point(int x, int y);
	
	Point operator + (Point& rhs); 
};

#endif
