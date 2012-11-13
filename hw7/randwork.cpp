#include "randwork.h"
#include <time.h>
#include <stdlib.h>
#include <set>

using namespace std;

void RandWorker::makeDecision (vector<Location*> *locs,vector<Nano*> *nanos,
				       vector<Location>& retlocs,vector<Nano>&retnanos) {
  vector<Location> ret;
  locs_ = locs;
  nanos_ = nanos;

  retlocs.clear();
  retnanos.clear();

  srand (time(NULL));

  int loc_size = locs->size();
  int rand_id = rand() % loc_size;
  int rand_lastDir = rand() % 4;
  vector<int> rand_seq;
  vector<int> rand_series;
  for(int i=0; i<4; i++)
    rand_series.push_back(i);
  while(rand_series.size() > 0){
    int random = rand() % rand_series.size();
    rand_seq.push_back( rand_series[random] );
    rand_series.erase(rand_series.begin()+random);
  }
  
  Location* tmp;

  tmp = locs_->at(rand_id)->getUniqueChild();
  Nano nano(tmp->getID(), rand_lastDir, rand_seq, myTeam_);
  Location* l = locs_->at(tmp->getID());
  Location loc(l->getID(), l->getX(), l->getY());
  retlocs.push_back(loc);
  retnanos.push_back(nano);
}
