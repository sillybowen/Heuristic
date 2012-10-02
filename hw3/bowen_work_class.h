#ifndef bowen_work_class_h_
#define bowen_work_class_h_
#include "patient.h"
#include "ambulance.h"
class Bowen_work {
 private:
  vector<Patient*> patients;
  vector<Hospital*> hospitals;
  vector<Ambulance*> ambulances;
  vector<Ambulance*> back_up_ambulances;
  vector<Ambulance> ambulances_solution;
  int solution ;
 public:
  Bowen_work(  vector<Hospital*> b_hospitals,
	       vector<Patient*> b_patients,
	       vector<Ambulance*> b_ambulances) {
    back_up_ambulances = b_ambulances;
    patients = b_patients;
    hospitals = b_hospitals;
    solution = 0;
    ambulances.resize(back_up_ambulances.size());
  }
  void work();
  void refresh();
  void output();
};
#endif
