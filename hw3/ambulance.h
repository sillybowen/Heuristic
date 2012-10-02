#ifndef ambulance_h_
#define ambulance_h_
#include "patient.h"
#include <vector>


using namespace std;
class Ambulance {
 private:
  int id;
  int next_available_time_;
  vector<Patient*> onboard_;
  vector<Position*> routine;
  vector<Patient*> *patients;
  vector<Hospital*> *hospitals;
  int saved_count_;
 public:
  Ambulance (Hospital*h,int i,vector<Patient*>* p, vector<Hospital*>*hos);
  void output();
  bool addRountine(Position* p);
  void move();
  void move2();
  void goToNearestHospital();
  void goToSave(Patient*p);
  bool couldSave(Patient*p);
  Patient* findNextToSave();
  Patient* findNextToSave2();
  bool couldSaveMore();
  int getSavedCount() {return saved_count_;}
  void setPatient(vector<Patient*>*p) {
    patients = p;
  }
  Ambulance* copy() {
    Ambulance* a = new Ambulance((Hospital*)routine[0],id,patients,hospitals);
    return a;
  }
};
#endif
