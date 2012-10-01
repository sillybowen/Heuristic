#ifndef kmeans_h_
#define kmeans_h_
#include "patient.h"
#include "ambulance.h"
#include <vector>
#include <algorithm>
using namespace std;
void kmeansToFindHospital(vector<Patient*> &patients,
                          vector<int> &ambNumber,
                          vector<Hospital*> &hospitals,
                          vector<Ambulance*>&ambulances);
#endif
