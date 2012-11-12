#include <cassert>
#include <iostream>
#include "nano.h"
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

void Nano::printDirPath(const vector<int>& dirPath) {
  for (int i = 0; i < dirPath.size(); ++i) {
    switch (dirPath[i]) {
      case 0: cout << "Up "; break;
      case 1: cout << "Down "; break;
      case 2: cout << "Left "; break;
      case 3: cout << "Right "; break;
      default: cout << "Error!";
    }
  }
  cout << endl;
}

int Nano::recNanoGoWithOrient(Location* startLoc, const vector<int>& orient,
    vector<int>& nanoDirPath) {
  assert(orient.size() == 4);
  if (startLoc->isOccupied())
    return 0;

  startLoc->tried_ = true;
  int lastDir, i, counter = 0;
  if (nanoDirPath.empty()) {
    lastDir = orient[3];
    i = 0;
  } else {
    lastDir = nanoDirPath.back();
    for (i = 0; i < orient.size(); ++i) {
      if (orient[i] == lastDir) {
        i = (i + 1) % 4;
        break;
      }
    }
  }
  assert(i != orient.size());
  Location* pNextLoc;
  for ( ; counter < 4; i = (i + 1) % 4, ++counter) {
    if (startLoc->getChildren().at(i) != NULL &&
        !(startLoc->getChildren().at(i)->tried_)) {
      pNextLoc = startLoc->getChildren().at(i);
      nanoDirPath.push_back(orient[i]);
      break;
    }
  }
  if (counter == 4) {  // No more way to go
    return 1;
  } else {
    return 1 + recNanoGoWithOrient(pNextLoc, orient, nanoDirPath);
  }
}

int Nano::searchOrientAtALoc(Location* startLoc, vector<int>& tryingSeq) {
  int myints[] = {2, 1, 0, 3};
  vector<int> tmpOrient(myints, myints + sizeof(myints) / sizeof(int));
  vector<int> nanoDirPath;
  nanoDirPath.clear();
  int maxLocEaten = recNanoGoWithOrient(startLoc, tmpOrient, nanoDirPath);
  cout << "maxLocEaten = " << maxLocEaten << endl;
  printDirPath(nanoDirPath);
  return maxLocEaten;
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
