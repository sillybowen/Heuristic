#ifndef ACO_Ambulance_Header_
#define ACO_Ambulance_Header_

#include <vector>
#include "hospital_patient.h"
#include "lock.hpp"

#define MAXAMBULANCELOAD 4

using std::vector;
using base::Mutex;

class AcoAmbulance {
public:
  AcoAmbulance(int ambID, int curLocation, Mutex* pAcoGMutex);
  ~AcoAmbulance() { }

private:
  const int                amb_id_;
  const int                max_load_;
  bool                     ready_to_hos_;// true if cur_load_ == MAXAMBULANCELOAD
  int                      passed_time_; // Init to 0
  int                      cur_load_;
  int                      cur_loc_;    // location represent by HospitalPatient ind
  Mutex*                   pAco_g_m_;
  vector<HospitalPatient*> onboard_;    // do NOT transfer ownership
  vector<int>              route_;      // route represent by HospitalPatient ind
};

#endif
