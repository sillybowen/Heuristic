#ifndef work_
#define work_
#include "loc.h"
#include "nano.h"
#include <vector>
class Worker {
 private:
  vector<Location*> locs_;
  vector<Nano*> nanos_;
  int k;
  int myTeam;
 public:
  vector<Location> makeDecision(vector<Location*>*locs,vector<Nano*>*nanos);
};
#endif
