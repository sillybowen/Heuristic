#include <cassert>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "person.h"
#define WEIGHTGRANULARITY 200

Person::Person(int nFeatures)
              : n_features_(nFeatures),
                cur_his_ind_(0),
                exact_w_(new double[nFeatures]) {
  const int halfValue = WEIGHTGRANULARITY / 2;
  const int range = 2 * halfValue / nFeatures + 1;
  int posCount = 0, negCount = 0;
  srand(time(NULL));

  for (int i = 0; i < nFeatures; ++i) {
    int sign = rand() % 2, random = rand() % range;
    if (sign) {  // Positive weights
      posCount += random;
      exact_w_[i] = double(random) / double(halfValue);
    } else {
      negCount += random;
      exact_w_[i] = double(-random) / double(halfValue);
    }
  }

  std::cout << "posCount= " << posCount << "   negCount= " << negCount << std::endl;
  if (posCount < halfValue) {
  }
}

Person::~Person() {
  delete [] exact_w_;
  for (int i = 0; i < wChg_his_.size(); ++i)
    delete [] wChg_his_[i];
  wChg_his_.clear();
}

void Person::getExactW(double* wVector) {  // Make sure source and dest NO Overlap
  memcpy(wVector, exact_w_, sizeof(double) * n_features_);
}

void Person::getWChgVector(double* wChgVector, int index) {
  assert(index >= 0 && index < cur_his_ind_);
  memcpy(wChgVector, wChg_his_[index], sizeof(double) * n_features_);
}

void Person::printExactW() const {
  for (int i = 0; i < n_features_; ++i)
    std::cout << exact_w_[i] << "  ";
  std::cout << std::endl;
}
