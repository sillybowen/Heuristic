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
  vector<int> roundInfo_;
  vector<Stock*> * stocks_;
 public:
  struct Scores {
    double s[3];
    double sum;
  };
 Stock(int id,int cid,vector<Stock*>*stocks) : 
  id_(id),
    classID_(cid),
    stocks_(stocks){};
  void  assignProb(int index,double p,double r);
  void addNeighbor(int index);
  void addRoundInfo(int info);
  void output();
  double score();
  int getClass() {return classID_;}
  Scores pureScore();
  Scores neighborsScore();
};
#endif
