#ifndef _FORCE_H_
#define _FORCE_H_

#include "Point.h"
#include <iostream>
using namespace std;

class Force {
  public:
    Point pos;
    Vector3f dir;

    Force() {
      pos = Point(0.0f, 0.0f, 0.0f);
      dir = Vector3f(0.0f, 0.0f, 0.0f);
    }

    Force(const Force& f) {
      pos = f.pos;
      dir = f.dir;
    }

    Force(const Point& pos, const Vector3f& dir) {
      this->pos = pos;
      this->dir = dir;
    }

	Force operator+(const Force& f) {
		return Force(pos, f.dir + dir);
	}

	Force operator+=(const Force& f) {
      *this = *this + f;
      return *this;
	}

	bool operator==(const Force& f) const {
		return f.pos == pos && f.dir == dir;
	}

    friend std::ostream& operator<< (std::ostream& stream, const Force& f) {
      stream << "Force( pos: " << f.pos.x() << ", " << f.pos.y() << ", " << f.pos.z();
      stream << "  dir: " << f.dir.x() << ", " << f.dir.y() << ", " << f.dir.z() << ")";
      return stream;
    }
};

#endif
