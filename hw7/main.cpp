#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "tree.h"
#include "loc.h"
#include "parser.h"
#include "nano.h"
#include "nano_guess.h"
using namespace std;

bool readSrvOutput(const char* srvOutFileName);

//vector<Location*> locs;
int main(int argc, char* argv[]) {
  if (argc < 4) {
    puts("Format: ./main  teamName  numOfNanomunchers  srvOutputFile");
    return 1;
  }

  readSrvOutput(argv[3]);
  return 0;
}

bool readSrvOutput(const char* srvOutFileName) {
  ifstream inF;

  inF.open(srvOutFileName, ifstream::in);
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
  Parser p;
  vector<Location> ret;
  ret = p.work(&srvTr);
  //ret are the location to deploy new nanomuncher

  vector<int> tryseq;
  NanoGuess::searchOrientAtALoc(p.getLocations().at(0), tryseq);
  return true;
}
