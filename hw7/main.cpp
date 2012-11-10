#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "tree.h"
#include "loc.h"

using namespace std;

bool readSrvOutput(const char* srvOutFileName);

vector<Location*> locs;
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

  tree srvTr(srvIter);
  cout << "Expect(graph): " << srvTr[0][0] << endl;  // "graph"
  cout << "Expect(nodes): " << srvTr[0][1][0][0] << endl;  // "nodes"
  // parse in node
  cout<<srvTr[0][1][0][1]<<endl;
  cout<<srvTr[0][1][0][1].children.size()<<endl;
  cout << "Expect(whole id0 ..): " << srvTr[0][1][0][1][0][0][1] << endl;  // "whole id0"
  locs.resize(srvTr[0][1][0][1].children.size());  
  Location * lc;
  string ids,xs,ys;
  stringstream ss;
  int id,x,y;
  for (int i = 0; i<srvTr[0][1][0][1].children.size();i++) {
    cout<<srvTr[0][1][0][1][i]<<endl;
    ids = srvTr[0][1][0][1][i][0][1].data;
    xs =  srvTr[0][1][0][1][i][1][1][0][1].data;
    ys =  srvTr[0][1][0][1][i][1][1][1][1].data;
    istringstream(ids)>>id;
    istringstream(xs)>>x;
    istringstream(ys)>>y;
    lc = new Location(id,x,y);
    locs[id] = lc;
  }
  for (int i = 0; i<locs.size();i++)
    locs[i]->output();
  inF.close();
  return true;
}
