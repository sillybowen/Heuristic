#include<iostream>
#include<string>
#include<sstream>
#include <vector>
#include <algorithm>
#include "patient.h"
#include "ambulance.h"
#include "kmeans.h"
#include "hospital_patient.h"
#include "aco_graphs.h"
#include "bowen_work_class.h"
using namespace std;
vector<Patient*> patients;
vector<int> ambNumber;
vector<Hospital*> hospitals;
vector<Ambulance*> ambulances;
Bowen_work *bw;
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
  bw = new Bowen_work(hospitals,patients,ambulances);
  bw->work();
  // Dump values into HospitalPatient class
  /*  int index = 0, firstHosIndex;
  vector<HospitalPatient*>* pHosPatVector = new vector<HospitalPatient*>();
  for (int i = 0; i < patients.size(); ++i) {
    HospitalPatient* newHosPat = new HospitalPatient(patients[i]->x, patients[i]->y,
        true, index++, -1, patients[i]->getRescueTime());
    pHosPatVector->push_back(newHosPat);
  }
  firstHosIndex = index;
  for (int i = 0; i < hospitals.size(); ++i) {
    HospitalPatient* newHosPat = new HospitalPatient(hospitals[i]->x,
        hospitals[i]->y, false, index++, hospitals[i]->getAmbNumber(), -1);
    pHosPatVector->push_back(newHosPat);
  }
  ACOGraphs acoGraph(pHosPatVector->size(), firstHosIndex, *pHosPatVector);
  // acoGraph.outputAdjG();
  // cout << "move prefer for ambId 0 to p2: " << acoGraph.calMovePreference(0, 2)
  //   << endl;
  cout << "--------------" << endl;
  cout << "Total patients saved = " << acoGraph.ACOAlgorithm(1000) << endl;
  cout << "--------------" << endl;
  // acoGraph.outputMatrixG();
  cout << "--------------" << endl;

  for (int i = 0; i<ambulances.size();i++)
    ambulances[i]->move();

  // Free mem for HospitalPatient class
  for (int i = 0; i < pHosPatVector->size(); ++i)
    delete pHosPatVector->at(i);
    delete pHosPatVector;*/
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
  /*  int total_saved = 0;
  cout<<"POSITION"<<endl;
  for (int i = 0; i<hospitals.size();i++) {
    hospitals[i]->outputHos();
  }
  cout<<"ROUTINE"<<endl;
  for (int j = 0; j<ambulances.size();j++){
    ambulances[j]->output();
    total_saved+= ambulances[j]->getSavedCount();
    }*/
  bw->output();

}
int main() {
  init();
  work();
  output();
  workdone();
}
