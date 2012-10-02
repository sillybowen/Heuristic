#include "bowen_work_class.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
using namespace std; 

void Bowen_work::work() {
  refresh();
    int total_saved = 0;
    for (int i = 0; i<ambulances.size();i++) {
    //    ambulances[i]->output();
    ambulances[i]->move();
  }
  for (int i = 0; i<ambulances.size();i++) {
    total_saved+=ambulances[i]->getSavedCount();
  }
  solution = total_saved;
  ambulances_solution.clear();
  for (int i = 0; i<ambulances.size();i++)
    ambulances_solution.push_back(*ambulances[i]);
  
  //  time_t t;
  // time(&t);
  srand(time(0));
    

  for (int q = 0; q<100;q++) 
    {
      refresh();
      total_saved = 0;
      for (int i = 0; i<ambulances.size();i++) {
	//	cout<<i<<endl;
	//	ambulances[i]->output();
	ambulances[i]->move2();
      }
      for (int i = 0; i<ambulances.size();i++) {
	//	ambulances[i]->output();
	total_saved+=ambulances[i]->getSavedCount();
      }
      if (total_saved>solution) {
	solution = total_saved;
	ambulances_solution.clear();
	for (int i = 0; i<ambulances.size();i++)
	  ambulances_solution.push_back(*ambulances[i]);
      }
    }
}
void Bowen_work::refresh() {
  for (int i = 0; i<patients.size();i++) {
    patients[i]->resetSaved();
  }
  for (int i = 0; i<ambulances.size();i++) {
    ambulances[i] = back_up_ambulances[i]->copy();
    ambulances[i]->setPatient(&patients);
  }
}
void Bowen_work::output() {
  cout<<"POSITION"<<endl;
  for (int i = 0; i<hospitals.size();i++) {
    hospitals[i]->outputHos();
  }
  cout<<"ROUTINE"<<endl;
  for (int j = 0; j<ambulances.size();j++){
    ambulances_solution[j].output();
  }
}
