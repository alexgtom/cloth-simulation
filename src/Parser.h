#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <Eigen/Dense>

#include "Shape.h"
#include "Environment.h"

class Parser {
  public:
    Environment readFile(string file);
};

Environment Parser::readFile(string file) {
  Environment env;
  vector<Shape*> shapes;
  vector<Force*> forces;

  ifstream inpfile(file.c_str());
  if(!inpfile.is_open()) {
    cerr << "Unable to open file: " << file << endl;
    exit(1);
  } 
  else {
    string line;
    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;

      std::getline(inpfile,line);
      std::stringstream ss(line);
      //cout << line << endl;
      while (ss >> buf) {
        splitline.push_back(buf);
      }

      //Ignore blank lines
      if(splitline.size() == 0) {
        continue;
      }

      //Ignore comments
      if(splitline[0][0] == '#') {
        continue;
      }

      if(!splitline[0].compare("sphere")) {
        float r = atof(splitline[1].c_str());
        float x = atof(splitline[2].c_str());
        float y = atof(splitline[3].c_str());
        float z = atof(splitline[4].c_str());
        Point *p = new Point(x, y, z);
        Shape *sph = new Sphere(r, *p);
        shapes.push_back(sph);
        //cout << "creating sphere with radius " << r << " at point (," << x << "," << y << "," << z << ")" << endl;
      } else if(!splitline[0].compare("cloth")) {
        //float r = atof(splitline[1].c_str());
        //float g = atof(splitline[2].c_str());
        //float b = atof(splitline[2].c_str());
        //string texture = splitline[1];
        //??? properties pending
        Shape *c = new Cloth();
        shapes.push_back(c);
      } else if(!splitline[0].compare("force")) {
        float px = atof(splitline[1].c_str());
        float py = atof(splitline[2].c_str());
        float pz = atof(splitline[3].c_str()); 
        float dx = atof(splitline[4].c_str());
        float dy = atof(splitline[5].c_str());
        float dz = atof(splitline[6].c_str());
        Point *p = new Point(px, py, pz);
        Eigen::Vector3f *d = new Vector3f(dx, dy, dz);
        Force *f = new Force(*p, *d);
        forces.push_back(f);
      }
    }
  }
  env.shape_list = shapes;
  env.force_list = forces;
  return env;
}


#endif
