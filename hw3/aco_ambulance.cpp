#include "aco_ambulance.h"

AcoAmbulance::AcoAmbulance(int ambID, int curLocation, Mutex* pAcoGMutex)
  : amb_id_(ambID),
    max_load_(MAXAMBULANCELOAD),
    ready_to_hos_(false),
    passed_time_(0),
    cur_load_(0),
    cur_loc_(curLocation),
    pAco_g_m_(pAcoGMutex) {
  // Push in the starting point(index)
  route_.push_back(curLocation);
}
