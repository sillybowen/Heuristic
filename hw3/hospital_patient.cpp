#include "hospital_patient.h"

HospitalPatient::HospitalPatient(int xpos, int ypos, bool isPatient, int index,
    int numOfAmbuls, int dyingTime) : Position(xpos, ypos),
                                      is_patient_(isPatient),
                                      graph_id_(index),
                                      num_of_ambulances_(numOfAmbuls),
                                      dying_time_(dyingTime),
                                      nearest_hos_id_(-1),
                                      nearest_hos_dist_(-1) { }

// Return -1 if this is a Hospital, or hospital id's, between 0 to N-1
int HospitalPatient::getNearestHospitalID() const {
  return nearest_hos_id_;
}

int HospitalPatient::getNearestHospitalDis() const {
  return nearest_hos_dist_;
}

void HospitalPatient::setNearestHospital(int nearestHosID, int nearestHosDis) {
  nearest_hos_id_ = nearestHosID;
  nearest_hos_dist_ = nearestHosDis;
}
