#include <iostream>
#include <fstream>
#include "tree.h"
using namespace std;

bool readSrvOutput(const char* srvOutFileName);

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
  string srvOutStr((istreambuf_iterator<char>(inF)), istreambuf_iterator<char>());

  // cout << srvOutStr;
  vector<char> srvStrVect(srvOutStr.begin(), srvOutStr.end());
  vector<char>::const_iterator srvStrVectIter = srvStrVect.begin();
  tree srvTr(srvStrVectIter);
  cout << "Expect(graph): " << srvTr[0][0] << endl;  // "graph"
  cout << "Expect(nodes): " << srvTr[0][1][0][0] << endl;  // "nodes"
  cout << "Expect(whole id0 ..): " << srvTr[0][1][0][1][0] << endl;  // "whole id0"

  inF.close();
  return true;
}
