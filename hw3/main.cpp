#include<iostream>
#include<string>
#include<sstream>
#include <vector>
#include <algorithm>
#include "patient.h"
#include "ambulance.h"
#include "kmeans.h"
#include "hospital_patient.h"
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
}
void work() {
  int id = 0;
  kmeansToFindHospital(patients,ambNumber,hospitals,ambulances);

  // Dump values into HospitalPatient class
  int index = 0;
  vector<HospitalPatient*>* pHosPatVector = new vector<HospitalPatient*>();
  for (int i = 0; i < patients.size(); ++i) {
    HospitalPatient* newHosPat = new HospitalPatient(patients[i]->x, patients[i]->y,
        true, index++, -1, patients[i]->getRescueTime());
    pHosPatVector->push_back(newHosPat);
  }
  for (int i = 0; i < hospitals.size(); ++i) {
    HospitalPatient* newHosPat = new HospitalPatient(hospitals[i]->x,
        hospitals[i]->y, false, index++, hospitals[i]->getAmbNumber(), -1);
    pHosPatVector->push_back(newHosPat);
  }
  // cout << "Index (expect 305)= " << index << endl;

  for (int i = 0; i<ambulances.size();i++)
    ambulances[i]->move();

  // Free mem for HospitalPatient class
  delete pHosPatVector;
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
  cout<<"POSITION"<<endl;
  for (int i = 0; i<hospitals.size();i++) {
    hospitals[i]->outputHos();
  }
  cout<<"ROUTINE"<<endl;
  for (int j = 0; j<ambulances.size();j++){
    ambulances[j]->output();
    total_saved+= ambulances[j]->getSavedCount();
  }
}
int main() {
  init();
  work();
  output();
  workdone();
}
