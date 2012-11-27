#include "engine.h"
#include <vector>
#include <iostream>
using namespace std;
int main() {
  Engine e;
  e.ParseFile("./inputfile.txt");
  vector<int>roundinfo;
  roundinfo.resize(205);
  for (int i = 0; i<3; i++) {
    for (int id = 0; id<roundinfo.size();id++) {
      roundinfo[id] = i;
    }
    e.giveRoundInfo(roundinfo);
  }
  e.output();

  vector<double> *ret;
  ret = e.makeDecision();
  for (int i = 0; i<ret->size();i++)
    cout<<ret->at(i)<<' ';
  cout<<endl;
}
