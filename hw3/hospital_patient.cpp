#include "hospital_patient.h"

HospitalPatient::HospitalPatient(int xpos, int ypos, bool isPatient, int index,
    int numOfAmbuls, int dyingTime) : Position(xpos, ypos),
                                      is_patient_(isPatient),
                                      graph_id_(index),
                                      num_of_ambulances_(numOfAmbuls),
                                      dying_time_(dyingTime) {
}

// Return -1 if this is a Hospital, or hospital id's, between 0 to N-1
int HospitalPatient::getNearestHospitalID() const {
  if (is_patient_)
    return 100;
  else
    return -1;
}
