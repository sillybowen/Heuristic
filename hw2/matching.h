#ifndef matching_h_
#define matching_h_
#include "string.h"
#include "mst.h"
#include "limits.h"

const int maxn = 1001;
const int INF = UINT_MAX;

class Matching {
 private:
  double w[maxn][maxn];
  double lx[maxn],ly[maxn];
  int linky[maxn];
  int visx[maxn],visy[maxn];
  double slack[maxn];
  int nx,ny;
  vector<point> points;
  vector<edge> edges;
  vector<bool> visited;
  bool find(int x);
  int KM();
  void createEdge();
  void output();
  void greedy();
 public:
  Matching (vector<point>p);
  vector<edge> ans_edges;
  void work();
  vector<edge> givenAnsEdges();
};
#endif
