#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "Shape.h"
#include "Environment.h"

using namespace std;

class Parser {
  public:
    Environment readFile(string file) {
      Environment env;
      return env;
    }
};

#endif
