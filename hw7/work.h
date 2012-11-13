#ifndef work_
#define work_
#include "loc.h"
#include "nano.h"
#include <vector>
#include <iostream>
class Worker {
 private:
  vector<Location*>* locs_;
  vector<Nano*>* nanos_;
  int k_;
  int myTeam_;
  void Count(int &lived, int &freed,int team);
 public:
 Worker(int myTeam,int k) :k_(k),myTeam_(myTeam){
    std::cerr<<"Work get K:"<<k_<<" team:"<<myTeam_<<endl;
  };
  vector<Location> makeDecision(vector<Location*>*locs,vector<Nano*>*nanos);
};
#endif
