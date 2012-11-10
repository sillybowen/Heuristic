#include "nano.h"
#include<iostream>
using namespace std;

map<string,int> Nano::dirMap_;

void Nano::initializeMap() {
  Nano::dirMap_.insert(pair<string,int>("Up",0));
  Nano::dirMap_.insert(pair<string,int>("Down",1));
  Nano::dirMap_.insert(pair<string,int>("Left",2));
  Nano::dirMap_.insert(pair<string,int>("Right",3));
}
int Nano::lookFor(string s) {
  return dirMap_.find(s)->second;
}
Nano::Nano (int id, 
	    string lastDir,
	    vector<string> dir, 
	    bool live, int team): 
  live_(live),id_(id),belong_(team) {
  lastDir_ = lookFor(lastDir);
  seq_.resize(4);
  for (int i = 0; i<4;i++)
    seq_[i] = lookFor(dir[i]);
}

void Nano::output() {
  cout<<id_<<' '<<belong_<<' ';
  if (live_) cout<<"live ";
  else cout<<"dead ";
  cout<<"last dir"<<lastDir_<<' ';
  for (int i = 0; i<4; i++)
    cout<<seq_[i];
  cout<<endl;
}
