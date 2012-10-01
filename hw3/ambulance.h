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
  vector<Position*> routine;// -1 .. -4 
 public:
  Ambulance (Hospital*h,int i);
  void output();
  bool addRountine(Position* p);
};
#endif
