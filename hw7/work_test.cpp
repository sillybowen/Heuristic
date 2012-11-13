#include "parser.h"
#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "nano.h"
using namespace std;
int main () {

  ifstream inF;

  inF.open("srvSampleOutput.txt", ifstream::in);
  if (!inF.good()) {
    return false;
  }
  vector<char> srvStrVect((istreambuf_iterator<char>(inF)),
      istreambuf_iterator<char>());
  vector<char>::const_iterator srvIter = srvStrVect.begin();

  Nano::initializeMap();
  tree srvTr(srvIter);
  inF.close();
  // interface to work
  int myteam = 1;
  int max_k_ = 10;
  Parser p(myteam, max_k_, string("Team1"));
  vector<Location> retloc;
  vector<Nano> retnano;
  p.work(&srvTr,retloc,retnano, 0);
  //ret are the location to deploy new nanomuncher
  //  ret.push_back(Location(0, 12, 5));
  //  printLocsToSrv(ret);
}
