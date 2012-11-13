#include "work.h"

using namespace std;

vector<Location> Worker::makeDecision (vector<Location*> *locs,vector<Nano*> *nanos) {
  vector<Location> ret;
  locs_ = locs;
  nanos_ = nanos;
  int myLivedNano,myFreeNano;
  int opLivedNano,opFreeNano;
  Count(myLivedNano,myFreeNano,myTeam_);
  Count(opLivedNano,opFreeNano,3-myTeam_);
  cerr<<"inworker"<<myLivedNano<<' '<<myLivedNano<<endl;
  cerr<<"inworker"<<opLivedNano<<' '<<opLivedNano<<endl;
  return ret;
}
void Worker::Count(int & lived, int & freed,int team) {
  freed = k_;
  lived = 0;
  for (int i = 0; i<nanos_->size();i++) 
    if (nanos_->at(i)->getTeam() == team){
      freed --;
      if (nanos_->at(i)->live())
	lived++;
    }
}

//Location* Worker::findNextPosition
