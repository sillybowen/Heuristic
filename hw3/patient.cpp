#include "patient.h"
#include <iostream>
using namespace std;
Patient::Patient(int x,int y,int t,int i): 
  Position(x,y),
  resecue_time_(t),
  id(i),
  saved(false){
}

void Patient::output() {
  cout<<"P"<<id<<','<<x<<','<<y<<','<<resecue_time_;
}
bool Patient::isPatient() {
  return true;
}
bool Hospital::isPatient() {
  return false;
}
Hospital::Hospital(int x,int y,int i,int ab):
  Position(x,y),
  id(i),
  ambulance_number_(ab){
}

void Hospital::output() {
  cout<<"H"<<id<<','<<x<<','<<y;
}
void Hospital::outputHos() {
  cout<<"H"<<id<<','<<x<<','<<y<<','<<ambulance_number_<<endl;
}
