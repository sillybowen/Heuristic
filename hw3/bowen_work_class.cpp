#include "bowen_work_class.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
using namespace std; 

void Bowen_work::work() {
  clock_t t1,t2;
  t1 = clock();
  float seconds;
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
    
  t2 = clock();
  seconds = 0;
  int q = 0;
  while (seconds<100)
    {
      refresh();
      //      cout<<seconds<<endl;
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
      //      cout<<total_saved<<endl;
      if (total_saved>solution) {
	solution = total_saved;
	ambulances_solution.clear();
	for (int i = 0; i<ambulances.size();i++)
	  ambulances_solution.push_back(*ambulances[i]);
      }
      t2 = clock();
      seconds = ((float)t2-float(t1))/CLOCKS_PER_SEC;
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
  int sol = 0;
  cout<<"ROUTINE"<<endl;
  for (int j = 0; j<ambulances.size();j++){
    sol+= ambulances_solution[j].getSavedCount();
    ambulances_solution[j].output();
  }
  //  cout<<sol<<endl;
}
