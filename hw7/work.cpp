#include "work.h"
#include <set>
#include <algorithm>
using namespace std;
bool compare(KillerNano k1, KillerNano k2) {
  if (k1.score<k2.score)
    return true;
}
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
  outputKillers();
  //cerr<<"first round finished!"<<endl;
  AllTakeAction();
  //cerr<<"second action finished!"<<endl;
  SecondRoundKill();
  Count(myLivedNano,myFreeNano,myTeam_);
  Count(opLivedNano,opFreeNano,3-myTeam_);
  cerr<<"inworker"<<myLivedNano<<' '<<myFreeNano<<endl;
  cerr<<"inworker"<<opLivedNano<<' '<<opFreeNano<<endl;
  outputKillers();

  EvaluateKillers();
  retlocs.clear();
  retnanos.clear();

  if (myFreeNano == 1) {
    Location* loc;
    Nano* nano;
    findOneToPlace(loc,nano);
    retlocs.push_back(*loc);
    retnanos.push_back(*nano);
    return ;
  }

  sort(killers.begin(),killers.end(),compare);
  int c = 0;
  for (int i = 0; i<killers.size(); i++) 
    if (killers[i].keep){
      if (c == myFreeNano-1)
	break;
      Location* l = locs_->at(killers[i].id);
      Location loc(l->getID(),l->getX(),l->getY());
      Nano nano (killers[i].id,
		 killers[i].seq[3],
		 killers[i].seq,
		 myTeam_);
      retlocs.push_back(loc);
      retnanos.push_back(nano);
      c++;
    }

  cout<<"retlocs size"<<retlocs.size()<<endl;
  //    if (retlocs.size()==0){
  if ((myLivedNano == 0 && retlocs.size()==0 )||(opFreeNano==0)) {
    Location* loc;
    Nano* nano;
    findOneToPlace(loc,nano);
    retlocs.push_back(*loc);
    retnanos.push_back(*nano);
  }
  for (int i = 0; i<retlocs.size();i++) {
    retlocs[i].output();
    retnanos[i].output();
  }
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
	tmp = locs_->at(id)->getChild(2-j);
	if (tmp!=NULL && tmp->isOccupied())
	  tmp = NULL;
	j++;
	if (tmp!=NULL) {
	  k.toKill = i;
	  k.id = tmp->getID();
	  k.startDir = j-1;
	  killers.push_back(k);
	}
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
    cerr<<"loc:";
    locs_->at(nanos_->at(k.toKill)->getId())->output();
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
int Worker::evaluate(int id, vector<int> tmpseq_,int isKiller){
  /*  cerr<<"generatedseq:";
  for (int i = 0; i<4; i++)
    cerr<<tmpseq_[i]<<' ';
    cerr<<endl;*/
  vector<Location*> modified_locs;
  set<Location*> used_locs;
  int ret = 0;
  Nano nano (id,tmpseq_[3],tmpseq_,myTeam_);
  if (isKiller>=0) {    
    int nanoid = killers[isKiller].toKill;
    int locid = nanos_->at(nanoid)->getId();
    locs_->at(locid)->setUnoccupied();
  }
  while (nano.live()) {
    used_locs.insert(locs_->at(nano.getId()));
    modified_locs.push_back(locs_->at(nano.getId()));
    locs_->at(nano.getId())->setOccupied();
    nano.move(locs_);
    ret++;
  }
  for (int i = 0;i<modified_locs.size();i++)
    modified_locs[i]->setUnoccupied();
  cerr<<"Score:"<<ret<<endl;
  if (isKiller>=0) {
    int nanoid = killers[isKiller].toKill;
    int locid = nanos_->at(nanoid)->getId();
    locs_->at(locid)->setOccupied();
  }
  return ret;
}
void Worker::generateSeq(int id, vector<int>& tmpseq_,vector<int>& retseq,int& score,int depth,int isKiller) {
  if (depth == 4) {
    int s = evaluate(id,tmpseq_,isKiller);
    if (s>score) {
      score = s;
      retseq=tmpseq_;
    }      
    return;
  }
  bool r;
  for (int i = 0; i<4;i++) {
    r = true;
    for (int j = 0; j<depth;j++)
      if (i == tmpseq_[j])
	r = false;
    if (r) {
      //      cerr<<"depth:"<<depth<<endl;
      tmpseq_[depth] = i;
      generateSeq(id,tmpseq_,retseq,score,depth+1,isKiller);
    }
  }
}
int Worker::Score(int id, vector<int>& seq_, int start,int isKiller) {
  seq_.clear();
  seq_.resize(4);
  vector<int> tmpseq;
  tmpseq.resize(4);
  int score = 0;
  if (start!=-1) {
    seq_[0]=start;
    generateSeq(id,tmpseq,seq_,score,1,isKiller);
  }
  else generateSeq(id,tmpseq,seq_,score,0,isKiller);
  return score;
}
void Worker::EvaluateKillers() {
  Location *loc;
  Nano* nano;
  for (int i = 0; i<killers.size();i++) {
    if (killers[i].startDir != -1)
      killers[i].score=Score(killers[i].id,killers[i].seq,killers[i].startDir,i);
    else
      killers[i].score=Score(killers[i].id,killers[i].seq,killers[i].startDir,-1);
    if (killers[i].score<3) killers[i].keep = false;
    else killers[i].keep = true;
  }

  for (int i = 0; i<killers.size();i++) 
    for (int j = i+1;j<killers.size();j++)
      if (killers[i].toKill == killers[j].toKill)
	if (killers[i].score<killers[j].score)
	  killers[i].keep = false;
	else
	  killers[j].keep = false;
}

int Worker::findOneToPlace(Location *&loc, Nano*&nano) {
  int longest=0;
  int id = -1;
  vector<int>  seq;
  vector<int> ansSeq;
  int tmps;
  int ansLast;
  for (int i = 0; i<locs_->size(); i++) 
    if (!locs_->at(i)->isOccupied()) {
      tmps =Score(locs_->at(i)->getID(),seq,-1,-1);
      if (tmps>longest){
	longest = tmps;
	ansSeq=seq;
	id = i;
      }
  }
  loc = new Location(id,locs_->at(id)->getX(),locs_->at(id)->getY());
  nano = new Nano(id,ansSeq[3],ansSeq,myTeam_);
  return longest;
}
