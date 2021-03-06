#ifndef Hospital_Patient_Header_
#define Hospital_Patient_Header_

#include <iostream>
#include "patient.h"

using std::cout;

class HospitalPatient : public Position {
public:
  // graph_id_ requires strict ordering, patients first, then hospitals
  HospitalPatient(int xpos, int ypos, bool isPatient, int index, int numOfAmbuls,
      int dyingTime);
  ~HospitalPatient() { }

  void output() const { cout << x << ',' << y; }
  bool isPatient() const { return is_patient_; }
  int getNumOfAmbuls() const { return num_of_ambulances_; }
  int getDyingTime() const { return dying_time_; }

  int getNearestHospitalID() const;
  int getNearestHospitalDis() const;
  void setNearestHospital(int nearestHosID, int nearestHosDis);

private:
  const bool      is_patient_;
  const int       graph_id_;
  const int       num_of_ambulances_;  // -1 for Patient
  const int       dying_time_;         // -1 for Hospital
  int             nearest_hos_id_;     // -1 for Hospital
  int             nearest_hos_dist_;   // -1 for Hospital
};

#endif
