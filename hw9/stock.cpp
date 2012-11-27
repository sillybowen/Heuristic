#include "stock.h"
#include <iostream>
using namespace std;
void Stock::AssignProb(int index, double p,double r) {
  prob_[index] = p;
  ret_[index] = r;
}

void Stock::AddNeighbor(int index) {
  neighbor_.push_back(index);
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
}
