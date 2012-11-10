#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "tree.h"
#include "loc.h"
#include "parser.h"
#include "nano.h"
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
  Parser p;
  p.GetNodes(&srvTr);
  p.GetEdges(&srvTr);
  p.GetEaten(&srvTr);
  p.GetNanos(&srvTr);
  //  p.output();
  return true;
}
