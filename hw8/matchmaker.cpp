#include <cstdlib>
#include <ctime>
#include "matchmaker.h"

#define PRECISIONPRIMENUMBER 100

Matchmaker::Matchmaker(const string& plyName, int nFeatures)
                      : Someone(plyName, nFeatures), seed_(0) { }

Matchmaker::~Matchmaker() { }

bool Matchmaker::isMatchmaker() const { return true; }

void Matchmaker::sendOutVector(double* aVector) {
  srand(time(NULL) + seed_++);
  double* tmpArr = new double[n_features_];
  for (int i = 0; i < n_features_; ++i) {
    aVector[i] = double(rand() % PRECISIONPRIMENUMBER + 1)
      / double(PRECISIONPRIMENUMBER);
    tmpArr[i] = aVector[i];
  }
  vect_his_.push_back(tmpArr);
  ++cur_his_ind_;
}
