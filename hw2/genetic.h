#ifndef genetic_h_
#define genetic_h_
#include "basic_geo.h"
#include <vector>
#include <list>
using namespace std;
class Genetic {
 private:
  vector<point> points;
  vector<int> ansSeq;
  double calDistance(vector<point> p, point pre_p, point post_p);
  int calFactorial(int n);
 public:
  Genetic(vector<int> pre_result, vector<point> p);
  void work(int max_length);
  void work2(int start_length, int end_length);
  vector<int> giveResult();
};
#endif
