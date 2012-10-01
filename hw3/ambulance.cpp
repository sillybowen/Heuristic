#include "ambulance.h" 
#include "patient.h"
#include <iostream>
using namespace std;
Ambulance::Ambulance(Hospital* h) {
  routine.push_back(h);
  next_available_time_ = 0;
}
void Ambulance::output() {
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
