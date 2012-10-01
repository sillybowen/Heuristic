#include "ambulance.h" 
#include "patient.h"
#include <iostream>
using namespace std;
Ambulance::Ambulance(Hospital* h,int i) {
  routine.push_back(h);
  next_available_time_ = 0;
  id = i;
}
void Ambulance::output() {
  cout<<"amb"<<id<<' ';
  for (int i = 0; i< routine.size();i++) {
    routine[i]->output();
    if (i!=routine.size()-1)
      cout<<',';
    else
      cout<<endl;
  }
}
bool Ambulance::addRountine(Position* p){
  routine.push_back(p);
  return true;
}