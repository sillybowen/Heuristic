#ifndef NN_h_
#define NN_h_
#include <vector>
#include <iostream>
#include <algorithm>
#include "basic_geo.h"

using namespace std;

class NN{
 private:
  vector<point> points;
  vector<edge> edges;
  vector<int>ans_sequence;
  void createEdges();
  void findPath();
 public:
  NN(vector<point> p);
  void work();
  vector<int> give_result();
};
#endif
