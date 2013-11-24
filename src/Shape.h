#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <Eigen/Core>

using namespace std;
using namespace Eigen;

class Shape {
  public:
    Vector3f world_pos;
    virtual void render_wireframe(int t) = 0;
    virtual void render_filled(int t) = 0;
};

class Sphere : public Shape {
  public:
    Sphere() {
    }

    Sphere(Vector3f world_pos) {
      this->world_pos = world_pos;
    }

    void render_wireframe(int t) {
    }

    void render_filled(int t) {
    }
};

class Cloth : public Shape {
  public:
    Cloth() {
    }

    Cloth(Vector3f world_pos) {
      this->world_pos = world_pos;
    }

    void render_wireframe(int t) {
    }

    void render_filled(int t) {
    }
};


#endif
