#include "engine.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Engine::~Engine() {
  for (int i = 0; i<stocks.size(); i++)
    delete stocks[i];
}
void Engine::ParseFile(string filePath) {
  ifstream input;
  input.open(filePath.c_str());
  char tmp[256];
  char c;
  int id,cid;
  double p,r;
  input.getline(tmp,256);
  input.getline(tmp,256);
  while (tmp[0]<='9'&&tmp[0]>='0') {
    string s = string(tmp);
    stringstream ss;
    cout<<s<<endl;
    ss<<s;
    ss>>id>>c>>cid;
    Stock *sto = new Stock(id,cid);
    for (int i = 0; i<3; i++) {
      ss>>c>>p>>c>>r;
      sto->AssignProb(i,p,r);
    }
    stocks.push_back(sto);
    //    stocks[stocks.size()-1]->output();
    input.getline(tmp,256);
  }
  while( tmp[0]<'0' || tmp[0]>'9')
    input.getline(tmp,256);
  int i1,i2;
  while (tmp[0]<='9'&&tmp[0]>='0') {
    string s = string(tmp);
    stringstream ss;
    cout<<s<<endl;
    ss<<s;
    ss>>i1>>c>>i2;
    cout<<i1<<' '<<i2<<endl;
    stocks[i1]->AddNeighbor(i2);
    stocks[i2]->AddNeighbor(i1);
    input.getline(tmp,256);
  }
  for (int i = 0; i<stocks.size(); i++)
    stocks[i]->output();
  input.close();
}
