#include <cassert>
#include <iostream>
#include "nano.h"
using namespace std;

map<string,int> Nano::dirMap_;

void Nano::initializeMap() {
  Nano::dirMap_.insert(pair<string,int>("Up",0));
  Nano::dirMap_.insert(pair<string,int>("Down",2));
  Nano::dirMap_.insert(pair<string,int>("Left",1));
  Nano::dirMap_.insert(pair<string,int>("Right",3));
}
int Nano::lookFor(string s) {
  return dirMap_.find(s)->second;
}
Nano::Nano (int id, 
	    string lastDir,
	    vector<string> dir, 
	    bool live, int team): 
  live_(live),id_(id),team_(team) {
  lastDir_ = lookFor(lastDir);
  seq_.resize(4);
  for (int i = 0; i<4;i++)
    seq_[i] = lookFor(dir[i]);
}

Nano::Nano (int id, 
	    int lastDir,
	    vector<int> seq, 
	    int team): 
  seq_(seq),id_(id),team_(team),lastDir_(lastDir) {}


void Nano::output() {
  cerr<<id_<<' '<<team_<<' ';
  if (live_) cerr<<"live ";
  else cerr<<"dead ";
  cerr<<"last dir"<<lastDir_<<' ';
  for (int i = 0; i<4; i++)
    cerr<<seq_[i];
  cerr<<endl;
}
void Nano::move(vector<Location*> *locs) {
  int startDir;
  locs->at(id_)->output();
  for (int i = 0; i<4; i++)
    if (lastDir_ == seq_[i]) 
      startDir = i+1;
  startDir%=4;
  Location* nextLoc = NULL;
  Location* now = locs->at(id_);
  int lastdir;
  for (int i = 0; i<4; i++) 
    if (nextLoc ==NULL){
      lastdir = (startDir+i)%4;
      nextLoc = now->getChild(seq_[lastdir]);
      if (nextLoc!=NULL && !nextLoc->isOccupied()) {
	id_ = nextLoc->getID();
	lastDir_ = seq_[lastdir];
	locs->at(id_)->output();
      }
      else nextLoc = NULL;
    }
  if (nextLoc == NULL) {
    live_ = false;
  }
}
