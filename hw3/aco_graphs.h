#ifndef ACO_Graphs_Header_
#define ACO_Graphs_Header_

#include <iostream>
#include <vector>
#include <unordered_set>
#include "hospital_patient.h"
#include "aco_ambulance.h"
#include "lock.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::unordered_multiset;
using base::Mutex;

#define SCORECAPITAL 2000
#define PHEROMONESTIMES 5

class ACOGraphs {
public:
  // Require strict ordering on vector<HospitalPatient*>, patients first
  // then hospitals
  ACOGraphs(int numVertices, int firHosInd,
      const vector<HospitalPatient*>& hosPatVect);
  ~ACOGraphs();
  void initAcoAmbulance();

  void outputAdjG() const;
  void outputMatrixG() const;
  void testForPreferBalance() const;

  long calMovePreference(int ambId, int nextVert) const;
  int ambulGoStochastic(int ambId) const;
  int nextAmbulanceMove(int ambId);

  friend class AcoAmbulance;

private:
  typedef unordered_multiset<int> EdgesMultiSet;
  typedef unordered_multiset<int>::const_iterator EdgesConstIter;
  struct EdgeInfo {
    int pheromones;
    int distance;  // |x1-x2| + |y1-y2|
  };

  const HospitalPatient* getpHosPat(int vertInd) const;

  const int                       numOfVertices_;
  const int                       fir_hos_ind_;
  Mutex                           aco_g_m_;  // Protect both graphs
  const vector<HospitalPatient*>& hosPatVect_;
  vector<EdgesMultiSet*>          adjVertList_;
  vector<EdgeInfo*>               matrixGraph_;
  vector<AcoAmbulance*>           pAmb_vect_;
};

#endif
