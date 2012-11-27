#include "engine.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Engine::~Engine() {
  for (int i = 0; i<stocks_.size(); i++)
    delete stocks_[i];
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
    ss<<s;
    ss>>id>>c>>cid;
    Stock *sto = new Stock(id,cid);
    for (int i = 0; i<3; i++) {
      ss>>c>>r>>c>>p;
      sto->assignProb(i,p,r);
    }
    stocks_.push_back(sto);
    //    stocks[stocks.size()-1]->output();
    input.getline(tmp,256);
  }
  while( tmp[0]<'0' || tmp[0]>'9')
    input.getline(tmp,256);
  int i1,i2;
  while (tmp[0]<='9'&&tmp[0]>='0') {
    string s = string(tmp);
    stringstream ss;
    ss<<s;
    ss>>i1>>c>>i2;
    cout<<i1<<' '<<i2<<endl;
    stocks_[i1]->addNeighbor(i2);
    stocks_[i2]->addNeighbor(i1);
    input.getline(tmp,256);
  }
  input.close();
}
void Engine::giveRoundInfo(vector<int> &info) {
  for (int i = 0; i<info.size(); i++) {
    stocks_[i]->addRoundInfo(info[i]);
  }
}

void Engine::output() {
  for (int i = 0; i<stocks_.size(); i++)
    stocks_[i]->output();
}

vector<double> *Engine::makeDecision() {
  if (decision_.size()==0)
    decision_.resize(stocks_.size());
  double sum = 0;
  double sum2 = 0;
  for (int i = 0; i<stocks_.size(); i++) {
    decision_[i] = stocks_[i]->score();
    sum += decision_[i];
  }
  for (int i = 0; i<stocks_.size()-1; i++) {
    decision_[i] /= sum;
    if (sum2+decision_[i]>1.0) {
      decision_[i] = 1.0-sum2;
    }
    sum2+= decision_[i];
  }
  decision_[stocks_.size()-1] = 1.0 - sum2;  
  return &decision_;
}
