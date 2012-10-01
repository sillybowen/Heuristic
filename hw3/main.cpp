#include<iostream>
#include<string>
#include<sstream>
#include "patient.h"
#include "ambulance.h"
#include "kmeans.h"
#include <vector>
#include <algorithm>
using namespace std;
vector<Patient*> patients;
vector<int> ambNumber;
vector<Hospital*> hospitals;
vector<Ambulance*> ambulances;
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
  sort(ambNumber.begin(),ambNumber.end());
}
void work() {
  int id = 0;
  kmeansToFindHospital(patients,ambNumber,hospitals);
  for (int i = 0; i<hospitals.size();i++) {
    hospitals[i]->outputHos();
    for (int j = 0; j<hospitals[i]->getAmbNumber();j++) {
      id++;
      Ambulance * amb = new Ambulance(hospitals[i],id);
      ambulances.push_back(amb);
    }

    for (int j = 1; j<=hospitals[i]->getAmbNumber();j++){
      ambulances[ambulances.size()-j]->output();
    }
  }
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
