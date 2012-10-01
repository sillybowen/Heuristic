#include "patient.h"
#include <iostream>
using namespace std;
Patient::Patient(int x,int y,int t,int i): 
  Position(x,y),
  resecue_time_(t),
  id(i){
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
Hospital::Hospital(int x,int y,int i):
  Position(x,y),
  id(i){
}

void Hospital::output() {
  cout<<"H"<<id<<','<<x<<','<<y;
}
