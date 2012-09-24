#ifndef evaluate_h_
#define evaluate_h_
#include <iostream>
#include "basic_geo.h"
#include <vector>
using namespace std;


class Evaluate {
 private:
  vector<point> points;  
 public:
  Evaluate(vector<point> p);
  double evaluate(vector<int> seq);
};
#endif
