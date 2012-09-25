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
  vector<double> *distance;
  int pick(int x);
  double removeCost(int x);
  void prework();
  double dis(int i,int j);
  void revert(int a,int b);
  vector<int> strongRevert(vector<int>v);
  bool updated;
  vector<int> swapHeu(vector<int> v);
 public:
  Trim(vector<point>p,list<int>* e,vector<double>* d);
  void work();
  void work2();
  vector<int> work3(vector<int> p);
  vector<int> work4(vector<int> p);
  vector<int> giveResult();
  
};
#endif
