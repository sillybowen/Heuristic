#ifndef randwork_
#define randwork_
#include "loc.h"
#include "nano.h"
#include <vector>
#include <iostream>

class RandWorker {
 private:
  vector<Location*>* locs_;
  vector<Nano*>* nanos_;
  int k_;
  int myTeam_;
 public:
 RandWorker(int myTeam,int k) :k_(k),myTeam_(myTeam){
    std::cerr<<"Work get K:"<<k_<<" team:"<<myTeam_<<endl;
  };
  void makeDecision(vector<Location*>*locs,vector<Nano*>*nanos,
		    vector<Location> &retloc,vector<Nano>&retnanos);
};
#endif
