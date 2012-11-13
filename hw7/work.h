#ifndef work_
#define work_
#include "loc.h"
#include "nano.h"
#include <vector>
#include <iostream>
class Worker {
 private:
  vector<Location*> locs_;
  vector<Nano*> nanos_;
  int k_;
  int myTeam_;
 public:
 Worker(int myTeam,int k) :k_(k),myTeam_(myTeam){
    std::cout<<"Work get K:"<<k_<<" team:"<<myTeam_<<endl;
  };
  vector<Location> makeDecision(vector<Location*>*locs,vector<Nano*>*nanos);
};
#endif
