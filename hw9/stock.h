#ifndef stock_h_
#define stock_h_
#include <vector>
using namespace std;

class Stock {
 private:
  double prob_[3];//0 hi 1 mi 2 low
  double ret_[3];
  int id_;
  int classID_;
  vector<int> neighbor_;
 public:
 Stock(int id,int cid) : id_(id),classID_(cid){};
  void  AssignProb(int index,double p,double r);
  void AddNeighbor(int index);
  void output();
};
#endif
