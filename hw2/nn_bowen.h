#ifndef nn_bowen_h_
#define nn_bowen_h_
#include <iostream>
#include <vector>
#include "basic_geo.h"
using namespace std;

class NN_Bowen {
 private:
  vector<point> points;
  vector<int> ans;
 public:
  NN_Bowen(vector<point> p);
  void work();
  vector<int> giveResult();

};
#endif
