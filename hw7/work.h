#ifndef work_
#define work_
#include "loc.h"
#include "nano.h"
#include <vector>
#include <iostream>
struct KillerNano {
  int toKill;
  int id;
  int startDir;
  int lastDir;
  bool keep;
  vector<int>seq;
};
class Worker {
 private:
  vector<Location*>* locs_;
  vector<Nano*>* nanos_;
  vector<KillerNano> killers;
  int k_;
  int myTeam_;
  void Count(int &lived, int &freed,int team);
  void AllTakeAction();
  void FirstRoundKill();
  void SecondRoundKill();
  void outputKillers();
 public:
 Worker(int myTeam,int k) :k_(k),myTeam_(myTeam){
    std::cerr<<"Work get K:"<<k_<<" team:"<<myTeam_<<endl;
  };
  void makeDecision(vector<Location*>*locs,vector<Nano*>*nanos,
		    vector<Location> &retloc,vector<Nano>&retnanos);
};
#endif
