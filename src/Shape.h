#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include <Eigen/Core>

#include "Point.h"
#include "Force.h"

using namespace std;
using namespace Eigen;

class Shape {
  public:
    Vector3f world_pos;
    virtual void render_wireframe(int t, Force& external) = 0;
    virtual void render_filled(int t, Force& external) = 0;

    // Test if f intersects with the shape or not (in object space), if so,
    // return intersection point
    virtual bool intersect(Force& f, Point* intersection) = 0;

    // Test if f interspects with the shape or not
    virtual bool intersectP(Force& f) = 0;

};

class Sphere : public Shape {
  public:
    float radius;
    Point center;

    Sphere() {
    }

    Sphere(float radius) {
      this->radius = radius;
    }

    Sphere(float radius, Point center) {
      this->radius = radius;
      this->center = center;
    }

    void render_wireframe(int t, Force& external) {
      glPushMatrix();
      glTranslatef(center[0] , center[1], center[2]);
		  GLfloat color[] = {0.0f, 0.1f, 0.6f, 1.0f};
		  GLfloat emission[] = {0.0f, 0.2f, 0.6f, 1.0f};
		  GLfloat specular[] = {0.0f, 0.2f, 0.7f, 1.0f};

	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	  	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	  	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	  	glMaterialf(GL_FRONT, GL_SHININESS, 0.5);
	  	glutWireSphere(radius, 50.0, 50.0);
	    glPopMatrix();
    }

    void render_filled(int t, Force& external) {
      glPushMatrix();
      glTranslatef(center[0] , center[1], center[2]);
		  GLfloat color[] = {0.0f, 0.1f, 0.6f, 1.0f};
		  GLfloat emission[] = {0.0f, 0.2f, 0.6f, 1.0f};
		  GLfloat specular[] = {0.0f, 0.2f, 0.7f, 1.0f};

	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	  	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	  	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	  	glMaterialf(GL_FRONT, GL_SHININESS, 0.5);
	  	glutSolidSphere(radius, 50.0, 50.0);
	    glPopMatrix();
    }

    bool intersect(Force& f, Point* intersection) {
      // Implemented sphere intersection equations from PBD pg. 117
      // calculate A, B, C following Shirley
      float A = f.dir.dot(f.dir);
      float B = 2.0f * (f.dir.dot(f.pos - center));
      float C = (f.pos - center).dot(f.pos - center) - radius * radius;
      float t0;
      float t1;
      float thit;

      if (!quadraticSolver(A, B, C, &t0, &t1))
        return false;

      if (t0 > INFINITY || t1 < 0.0f)
        return false;
      thit = t0;
      if (t0 < 0.0f) {
        thit = t1;
        if (thit > INFINITY)
          return false;
      }

      *intersection = f.pos + thit * f.dir;

      return true;
    }

    bool intersectP(Force& f) {
      Point intersection;
      return intersect(f, &intersection);
    }

    bool quadraticSolver(float A, float B, float C, float *t0, float *t1) {
      float sq_term = B * B - 4 * A * C;
      if (sq_term < 0)
        return false;
      
      *t0 = 0.5f * (-B - sqrt(sq_term)) / A;
      *t1 = 0.5f * (-B + sqrt(sq_term)) / A;

      return true;
    }

};

class Cloth : public Shape {
  public:
    Cloth() {
    }

    Cloth(Vector3f world_pos) {
      this->world_pos = world_pos;
    }

    void render_wireframe(int t, Force& external) {
    }

    void render_filled(int t, Force& external) {
    }


    bool intersect(Force& f, Point* intersection) {
    }

    bool intersectP(Force& f) {
    }
};


#endif
