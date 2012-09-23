#ifndef matching_h_
#define matching_h_
#include "string.h"
#include "mst.h"
#include "limits.h"

const int maxn = 1001;
const int INF = UINT_MAX;

class Matching {
 private:
  vector<point> points;
  vector<edge> edges;
  vector<bool> visited;
  void createEdge();
  void greedy();
 public:
  Matching (vector<point>p);
  vector<edge> ans_edges;
  void work();
  void work2();
  void output();
  vector<edge> givenAnsEdges();
};
#endif
