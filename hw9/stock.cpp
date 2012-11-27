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
  roundInfo.push_back(info);
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
  for (int i = 0; i<roundInfo.size();i++) {
    if (roundInfo[i] == 0) 
      cout<<'h';
    else if (roundInfo[i] == 1)
      cout<<'m';
    else cout<<'l';
    cout<<' ';
  }
  cout<<endl;
}
