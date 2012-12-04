#include <cassert>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "person.h"
#define WEIGHTGRANULARITY 200

Person::Person(const string& plyName, int nFeatures)
              : Someone(plyName, nFeatures),
                exact_w_(new double[nFeatures]) {
  const int halfValue = WEIGHTGRANULARITY / 2;
  const int range = halfValue / n_features_ + 1;
  int posCount = 0, negCount = 0;
  srand(time(NULL));

  for (int i = 0; i < n_features_; ++i) {
    int sign = rand() % 2, random = rand() % range;
    if (sign) {  // Positive weights
      posCount += random;
      exact_w_[i] = double(random) / double(halfValue);
    } else {
      negCount += random;
      exact_w_[i] = double(-random) / double(halfValue);
    }
  }

  // std::cout << "posCount= " << posCount << "   negCount= " << negCount << std::endl;
  assert(posCount <= halfValue && negCount <= halfValue);
  while (posCount < halfValue) {  // Make positive weights addup to 1.0 (include 0)
    int randPos = rand() % n_features_;
    if (exact_w_[randPos] < 0.0) continue;
    else {
      exact_w_[randPos] += 1.0 / double(halfValue);
      ++posCount;
    }
  }
  while (negCount < halfValue) {  // Make negative weights addup to 1.0 (Not count 0)
    int randPos = rand() % n_features_;
    if (exact_w_[randPos] >= 0.0) continue;
    else {
      exact_w_[randPos] -= 1.0 / double(halfValue);
      ++negCount;
    }
  }

  // std::cout << "New posCount= " << posCount << "   negCount= " << negCount
    // << std::endl;
  // Push exact w to @vect_his_
  addNewVectHistory(exact_w_);
}

Person::~Person() {
  delete [] exact_w_;
}

int Person::seed_ = 0;

void Person::randWeightsGenerator(double* aVector, int len) {
  const int halfValue = WEIGHTGRANULARITY / 2;
  const int range = halfValue / len + 1;
  int posCount = 0, negCount = 0;
  srand(time(NULL) + seed_++);

  for (int i = 0; i < len; ++i) {
    int sign = rand() % 2, random = rand() % range;
    if (sign) {  // Positive weights
      posCount += random;
      aVector[i] = double(random) / double(halfValue);
    } else {
      negCount += random;
      aVector[i] = double(-random) / double(halfValue);
    }
  }

  // std::cout << "posCount= " << posCount << "   negCount= " << negCount << std::endl;
  assert(posCount <= halfValue && negCount <= halfValue);
  while (posCount < halfValue) {  // Make positive weights addup to 1.0 (include 0)
    int randPos = rand() % len;
    if (aVector[randPos] < 0.0) continue;
    else {
      aVector[randPos] += 1.0 / double(halfValue);
      ++posCount;
    }
  }
  while (negCount < halfValue) {  // Make negative weights addup to 1.0 (Not count 0)
    int randPos = rand() % len;
    if (aVector[randPos] >= 0.0) continue;
    else {
      aVector[randPos] -= 1.0 / double(halfValue);
      ++negCount;
    }
  }

}

bool Person::isMatchmaker() const { return false; }

void Person::sendOutVector(double* aVector) {
  if (cur_his_ind_ == 1)
    getExactW(aVector);
  else {
    getWChgVector(aVector, cur_his_ind_ - 1);
  }
}

void Person::getExactW(double* wVector) {  // Make sure source and dest NO Overlap
  memcpy(wVector, exact_w_, sizeof(double) * n_features_);
  addNewVectHistory();
}

void Person::getWChgVector(double* wChgVector, int index) {
  assert(index >= 0 && index < cur_his_ind_);
  memcpy(wChgVector, vect_his_[index], sizeof(double) * n_features_);
  addNewVectHistory();
}

void Person::printExactW() const {
  for (int i = 0; i < n_features_; ++i)
    std::cout << exact_w_[i] << "  ";
  std::cout << std::endl;
}

bool Person::addNewVectHistory(const double* aVector) {
  double* tmpArr = new double[n_features_];
  if (!tmpArr) return false;
  else {
    for (int i = 0; i < n_features_; ++i)
      tmpArr[i] = aVector[i];
    vect_his_.push_back(tmpArr);
    ++cur_his_ind_;
    return true;
  }
}

void Person::gotValueForJustSentCand(double newValue) {
  assert(isMatchmaker());
}

bool Person::addNewVectHistory() {  // Generating "noise" array
  double* tmpArr = new double[n_features_];
  for (int i = 0; i < n_features_; ++i)
    tmpArr[i] = 0.0;

  int isModifyArr[n_features_];
  memset(isModifyArr, 0, sizeof(isModifyArr));
  if (!tmpArr) return false;
  else {
    const int numOfNoises = n_features_ * 0.05;
    for (int i = 0; i < numOfNoises; ) {
      int pos = rand() % n_features_;
      if (isModifyArr[pos] != 0 || fabs(exact_w_[pos]) < 0.05)
        continue;
      else {
        isModifyArr[pos] += 1;
        ++i;
      }
      int trunckW = exact_w_[pos] * 100;
      int noiseW = trunckW * double((rand() % 41) - 20) / 100.0;
      tmpArr[pos] = double(noiseW) / 100.0;
    }
    vect_his_.push_back(tmpArr);
    ++cur_his_ind_;
    return true;
  }
}
