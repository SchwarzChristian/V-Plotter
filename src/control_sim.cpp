#include <iostream>
#include <sstream>
#include <thread>
#include <cmath>

#include <GL/glut.h>

#include "control.hpp"

using namespace std;

const string MOTOR_TYPES[] = {
  "MOTOR_LEFT",
  "MOTOR_RIGHT",
  "MOTOR_PEN"
};

thread* glut_loop;

Point* motor_l;
Point* motor_r;
Coord* coord;

double _ll;
double _lr;

int _w, _h;

vector< vector<Point*>* > traces;
vector<Point*> *curr_trace;

void timer(int x) {
  glutPostRedisplay();
  glutTimerFunc(1000 / 60.0, timer, x + 1);
}

Point* get_pos(Coord& coord) {
  motor_l = &coord.motor_l();
  motor_r = &coord.motor_r();

  double w = motor_r->x - motor_l->x;
  double x = (_ll*_ll - _lr*_lr + w*w) / 2.0 / w;
  double y = sqrt(_ll*_ll - x*x);

  // if (y < 0) {
  //   x = w/2.0 - root;
  //   y = sqrt(_ll*_ll + x*x);
  // }

  // cout << "ll:   " << _ll  << endl;
  // cout << "lr:   " << _lr  << endl;
  // cout << "w:    " << w    << endl;
  // cout << "x:    " << x    << endl;
  // cout << "y:    " << y    << endl;

  return new Point(motor_l->x + x, motor_l->y - y);
}

void init_control(Coord& _coord) {
  coord = &_coord;

  motor_r = &_coord.motor_r();
  motor_l = &_coord.motor_l();
  Point* pos = &_coord.pos();

  _w = motor_r->x + motor_l->x;
  _h = motor_l->y + motor_l->x;

  double dx = pos->x - motor_l->x;
  double dy = motor_l->y - pos->y;

  _ll = sqrt(dx*dx + dy*dy);

  dx = motor_r->x - pos->x;

  _lr = sqrt(dx*dx + dy*dy);
  
  glut_loop = new thread([]() {
      int argc = 1;
      char* argv = (char*)"";

      glutInit(&argc, &argv);
      glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

      // Create the window
      glutInitWindowSize (500, 500);
      glutInitWindowPosition (100, 100);
      glutCreateWindow("V-Plotter Simulator");

      glClearColor(0.0, 0.0, 0.0, 0.0);
      
      glEnable(GL_DEPTH_TEST);

      timer(0);
      
      glutDisplayFunc([]() {
	  glClear(GL_COLOR_BUFFER_BIT);
	  glLoadIdentity();


	  // cout << "pos:     (" << pos->x << ";" << pos->y << ")" << endl;
	  // cout << "motor_l: (" << motor_l->x << ";" << motor_l->y << ")" << endl;
	  // cout << "motor_r: (" << motor_r->x << ";" << motor_r->y << ")" << endl;

	  // draw motors
	  glBegin(GL_QUADS);
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glVertex3f( motor_l->x - 5,  motor_l->y - 5, 0);
	  glVertex3f( motor_l->x + 5,  motor_l->y - 5, 0);
	  glVertex3f( motor_l->x + 5,  motor_l->y + 5, 0);
	  glVertex3f( motor_l->x - 5,  motor_l->y + 5, 0);

	  glVertex3f( motor_r->x - 5,  motor_r->y - 5, 0);
	  glVertex3f( motor_r->x + 5,  motor_r->y - 5, 0);
	  glVertex3f( motor_r->x + 5,  motor_r->y + 5, 0);
	  glVertex3f( motor_r->x - 5,  motor_r->y + 5, 0);
	  glEnd();

	  // draw trace
	  glColor4f(0, 0, 1, 1);
	  for (auto trace : traces) {
	    glBegin(GL_LINE_STRIP);
	    for (Point* p : *trace)
	      glVertex3f(p->x, p->y, 0);
	    glEnd();
	  }

	  if (curr_trace) {
	    glBegin(GL_LINE_STRIP);
	    for (Point* p : *curr_trace)
	      glVertex3f(p->x, p->y, 0);
	    glEnd();
	  }

	  // draw printer
	  Point* pos = get_pos(*coord);
	  glBegin(GL_LINES);
	  if (curr_trace) glColor4f(0, 0, 1, 1);
	  else glColor3f(0, 1, 0);

	  glVertex3f(pos->x - 10, pos->y - 10, 0);
	  glVertex3f(pos->x + 10, pos->y + 10, 0);

	  glVertex3f(pos->x + 10, pos->y - 10, 0);
	  glVertex3f(pos->x - 10, pos->y + 10, 0);

	  // draw strings
	  glColor3f(1, 0, 0);
	  glVertex3f(motor_l->x, motor_l->y, 0);
	  glVertex3f(pos->x,     pos->y, 0);
	  
	  glVertex3f(motor_r->x, motor_r->y, 0);
	  glVertex3f(pos->x,     pos->y, 0);
	  glEnd();

	  glFlush();
	});

      glutReshapeFunc([](int w, int h) {
	  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(0.0, _w, 0.0, _h, -1.0, 1.0);
	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();
	}); 
      
      glutMainLoop();
    });
}

Stepper::Stepper(motor_t type) {
  _speed = 0;
  _steps = 0;
  _id = (int)type;
  _steps_target = 0;
  _last_step = 0;
  _type = type;
  _real_steps = _ll;
}

void Stepper::move(double speed, double steps) {
  _speed = speed;
  _steps_target = _steps + steps;

  if (_steps_target < _steps) _speed = -_speed;
}

bool Stepper::tic() {
  // cout << "motor:  " << MOTOR_TYPES[_type] << endl
  //      << "steps:  " << _steps << endl
  //      << "target: " << _steps_target << endl
  //      << "speed:  " << _speed << endl << endl;
  if (_steps < _steps_target + 0.5 and _steps > _steps_target - 0.5)
    return false;

  _steps += MAX_SPEED * _speed * STEPS_PER_SECOND / 1000000.0;

  if (_speed < 0) {
    if (_steps <= _last_step - 1) {
      step();
      --_last_step;
    }
  } else {
    if (_steps >= _last_step + 1) {
      step();
      ++_last_step;
    }
  }

  return true;
}

void Stepper::step() {
  char direction = _speed > 0 ? 1 : -1;
  switch (_type) {
  case MOTOR_LEFT:
    _ll += direction;
    break;
  case MOTOR_RIGHT:
    _lr += direction;
    break;
  default: break;
  }
  _real_steps += direction;
  if (curr_trace)
    curr_trace->push_back(get_pos(*coord));
}

string Stepper::to_s() {
  stringstream ss;
  ss << "{Stepper(" << MOTOR_TYPES[_type] << "), pos: " << _real_steps << "}";
  return ss.str();
}

Servo::Servo(int range, byte pin) {

}

void Servo::set(int target) const {
  
}

void Servo::up() const {
  traces.push_back(curr_trace);
  curr_trace = NULL;
}

void Servo::down() const {
  if (not curr_trace)
    curr_trace = new vector<Point*>();
}
