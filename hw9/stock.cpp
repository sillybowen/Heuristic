#include "stock.h"
#include <iostream>
using namespace std;
void Stock::assignProb(int index, double p,double r) {
  prob_[index] = p;
  ret_[index] = r;
}

void Stock::addNeighbor(int index) {
  neighbor_.push_back(index);
}
void Stock::addRoundInfo(int info) {
  roundInfo_.push_back(info);
}
void Stock::output() {
  cout<<id_<<' '<<classID_<<' ';
  for (int i = 0; i<3; i++) {
    cout<<prob_[i]<<' '<<ret_[i]<<' ';
  }
  cout<<endl;
  cout<<"Neighbours:";
  for (int i=0 ; i<neighbor_.size(); i++)
    cout<<neighbor_[i]<<' ';
  cout<<endl;
  cout<<"Round Info:";
  for (int i = 0; i<roundInfo_.size();i++) {
    if (roundInfo_[i] == 0) 
      cout<<'h';
    else if (roundInfo_[i] == 1)
      cout<<'m';
    else cout<<'l';
    cout<<' ';
  }
  cout<<endl;
}

double Stock::score() {
  double tmp[3];
  tmp[0] = 0;
  tmp[1] = 0;
  tmp[2] = 0;
  double sum = 0;
  double ret=0;
  if (roundInfo_.size() == 0) {
    for (int i = 0; i<3; i++)
      ret += (prob_[i]*ret_[i]);
    return ret;
  }

  for (int i= 0; i<roundInfo_.size(); i++) {
    sum+=(i+1);
    tmp[roundInfo_[i]]+= (i+1);
  }
  if (tmp[0]<0.3) return 0;
  for (int i = 0; i<3; i++) {
    ret+= (ret_[i] * tmp[i]/sum);
  }
  if (ret<2.0) ret = 0;
  cout<<ret<<endl;
  return ret;
}
