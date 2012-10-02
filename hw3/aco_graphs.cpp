#include <cstdlib>
#include <ctime>
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
  initAdjGraph();

  // Init ambulances
  initAcoAmbulance();
}

ACOGraphs::~ACOGraphs() {
  for (int i = 0; i < numOfVertices_; ++i) {
    delete adjVertList_[i];
    delete [] matrixGraph_[i];
  }
}

void ACOGraphs::pheromonFading() {
  for (int i = 0; i < numOfVertices_; ++i) {
    for (int j = 0; j < numOfVertices_; ++j) {
      int new_pheromons = matrixGraph_[i][j].pheromones - PHEROMONESFADING;
      matrixGraph_[i][j].pheromones = (new_pheromons < 0)? 0 : new_pheromons;
      assert(matrixGraph_[i][j].pheromones >= 0);
    }
  }
}

void ACOGraphs::initAdjGraph() {
  // Clear Old data
  if (adjVertList_.size() > 0) {
    int size = adjVertList_.size();
    for (int i = 0; i < size; ++i)
      delete adjVertList_[i];
  }
  adjVertList_.clear();

  for (int i = 0; i < numOfVertices_; ++i) {
    adjVertList_.push_back(new EdgesMultiSet);
    for (int j = 0; j < numOfVertices_; ++j) {
      if (j != i && hosPatVect_[j]->isPatient()) {
        adjVertList_[i]->insert(j);
      }
    }
  }
}

void ACOGraphs::initAcoAmbulance() {
  int ambId = 0;

  // Clear old data
  if (pAmb_vect_.size() > 0) {
    int size = pAmb_vect_.size();
    for (int i = 0; i < size; ++i)
      delete pAmb_vect_[i];
  }
  pAmb_vect_.clear();

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

  assert(accumMapIter != accumPreferMap.end());

  return (*accumMapIter).second;
}

void ACOGraphs::initWithBowenResult(const vector<int>& routeList) {
  int size = routeList.size();
  for (int i = 0; i < size - 1; ++i) {  // Add pheromone for route[i] to route[i+1]
    if (routeList[i] < 0 && routeList[i+1] < 0)  // two hospitals, skip
      continue;
    int startV = (routeList[i] < 0)? (-routeList[i] - 1 + fir_hos_ind_):
      (routeList[i] - 1);
    int endV = (routeList[i + 1] < 0)? (-routeList[i + 1] - 1 + fir_hos_ind_):
      (routeList[i + 1] - 1);
    // cout << "S: " << startV << " E: " << endV << endl;
    matrixGraph_[startV][endV].pheromones = INITGREEDYPHEROMONES;
    matrixGraph_[endV][startV].pheromones = INITGREEDYPHEROMONES;
  }
}

int ACOGraphs::ACOAlgorithm(int maxRuns, const vector<int>& greedyRouteList) {
  int maxSaved = 0, tmp;
  int ambulsize = pAmb_vect_.size();
  vector<int> cpRoute[ambulsize];

  initWithBowenResult(greedyRouteList);
  while (maxRuns-- > 0) {
    srand(time(NULL) + maxRuns);
    if ((tmp = oneACORun()) > maxSaved) {
      maxSaved = tmp;
      for (int i = 0; i < ambulsize; ++i)
        cpRoute[i] = (pAmb_vect_[i]->route_);
    }
    // cout << "Saved " << tmp << endl;
    initAdjGraph();
    initAcoAmbulance();
    pheromonFading();
  }

  // print ambulance route
  for (int i = 0; i < ambulsize; ++i) {
    int routeSize = cpRoute[i].size();
    for (int j = 0; j < routeSize; ++j) {
      if (j != 0)
        cout << ',';
      if (getpHosPat(cpRoute[i][j])->isPatient()) {
        cout << 'P' << cpRoute[i][j] + 1 << ',';
      } else {
        cout << 'H' << cpRoute[i][j] - fir_hos_ind_ + 1 << ',';
      }
      getpHosPat(cpRoute[i][j])->output();
      if (getpHosPat(cpRoute[i][j])->isPatient()) {
        cout << ',' << getpHosPat(cpRoute[i][j])->getDyingTime();
      }
    }
    cout << endl;
  }
  return maxSaved;
}

