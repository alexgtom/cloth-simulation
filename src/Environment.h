#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "Shape.h"
#include "Force.h"

class Environment {
  public:
    vector<Shape*> shape_list;
    vector<Force*> force_list;

    Force& externalForce(Shape* shape) {
      Force external;
      
      for(int i = 0; i < force_list.size(); i++) {
        if (shape->intersectP(*force_list[i])) {
          // set external force position to the first force's position
          if (i == 0)
            external.pos = force_list[i]->pos;
          external += *(force_list[i]);
        }
      }

      return external;
    }
};

#endif
