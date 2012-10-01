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
  cout<<"enter work"<<endl;
  kmeansToFindHospital(patients,ambNumber,hospitals,ambulances);
  cout<<"after kmeans"<<endl;
  for (int i = 0; i<ambulances.size();i++)
    ambulances[i]->move();
}
void workdone() {
  for (int i = 0; i<patients.size();i++)
    delete patients[i];
  for (int i = 0; i<ambulances.size();i++)
    delete ambulances[i];
  for (int i = 0; i<hospitals.size();i++)
    delete hospitals[i];
}
void output() {
  int total_saved = 0;
  for (int j = 0; j<ambulances.size();j++){
    ambulances[j]->output();
    total_saved+= ambulances[j]->getSavedCount();
  }
cout<<"Total saved:"<<total_saved<<endl;
}
int main() {
  init();
  work();
  output();
  workdone();
}