int ACOGraphs::oneACORun() {
  int numAmbuls = pAmb_vect_.size();
  int numSaved = 0;

  for (int i = 0; i < numAmbuls; ++i) {
    numSaved += dispatchAmbulance(i);
  }
  return numSaved;
}

int ACOGraphs::dispatchAmbulance(int ambId) {
  int ret;
  while ((ret = moveAmbulance(ambId)) != -1);

  return pAmb_vect_[ambId]->tot_saved_;
}

// Move for ambulance ind: ambId, find one, then carry out move
// -------Update relevent states after move
// Do the move, update relevant states
int ACOGraphs::moveAmbulance(int ambId) {
  int nextVert;
  aco_g_m_.lock();
  nextVert = nextAmbulanceMove(ambId);

  if (nextVert == -1) {
    aco_g_m_.unlock();
    return -1;
  } else {
    // Do the move, update state information
    int ambulCurLoc = pAmb_vect_[ambId]->cur_loc_;
    if (hosPatVect_[nextVert]->isPatient()) {  // Pick up new patient
      EdgesIter toremove;
      if (hosPatVect_[nextVert]->isPatient()) {  // Remove bidirectional edges
        for (int i = 0; i < numOfVertices_; ++i) {
          if (i != nextVert) {
            toremove = adjVertList_[i]->find(nextVert);
            toremove = adjVertList_[i]->erase(toremove);
          }
        }
      } else {
        toremove = adjVertList_[ambulCurLoc]->find(nextVert);
        toremove = adjVertList_[ambulCurLoc]->erase(toremove);
      }
      pAmb_vect_[ambId]->cur_load_ += 1;
      if (pAmb_vect_[ambId]->cur_load_ == pAmb_vect_[ambId]->max_load_)
        pAmb_vect_[ambId]->ready_to_hos_ = true;
      if (hosPatVect_[nextVert]->getDyingTime() < pAmb_vect_[ambId]->l_dyingT_)
        pAmb_vect_[ambId]->l_dyingT_ = hosPatVect_[nextVert]->getDyingTime();
      pAmb_vect_[ambId]->onboard_.push_back(nextVert);
    } else {  // Unload patients to hospital
      pAmb_vect_[ambId]->ready_to_hos_ = false;
      pAmb_vect_[ambId]->tot_saved_ += pAmb_vect_[ambId]->cur_load_;
      pAmb_vect_[ambId]->cur_load_ = 0;
      pAmb_vect_[ambId]->l_dyingT_ = INT_MAX;
      pAmb_vect_[ambId]->onboard_.clear();
    }
    pAmb_vect_[ambId]->passed_time_ +=
      (matrixGraph_[ambulCurLoc][nextVert].distance + 1);
    pAmb_vect_[ambId]->cur_loc_ = nextVert;
    pAmb_vect_[ambId]->route_.push_back(nextVert);
    // Update pheromones
    matrixGraph_[ambulCurLoc][nextVert].pheromones += PHEROMONESONCEADDUP;
    matrixGraph_[nextVert][ambulCurLoc].pheromones += PHEROMONESONCEADDUP;
  }
  aco_g_m_.unlock();
  return 1;
}

// return the nextmove vertex id, -1 if No move possible
// Assuming holding the mutex!!!
int ACOGraphs::nextAmbulanceMove(int ambId) {
  int curLocation = pAmb_vect_[ambId]->cur_loc_;
  int curLoad = pAmb_vect_[ambId]->cur_load_;
  int numEdgesToGo = adjVertList_[curLocation]->size();
  int nearestHosId = hosPatVect_[curLocation]->getNearestHospitalID();
  int retries = MAXSTOCHASTICRETRIES;

  // Already full load
  if (curLoad == pAmb_vect_[ambId]->max_load_)
    return nearestHosId;

  if (numEdgesToGo > 0) {
    int tmp;
    while (retries-- > 0 && (tmp = ambulGoStochastic(ambId)) == -1);
    if (tmp == -1 && curLoad > 0) {  // Go to nearest hospital
      return nearestHosId;
    } else if (tmp == -1) {
      return -1;
    } else {
      return tmp;
    }
  } else if (numEdgesToGo == 0 && curLoad > 0) {
    return nearestHosId;
  } else {
    return -1;
  }
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
