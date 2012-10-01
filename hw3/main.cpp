#include<iostream>
#include<string>
#include<sstream>
#include "patient.h"
#include "ambulance.h"
#include <vector>
using namespace std;
vector<Patient*> patients;
vector<int> ambNumber;
vector<Hospital*> hospital;
void init() {
  string s;
  cin>>s;
  while(s[0]<'0'||s[0]>'9') cin>>s;
  int x,y,t;
  Patient*p;
  int i=0;
  while (s[0]>='0'&& s[0]<='9') {
    stringstream ss(s);
    char c;
    ss>>x>>c>>y>>c>>t;
    i++;
    p = new Patient(x,y,t,i);
    patients.push_back(p);
    cin>>s;
  }
  int ambNum;
  while(cin>>ambNum) {
    ambNumber.push_back(ambNum);
  }
}
void work() {
  Hospital * h = new Hospital(0,0,1);
  Ambulance amb(h);
  amb.addRountine(patients[0]);
  amb.addRountine(patients[100]);
  amb.addRountine(h);
  amb.output();
}
void workdone() {
  for (int i = 0; i<patients.size();i++)
    delete patients[i];
}
int main() {
  init();
  work();
  workdone();
}
