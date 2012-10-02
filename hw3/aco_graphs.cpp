#include <cassert>
#include <climits>
#include <cstring>
#include <map>
#include "aco_graphs.h"

using std::map;
using std::pair;

ACOGraphs::ACOGraphs(int numVertices, int firHosInd,
    const vector<HospitalPatient*>& hosPatVect) : numOfVertices_(numVertices),
                                                  fir_hos_ind_(firHosInd),
                                                  hosPatVect_(hosPatVect) {
  for (int i = 0; i < numVertices; ++i) {
    matrixGraph_.push_back(new EdgeInfo[numVertices]);
  }

  // Fill in EdgeInfo matrix
  for (int i = 0; i < numVertices; ++i) {
    // Self loop is invalid
    matrixGraph_[i][i].pheromones = -1;
    matrixGraph_[i][i].distance = -1;
  }
  for (int i = 0; i < numVertices; ++i) {
    int nearestHosId = 0;
    int nearestHosDis = INT_MAX;
    for (int j = i + 1; j < numVertices; ++j) {
      matrixGraph_[i][j].pheromones = 0;
      matrixGraph_[i][j].distance = hosPatVect[i]->distance(hosPatVect[j]);
      matrixGraph_[j][i].pheromones = 0;
      matrixGraph_[j][i].distance = matrixGraph_[i][j].distance;
      if ((hosPatVect_[i]->isPatient()) && !(hosPatVect_[j]->isPatient())
          && matrixGraph_[i][j].distance < nearestHosDis) {
        nearestHosDis = matrixGraph_[i][j].distance;
        nearestHosId = j;
      }
    }

    if (hosPatVect_[i]->isPatient())
      hosPatVect_[i]->setNearestHospital(nearestHosId, nearestHosDis);
  }

  // Fill in adjacent graph
  for (int i = 0; i < numVertices; ++i) {
    adjVertList_.push_back(new EdgesMultiSet);
    for (int j = 0; j < numVertices; ++j) {
      if (j != i && hosPatVect[j]->isPatient()) {
        adjVertList_[i]->insert(j);
      }
    }
  }

  // Init ambulances
  initAcoAmbulance();
}

ACOGraphs::~ACOGraphs() {
  for (int i = 0; i < numOfVertices_; ++i) {
    delete adjVertList_[i];
    delete [] matrixGraph_[i];
  }
}

void ACOGraphs::initAcoAmbulance() {
  int ambId = 0;
  for (int i = fir_hos_ind_; i < numOfVertices_; ++i) {
    int numAmbuls = hosPatVect_[i]->getNumOfAmbuls();
    while (numAmbuls-- > 0) {
      AcoAmbulance* new_ambul = new AcoAmbulance(ambId++, i, &aco_g_m_, this);
      pAmb_vect_.push_back(new_ambul);
    }
  }
  assert(ambId == (int) pAmb_vect_.size());
}

const HospitalPatient* ACOGraphs::getpHosPat(int vertInd) const {
  return hosPatVect_[vertInd];
}


void ACOGraphs::outputAdjG() const {
  for (int i = 0; i < numOfVertices_; ++i) {
    cout << "Vert(" << i << '|' << ((hosPatVect_[i]->isPatient())? 'P':'H')
      << "): ";
    EdgesConstIter cit = adjVertList_[i]->begin();
    while (cit != adjVertList_[i]->end()) {
      cout << *cit << " ";
      ++cit;
    }
    cout << endl;
  }
}

void ACOGraphs::outputMatrixG() const {
  for (int i = 0; i < numOfVertices_; ++i) {
    if (hosPatVect_[i]->isPatient())
      cout << 'P' << i << "  ";
    else
      cout << 'H' << i - fir_hos_ind_ << "  ";
  }
  cout << endl;

  for (int i = 0; i < numOfVertices_; ++i) {
    for (int j = 0; j < numOfVertices_; ++j) {
      cout << matrixGraph_[i][j].pheromones << '|' << matrixGraph_[i][j].distance
        << "  ";
    }
    cout << "NearHos " << hosPatVect_[i]->getNearestHospitalID() << '|'
      << hosPatVect_[i]->getNearestHospitalDis() << endl;
  }
}

// nextVert is the next index to move to
// Must > 1 for consideration
long ACOGraphs::calMovePreference(int ambId, int nextVert) const {
  int curLocation = pAmb_vect_[ambId]->cur_loc_;

  long timeToDie = (long) getpHosPat(nextVert)->getDyingTime();
  long dis = (long) matrixGraph_[curLocation][nextVert].distance;

  // If curLocation->nextVert->nearest hospital cannot save every one onboard
  // do NOT consider
  long minTimeToDie = (timeToDie < pAmb_vect_[ambId]->l_dyingT_)?
    timeToDie : (pAmb_vect_[ambId]->l_dyingT_);
  // '2' is for loading and unloading patients
  long totalSavingT = (pAmb_vect_[ambId]->passed_time_ + dis + 2 +
      (long) getpHosPat(nextVert)->getNearestHospitalDis());
  if (minTimeToDie < totalSavingT)
    return 0;

  long score = SCORECAPITAL / (minTimeToDie - totalSavingT + 1) +
               SCORECAPITAL / (dis + 1) + PHEROMONESTIMES *
               ((long) matrixGraph_[curLocation][nextVert].pheromones);
  if (score < 1)
    return 1;
  else
    return score;
}

// Made sure there is way to go => adjVertList_[curLocation]->size() > 0
// Assuming holding the Mutex
int ACOGraphs::ambulGoStochastic(int ambId) const {
  int curLocation = pAmb_vect_[ambId]->cur_loc_;
  // key: move preference weight, value: vertex index for the move to position
  map<long, int> accumPreferMap;
  map<long, int>::const_iterator accumMapIter;
  pair<map<long, int>::iterator, bool> ret;

  EdgesConstIter cit = adjVertList_[curLocation]->begin();
  long sum = 0;
  while (cit != adjVertList_[curLocation]->end()) {
    int moveWeight = calMovePreference(ambId, *cit);
    if (moveWeight >= 1) {  // Consider this edge
      sum += moveWeight;
      ret = accumPreferMap.insert(pair<long, int>(sum, *cit));
      assert(ret.second == true);
    }
    ++cit;
  }
  if (accumPreferMap.size() == 0)  // No more patient can save without dying others
    return -1;

  // Generate random number:
  long randVal = rand() % sum;
  accumMapIter = accumPreferMap.upper_bound(randVal);

  assert(accumMapIter == accumPreferMap.end());

  return (*accumMapIter).second;
}

// next stochastic move for ambulance ind: ambId, find one, then carry out move
// -------Update relevent states after move
// return the nextmove vertex id, -1 if No move possible
// Assuming holding the mutex!!!
int ACOGraphs::nextAmbulanceMove(int ambId) {
  int curLocation = pAmb_vect_[ambId]->cur_loc_;
  int curLoad = pAmb_vect_[ambId]->cur_load_;


  return -1;
}

// Only used for '2test' file, ambId = 0
void ACOGraphs::testForPreferBalance() const {
  const int ambId = 0;
  int countArr[4];
  for (int i = 0; i < 4; ++i)
    countArr[i] = 0;
  for (int i = 0; i < 10000; ++i)
    (countArr[ambulGoStochastic(ambId)])++;
  for (int i = 0; i < 4; ++i)
    cout << "chose next : " << i << " = " << countArr[i] << endl;
}
