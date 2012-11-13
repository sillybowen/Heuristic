#include "work.h"

using namespace std;

void Worker::makeDecision (vector<Location*> *locs,vector<Nano*> *nanos,
				       vector<Location>& retlocs,vector<Nano>&retnanos) {
  vector<Location> ret;
  locs_ = locs;
  nanos_ = nanos;

  killers.clear();
  int myLivedNano,myFreeNano;
  int opLivedNano,opFreeNano;
  Count(myLivedNano,myFreeNano,myTeam_);
  Count(opLivedNano,opFreeNano,3-myTeam_);
  cerr<<"inworker"<<myLivedNano<<' '<<myFreeNano<<endl;
  cerr<<"inworker"<<opLivedNano<<' '<<opFreeNano<<endl;
  if (myFreeNano==0) return;

  AllTakeAction();
  FirstRoundKill();
  //cerr<<"first round finished!"<<endl;
  AllTakeAction();
  //cerr<<"second action finished!"<<endl;
  SecondRoundKill();
  Count(myLivedNano,myFreeNano,myTeam_);
  Count(opLivedNano,opFreeNano,3-myTeam_);
  cerr<<"inworker"<<myLivedNano<<' '<<myFreeNano<<endl;
  cerr<<"inworker"<<opLivedNano<<' '<<opFreeNano<<endl;
  outputKillers();

  
  if (myLivedNano>0){}
    //checkOndirectionBlock();

  //return ret;
}
void Worker::SecondRoundKill() {
  int id;
  Location* tmp;
  KillerNano k;
  int dir;
  int j;
  for (int i = 0; i<nanos_->size();i++) 
    if (nanos_->at(i)->live() && nanos_->at(i)->getTeam()!=myTeam_) {
      id = nanos_->at(i)->getId();
      dir = nanos_->at(i)->getLastDir_();
      j = 0;
      tmp = NULL;
      while (j<dir && tmp == NULL) {
	tmp = locs_->at(id)->getChild(j);
	if (tmp!=NULL && tmp->isOccupied())
	  tmp = NULL;
	j++;
      }
      if (tmp!=NULL) {
	k.toKill = i;
	k.id = tmp->getID();
	k.startDir = j-1;
	killers.push_back(k);
      }
    }
}
void Worker::outputKillers() {
  for (int i = 0; i<nanos_->size();i++) {
    cerr<<"nano:"<<i<<endl;
    nanos_->at(i)->output();
  }
  for (int i = 0; i<killers.size();i++) {
    KillerNano k = killers[i];
    cerr<<"Killer"<<i<<endl;
    cerr<<"To KIllL:";
    nanos_->at(k.toKill)->output();
    cerr<<"At location:";
    locs_->at(k.id)->output();
    cerr<<"start at direction"<<k.startDir<<endl;
  }
}
void Worker::FirstRoundKill() {
  int id;
  Location* tmp;
  KillerNano k;
  for (int i = 0; i<nanos_->size();i++) 
    if (nanos_->at(i)->live() && nanos_->at(i)->getTeam()!=myTeam_) {
      int id = nanos_->at(i)->getId();
      tmp = locs_->at(id)->getUniqueChild();
      if (tmp!=NULL) {
	k.toKill = i;
	k.id = tmp->getID();
	k.startDir = -1;
	killers.push_back(k);
      }
    }
}
void Worker::Count(int & lived, int & freed,int team) {
  freed = k_;
  lived = 0;
  for (int i = 0; i<nanos_->size();i++) 
    if (nanos_->at(i)->getTeam() == team){
      //      nanos_->at(i)->output();
      freed --;
      //      cerr<<"freed"<<freed<<endl;
      if (nanos_->at(i)->live())
	lived++;
    }
}

void Worker::AllTakeAction() {
    for (int i = 0; i<nanos_->size();i++) {
      locs_->at(nanos_->at(i)->getId())->setOccupied();
    }
    for (int i = 0; i<nanos_->size();i++) 
      if (nanos_->at(i)->live()){
	cerr<<"move!"<<endl;
	nanos_->at(i)->output();
	nanos_->at(i)->move(locs_);
	nanos_->at(i)->output();
      }
    for (int i = 0; i<nanos_->size();i++) {
      locs_->at(nanos_->at(i)->getId())->setOccupied();
    }
    for (int i = 0; i<nanos_->size();i++) 
      for (int j = 0;j<nanos_->size();j++) 
	if (i!=j && nanos_->at(i)->live() && nanos_->at(j)->live()
	    && nanos_->at(i)->getId() == nanos_->at(j)->getId()){
	  if (nanos_->at(i)->getLastDir_()>nanos_->at(j)->getLastDir_()) {
	    nanos_->at(i)->die();
	    cerr<<"die!"<<endl;
	    nanos_->at(i)->output();
	  }
	  else {
	    cerr<<"die!"<<endl;
	    nanos_->at(j)->die();
	    nanos_->at(j)->output();
	  }
	}
}
//Location* Worker::findNextPosition
