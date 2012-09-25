#ifndef trim_h_
#define trim_h_
#include "basic_geo.h"
#include <vector>
#include <list>
using namespace std;
class Trim {
 private:
  vector<point> points;
  vector<int> eulerOutput;
  vector<int> ansSeq;
  int pick(int x);
  double removeCost(int x);
  void prework();
  double dis(int i,int j);
  void revert(int a,int b);
 public:
  Trim(vector<point>p,list<int>* e);
  void work();
  void work2();
  vector<int> work3(vector<int> p);
  vector<int> giveResult();
  
};
#endif
