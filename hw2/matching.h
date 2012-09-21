#ifndef matching_h_
#define matching_h_
#include "string.h"
#include "mst.h"
const int maxn = 1001;
const int INF = (1<<31)-1;



class Matching {
 private:
  double w[maxn][maxn];
  double lx[maxn],ly[maxn];
  int linky[maxn];
  int visx[maxn],visy[maxn];
  double slack[maxn];
  int nx,ny;
  vector<point> points;
  bool find(int x);
  int KM();
  void createEdge();
  void output();
  void greedy();
 public:
  Matching (vector<point>p);
  void work();
};
#endif
