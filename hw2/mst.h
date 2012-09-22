#ifndef mst_h_
#define mst_h_
#include <vector>
#include <cmath>
#include "basic_geo.h"

using namespace std;

class Mst {
 private:
  vector<point> points;
  vector<edge> edges;
  vector<int> root;
  vector<edge> ans_edges;
  vector<int> degree;
  void createEdges();
  int getRoot(int i);
  void merge(int x,int y);
  void findMst();
  void calculateDegree();
 public:
  Mst(vector<point> p);
  void work();
  vector<point> giveOddDegreePoints();
  vector<edge> givenAnsEdges();
};
#endif
